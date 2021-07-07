// Definition of a class to manage a Verilator simulation.
//
// This file is part of the Embecosm Debug Server target for CORE-V MCU
//
// Copyright (C) 2021 Embecosm Limited
// SPDX-License-Identifier: Apache-2.0

#include <iostream>

#include "VSim.h"

using std::cerr;
using std::cout;
using std::endl;

/// \brief Constructor for the Verilator simulator
///
/// \param[in] clkPeriodNs    Main clock period in nanoseconds
/// \param[in] tckPeriodNs    JTAG TAP clock period in nanoseconds.  Should be
///                           at least 10x the main clock period.
/// \param[in] resetPeriodNs  Reset period in nanoseconds.  Should be at
///                           least 5 JTAG clock periods.
/// \param[in] simTimeNs      Time to simulate for.  Zero means simulate
///                           forever.
VSim::VSim (const vluint64_t clkPeriodNs,
	    const vluint64_t tckPeriodNs,
	    const vluint64_t resetPeriodNs,
	    const vluint64_t simTimeNs)
{
  mContextp.reset(new VerilatedContext);
  mCpu.reset(new Vcore_v_mcu_wrapper);
  mTfp.reset(new VerilatedVcdC);

  // Set up simulation context with 1ns ticks
  mContextp->timeunit(9);
  mContextp->timeprecision(9);
  cout << "Timescale " << mContextp->timeunitString() << " / "
       << mContextp->timeprecisionString() << endl;

  // Set up tracing
  Verilated::traceEverOn (true);
  mCpu->trace (mTfp.get(), 99);
  mTfp->set_time_unit("1ns");
  mTfp->set_time_resolution("1ns");
  mTfp->open ("simx.vcd");

  // Set up clock timings reset and simulation time, warning if JTAG looks too
  // quick or reset period is too short.  Easy because we have made 1 tick =
  // 1ns.
  if ((clkPeriodNs * 10) < tckPeriodNs)
    cerr << "Warning: JTAG clock period of " << clkPeriodNs
	 << "ns less than 10x main clock period of " << tckPeriodNs << "ns"
	 << endl;

  if ((tckPeriodNs * 5) < resetPeriodNs)
    cerr << "Warning: Reset time of " << resetPeriodNs
	 << "ns less than 5x JTAG clock period of " << tckPeriodNs << "ns"
	 << endl;

  mClkHalfPeriodTicks = clkPeriodNs / 2;
  mTckHalfPeriodTicks = tckPeriodNs / 2;
  mResetPeriodTicks = resetPeriodNs;
  mSimTimeTicks = simTimeNs;

  // Initial clock/reset signal values
  mTickCount = 0;
  mContextp->time(mTickCount);

  vluint8_t nResetBit = mTickCount < mResetPeriodTicks ? 0 : 1;
  mContextp->time(mTickCount);
  mCpu->ref_clk_i = 1U;
  mCpu->rstn_i = nResetBit;

  mCpu->tck_i = 1U;
  mCpu->trst_i = nResetBit;
  mTckPosedge = true;
  mTckNegedge = false;
}

/// \brief Destructor for the Verilator simulator
///
/// Close down the VCD recording and as good practice explicitly release what
/// we created in the constructor
VSim::~VSim ()
{
  mTfp->close ();

  mTfp.reset(nullptr);
  mCpu.reset(nullptr);
  mContextp.reset(nullptr);
}

/// \brief Getter for the current time in nanoseconds
///
/// Since 1 tick = 1ns, this is easy
///
/// \return Simulated time in nanoseconds.
vluint64_t
VSim::simTimeNs () const
{
  return mSimTimeTicks;
}

/// \brief Determine if we have finished simulating
///
/// Either we have explicitly finished within the Verilog, or we are have now
/// simulated more time than the maximum simulation time (remember maximum
/// time of zero means simulate for ever.
///
/// \return  \c true if we have finished simulating, \c false otherwise.
bool
VSim::allDone () const
{
  return mContextp->gotFinish ()
    || ((mSimTimeTicks !=0) && (mContextp->time () >= mSimTimeTicks));
}

/// \brief Advance one half main clock period
///
/// Updates the time simulated, the clock and reset inputs and whether we are
/// on the posedge or negedge of the JTAG TAP clock.
void
VSim::advanceHalfPeriod ()
{
  mTickCount += mClkHalfPeriodTicks;
  mContextp->time(mTickCount);

  vluint8_t old_tck = mCpu->tck_i;
  vluint8_t nResetBit = mTickCount < mResetPeriodTicks ? 0 : 1;

  mCpu->ref_clk_i = 1U - (mTickCount / mClkHalfPeriodTicks) % 2;
  mCpu->rstn_i = nResetBit;

  mCpu->tck_i = 1U - (mTickCount / mTckHalfPeriodTicks) % 2;
  mCpu->trst_i = nResetBit;

  mTckPosedge = (old_tck == 0U) && (mCpu->tck_i == 1U);
  mTckNegedge = (old_tck == 1U) && (mCpu->tck_i == 0U);
}

/// \brief Determine if the model is in reset
///
/// Just a matter of looking at the time.
///
/// \return \c true if we are in reset, \c false otherwise.
bool
VSim::inReset () const
{
  return mTickCount < mResetPeriodTicks;
}

/// \brief Determine if we have a positive edge of the JTAG TAP clock
///
/// The actual status is determined when we advance time.  Since the JTAG
/// clock is (much) slower than the main clock, most of the time we will
/// be on neither a positive or negative edge clock.  However we should only
/// change JTAG inputs on a positive edge and read outputs on a negative edge.
///
/// \return \c true if we are on a JTAG positive edge, \c false otherwise.
bool
VSim::tapPosedge () const
{
  return mTckPosedge;
}

/// \brief Determine if we have a negative edge of the JTAG TAP clock
///
/// \see VSim::jtagPosedge for an explanation.
///
/// \return \c true if we are on a JTAG negative edge, \c false otherwise.
bool
VSim::tapNegedge () const
{
  return mTckNegedge;
}

/// \brief evaluate the Verilator model
///
/// Use the underlying Verilator model call and dump trace output
void
VSim::eval ()
{
  mCpu->eval ();
  mTfp->dump (mContextp->time ());

}

/// \brief Setter for the TDI input port
///
/// We take the input as a bool, the signal value is a \c vluint8_t, hence the
/// explicity logic used, although it will compile down efficiently.
///
/// \param[in] tdi_  The value to set on the TDI port.
void
VSim::tdi (const bool tdi_)
{
  mCpu->tdi_i = tdi_ ? 1U : 0U;
}

/// \brief Getter for the TDI input port
///
/// It is sometime convenient to read back the value of the TDI input port.
/// We explicitly convert the signal type of \c vluint8_t to \c bool, although
/// this will compile efficiently.
///
/// \return The value of the TDI port.
bool
VSim::tdi () const
{
  return mCpu->tdi_i != 0U;
}

/// \brief Getter for the TDO output port
///
/// \note There is no setter, since we should not be setting an output port.
///
/// We explicitly convert the signal type of \c vluint8_t to \c bool, although
/// this will compile efficiently.
///
/// \return The value of the TDO port.
bool
VSim::tdo () const
{
  return mCpu->tdo_o != 0U;
}

/// \brief Setter for the TDI input port
///
/// \see VSim::tms (const bool) for an explanation of the logic.
///
/// \param[in] tms_  The value to set on the TMS port.
void
VSim::tms (const bool tms_)
{
  mCpu->tms_i = tms_ ? 1U : 0U;
}

/// \brief Getter for the TMS input port
///
/// \see VSim::tms () for an explanation of the logic.
///
/// \return The value of the TMS port.
bool
VSim::tms () const
{
  return mCpu->tms_i != 0U;
}
