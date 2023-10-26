// Declaration of the Debug Module Interface
//
// This file is part of the Embecosm Debug Server target for CORE-V MCU
//
// Copyright (C) 2021 Embecosm Limited
// SPDX-License-Identifier: Apache-2.0

#ifndef DMI_H
#define DMI_H

#include <cstdint>
#include <map>
#include <memory>

#include "IDtm.h"

/// \brief The class modeling the Debug Module Interface
///
/// This sits on top of the Debug Transport Module
///
/// Within this class we provide classes to represent each of the registers.
class Dmi
{
public:
  /// \brief The class modeling the abstract \c data registers.
  class Data
  {
  public:
    /// \brief The number of abstract \c data registers
    static const std::size_t NUM_REGS = 12;

    // Constructors & destructor
    Data () = delete;
    Data (std::unique_ptr<IDtm> &dtm_);
    ~Data () = default;

    // Delete copy operator
    Data &operator= (const Data &) = delete;

    // API
    void read (const std::size_t n);
    void reset (const std::size_t n);
    void write (const std::size_t n);
    uint32_t data (const std::size_t n) const;
    void data (const std::size_t n, const uint32_t dataVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Data> &p);

  private:
    /// \brief The address of the \c data registers in the DMI
    static constexpr uint64_t DMI_ADDR[NUM_REGS]
        = { 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf };

    /// \brief The reset value of the \c data registers in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Data registers
    uint32_t mDataReg[NUM_REGS];
  };

  /// \brief The class modeling the \c dmcontrol register.
  class Dmcontrol
  {
  public:
    // Constructors & destructor
    Dmcontrol () = delete;
    Dmcontrol (std::unique_ptr<IDtm> &dtm_);
    ~Dmcontrol () = default;

    // Delete copy operator
    Dmcontrol &operator= (const Dmcontrol &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    void prettyPrint (const bool flag);
    void haltreq (const bool flag);
    void resumereq ();
    bool hartreset () const;
    void hartreset (const bool flag);
    void ackhavereset ();
    bool hasel () const;
    void hasel (const bool flag);
    uint32_t hartsel () const;
    void hartsel (const uint32_t hartselVal);
    uint32_t hartselMax ();
    void setresethaltreq ();
    void clrresethaltreq ();
    bool ndmreset () const;
    void ndmreset (const bool flag);
    bool dmactive () const;
    void dmactive (const bool flag);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Dmcontrol> &p);

  private:
    /// \brief Masks for flag bits in \c dmcontrol
    enum Masks
    {
      HALTREQ_MASK = 0x80000000,
      RESUMEREQ_MASK = 0x40000000,
      HARTRESET_MASK = 0x20000000,
      ACKHAVERESET_MASK = 0x10000000,
      HASEL_MASK = 0x04000000,
      HARTSELLO_MASK = 0x03ff0000,
      HARTSELHI_MASK = 0x0000ffc0,
      SETRESETHALTREQ_MASK = 0x00000008,
      CLRRESETHALTREQ_MASK = 0x00000004,
      NDMRESET_MASK = 0x00000002,
      DMACTIVE_MASK = 0x00000001,
    };

    /// \brief Offsets for flag bits in \c dmcontrol
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      HALTREQ_OFFSET = 31,
      RESUMEREQ_OFFSET = 30,
      HARTRESET_OFFSET = 29,
      ACKHAVERESET_OFFSET = 28,
      HASEL_OFFSET = 26,
      HARTSELLO_OFFSET = 16,
      HARTSELHI_OFFSET = 6,
      SETRESETHALTREQ_OFFSET = 3,
      CLRRESETHALTREQ_OFFSET = 2,
      NDMRESET_OFFSET = 1,
      DMACTIVE_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c dmcontrol
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      HALTREQ_SIZE = 1,
      RESUMEREQ_SIZE = 1,
      HARTRESET_SIZE = 1,
      ACKHAVERESET_SIZE = 1,
      HASEL_SIZE = 1,
      HARTSELLO_SIZE = 10,
      HARTSELHI_SIZE = 10,
      SETRESETHALTREQ_SIZE = 1,
      CLRRESETHALTREQ_SIZE = 1,
      NDMRESET_SIZE = 1,
      DMACTIVE_SIZE = 1,
    };

    /// \brief The address of the \c dmcontrol register in the DMI.
    static const uint64_t DMI_ADDR = 0x10;

    /// \brief The reset value of the \c dmcontrol register in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief The most recently selected value of \c hartsel
    uint32_t mCurrentHartsel;

    /// \brief Whether pretty printing is enabled for the \c dmcontrol
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using.
    std::unique_ptr<IDtm> &mDtm;

    /// \brief the value of the Dmcontrol register.
    uint32_t mDmcontrolReg;
  };

  /// \brief The class modeling the \c dmstatus register.
  class Dmstatus
  {
  public:
    // Constructors & destructor
    Dmstatus () = delete;
    Dmstatus (std::unique_ptr<IDtm> &dtm_);
    ~Dmstatus () = default;

    // Delete copy operator
    Dmstatus &operator= (const Dmstatus &) = delete;

    // API
    void read ();
    void prettyPrint (const bool flag);
    bool impebreak () const;
    bool havereset () const;
    bool resumeack () const;
    bool nonexistent () const;
    bool unavail () const;
    bool running () const;
    bool halted () const;
    bool authenticated () const;
    bool authbusy () const;
    bool hasresethaltreq () const;
    bool confstrptrvalid () const;
    uint8_t version () const;

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Dmstatus> &p);

  private:
    /// \brief Masks for flag bits in \c dmstatus
    enum Masks
    {
      IMPEBREAK_MASK = 0x00400000,
      ALLHAVERESET_MASK = 0x00080000,
      ANYHAVERESET_MASK = 0x00040000,
      ALLRESUMEACK_MASK = 0x00020000,
      ANYRESUMEACK_MASK = 0x00010000,
      ALLNONEXISTENT_MASK = 0x00008000,
      ANYNONEXISTENT_MASK = 0x00004000,
      ALLUNAVAIL_MASK = 0x00002000,
      ANYUNAVAIL_MASK = 0x00001000,
      ALLRUNNING_MASK = 0x00000800,
      ANYRUNNING_MASK = 0x00000400,
      ALLHALTED_MASK = 0x00000200,
      ANYHALTED_MASK = 0x00000100,
      AUTHENTICATED_MASK = 0x00000080,
      AUTHBUSY_MASK = 0x00000040,
      HASRESETHALTREQ_MASK = 0x00000020,
      CONFSTRPTRVALID_MASK = 0x00000010,
      VERSION_MASK = 0x0000000f,
    };

    /// \brief Offsets for flag bits in \c dmstatus
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      IMPEBREAK_OFFSET = 22,
      ALLHAVERESET_OFFSET = 19,
      ANYHAVERESET_OFFSET = 18,
      ALLRESUMEACK_OFFSET = 17,
      ANYRESUMEACK_OFFSET = 16,
      ALLNONEXISTENT_OFFSET = 15,
      ANYNONEXISTENT_OFFSET = 14,
      ALLUNAVAIL_OFFSET = 13,
      ANYUNAVAIL_OFFSET = 12,
      ALLRUNNING_OFFSET = 11,
      ANYRUNNING_OFFSET = 10,
      ALLHALTED_OFFSET = 9,
      ANYHALTED_OFFSET = 8,
      AUTHENTICATED_OFFSET = 7,
      AUTHBUSY_OFFSET = 6,
      HASRESETHALTREQ_OFFSET = 5,
      CONFSTRPTRVALID_OFFSET = 4,
      VERSION_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c dmstatus
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      IMPEBREAK_SIZE = 1,
      ALLHAVERESET_SIZE = 1,
      ANYHAVERESET_SIZE = 1,
      ALLRESUMEACK_SIZE = 1,
      ANYRESUMEACK_SIZE = 1,
      ALLNONEXISTENT_SIZE = 1,
      ANYNONEXISTENT_SIZE = 1,
      ALLUNAVAIL_SIZE = 1,
      ANYUNAVAIL_SIZE = 1,
      ALLRUNNING_SIZE = 1,
      ANYRUNNING_SIZE = 1,
      ALLHALTED_SIZE = 1,
      ANYHALTED_SIZE = 1,
      AUTHENTICATED_SIZE = 1,
      AUTHBUSY_SIZE = 1,
      HASRESETHALTREQ_SIZE = 1,
      CONFSTRPTRVALID_SIZE = 1,
      VERSION_SIZE = 4,
    };

    /// \brief The address of the \c dmstatus register in the DMI
    static const uint64_t DMI_ADDR = 0x11;

    /// \brief Whether pretty printing is enabled for the \c dmstatus
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Dmstatus register
    uint32_t mDmstatusReg;
  };

  /// \brief The class modeling the \c hartinfo register.
  class Hartinfo
  {
  public:
    // Constructors & destructor
    Hartinfo () = delete;
    Hartinfo (std::unique_ptr<IDtm> &dtm_);
    ~Hartinfo () = default;

    // Delete copy operator
    Hartinfo &operator= (const Hartinfo &) = delete;

    // API
    void read ();
    void prettyPrint (const bool flag);
    uint8_t nscratch () const;
    bool dataaccess () const;
    uint8_t datasize () const;
    uint16_t dataaddr () const;

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Hartinfo> &p);

  private:
    /// \brief Masks for flag bits in \c hartinfo
    enum Masks
    {
      NSCRATCH_MASK = 0x00f00000,
      DATAACCESS_MASK = 0x00010000,
      DATASIZE_MASK = 0x0000f000,
      DATAADDR_MASK = 0x00000fff,
    };

    /// \brief Offsets for flag bits in \c hartinfo
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      NSCRATCH_OFFSET = 20,
      DATAACCESS_OFFSET = 16,
      DATASIZE_OFFSET = 12,
      DATAADDR_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c hartinfo
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      NSCRATCH_SIZE = 4,
      DATAACCESS_SIZE = 1,
      DATASIZE_SIZE = 4,
      DATAADDR_SIZE = 12,
    };

    /// \brief The address of the \c hartinfo register in the DMI
    static const uint64_t DMI_ADDR = 0x12;

    /// \brief Whether pretty printing is enabled for the \c hartinfo
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Hartinfo register
    uint32_t mHartinfoReg;
  };

  /// \brief The class modeling the \c haltsum registers.
  class Haltsum
  {
  public:
    /// \brief The number of \c haltsum registers
    static const std::size_t NUM_REGS = 4;

    // Constructors & destructor
    Haltsum () = delete;
    Haltsum (std::unique_ptr<IDtm> &dtm_);
    ~Haltsum () = default;

    // Delete copy operator
    Haltsum &operator= (const Haltsum &) = delete;

    // API
    void read (const std::size_t n);
    uint32_t haltsum (const std::size_t n) const;

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Haltsum> &p);

  private:
    /// \brief The address of the \c haltsum registers in the DMI
    static constexpr uint64_t DMI_ADDR[NUM_REGS] = { 0x40, 0x13, 0x34, 0x35 };

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Haltsum registers
    uint32_t mHaltsumReg[NUM_REGS];
  };

  /// \brief The class modeling the \c hawindowsel register.
  class Hawindowsel
  {
  public:
    // Constructors & destructor
    Hawindowsel () = delete;
    Hawindowsel (std::unique_ptr<IDtm> &dtm_);
    ~Hawindowsel () = default;

    // Delete copy operator
    Hawindowsel &operator= (const Hawindowsel &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    uint16_t hawindowsel () const;
    void hawindowsel (const uint16_t hawindowselVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Hawindowsel> &p);

  private:
    /// \brief Masks for flag bits in \c hawindowsel
    enum Masks
    {
      HAWINDOWSEL_MASK = 0x00007fff,
    };

    /// \brief Offsets for flag bits in \c hawindowsel
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      HAWINDOWSEL_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c hawindowsel
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      HAWINDOWSEL_SIZE = 15,
    };

    /// \brief The address of the \c hawindowsel register in the DMI
    static const uint64_t DMI_ADDR = 0x14;

    /// \brief The reset value of the \c hawindowsel register in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Hawindowsel register
    uint32_t mHawindowselReg;
  };

  /// \brief The class modeling the \c hawindow register.
  class Hawindow
  {
  public:
    // Constructors & destructor
    Hawindow () = delete;
    Hawindow (std::unique_ptr<IDtm> &dtm_);
    ~Hawindow () = default;

    // Delete copy operator
    Hawindow &operator= (const Hawindow &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    uint32_t hawindow () const;
    void hawindow (const uint32_t hawindowVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Hawindow> &p);

  private:
    /// \brief The address of the \c hawindow register in the DMI
    static const uint64_t DMI_ADDR = 0x15;

    /// \brief The reset value of the \c hawindow register in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Hawindow register
    uint32_t mHawindowReg;
  };

  /// \brief The class modeling the \c abstractcs register.
  class Abstractcs
  {
  public:
    // Public enumeration of cmderr falues
    enum CmderrVal
    {
      CMDERR_NONE = 0,
      CMDERR_BUSY = 1,
      CMDERR_UNSUPPORTED = 2,
      CMDERR_EXCEPT = 3,
      CMDERR_HALT_RESUME = 4,
      CMDERR_BUS = 5,
      CMDERR_OTHER = 7,
      CMDERR_UNKNOWN = -1,
    };

    // Constructors & destructor
    Abstractcs () = delete;
    Abstractcs (std::unique_ptr<IDtm> &dtm_);
    ~Abstractcs () = default;

    // Delete copy operator
    Abstractcs &operator= (const Abstractcs &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    void prettyPrint (const bool flag);
    uint8_t progbufsize () const;
    bool busy () const;
    CmderrVal cmderr () const;
    void cmderrClear ();
    uint8_t datacount () const;
    static const char *cmderrName (CmderrVal err);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Abstractcs> &p);

  private:
    /// \brief Masks for flag bits in \c abstractcs
    enum Masks
    {
      PROGBUFSIZE_MASK = 0x1f000000,
      BUSY_MASK = 0x00001000,
      CMDERR_MASK = 0x00000700,
      DATACOUNT_MASK = 0x0000000f,
    };

    /// \brief Offsets for flag bits in \c abstractcs
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      PROGBUFSIZE_OFFSET = 24,
      BUSY_OFFSET = 12,
      CMDERR_OFFSET = 8,
      DATACOUNT_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c abstractcs
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      PROGBUFSIZE_SIZE = 5,
      BUSY_SIZE = 1,
      CMDERR_SIZE = 3,
      DATACOUNT_SIZE = 4,
    };

    /// \brief The address of the \c abstractcs register in the DMI.
    static const uint64_t DMI_ADDR = 0x16;

    /// \brief The reset value of the \c abstractcs register in the DMI.
    ///
    /// Set all the \c cmderr bits to 1 t clear.
    static const uint32_t RESET_VALUE = CMDERR_MASK;

    /// \brief Whether pretty printing is enabled for the \c abstractcs
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using.
    std::unique_ptr<IDtm> &mDtm;

    /// \brief the value of the Abstractcs register.
    uint32_t mAbstractcsReg;
  };

  /// \brief The class modeling the \c command register.
  ///
  /// \note This is a write only register, thus no read method is needed.
  class Command
  {
  public:
    /// \brief An enumeration for the type of abstract command.
    enum CmdtypeEnum
    {
      ACCESS_REG = 0,
      QUICK_ACCESS = 1,
      ACCESS_MEM = 2,
    };

    // \brief An enumeration indicating the size of memory/register acces.
    enum AasizeEnum
    {
      ACCESS8 = 0,
      ACCESS16 = 1,
      ACCESS32 = 2,
      ACCESS64 = 3,
      ACCESS128 = 4,
    };

    // Constructors & destructor
    Command () = delete;
    Command (std::unique_ptr<IDtm> &dtm_);
    ~Command () = default;

    // Delete copy operator
    Command &operator= (const Command &) = delete;

    // API
    void reset ();
    void write ();
    void prettyPrint (const bool flag);
    void cmdtype (const CmdtypeEnum cmdtypeVal);
    void control (const uint32_t controlVal);
    void aamvirtual (bool flag);
    void aarsize (const AasizeEnum aarsizeVal);
    void aamsize (const AasizeEnum aamsizeVal);
    void aapostincrement (const bool flag);
    void aapostexec (const bool flag);
    void aatransfer (const bool flag);
    void aawrite (const bool flag);
    void aatargetSpecific (uint8_t val);
    void aaregno (uint16_t val);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Command> &p);

  private:
    /// \brief Masks for flag bits in \c command
    enum Masks
    {
      CMDTYPE_MASK = 0xff000000,
      CONTROL_MASK = 0x00ffffff,
      AAMVIRTUAL_MASK = 0x00800000,
      AARSIZE_MASK = 0x00700000,
      AAMSIZE_MASK = 0x00700000,
      AAPOSTINCREMENT_MASK = 0x00080000,
      POSTEXEC_MASK = 0x00040000,
      TRANSFER_MASK = 0x00020000,
      WRITE_MASK = 0x00010000,
      TARGET_SPECIFIC_MASK = 0x0000c000,
      REGNO_MASK = 0x0000ffff,
    };

    /// \brief Offsets for flag bits in \c command
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      CMDTYPE_OFFSET = 24,
      CONTROL_OFFSET = 0,
      AAMVIRTUAL_OFFSET = 23,
      AARSIZE_OFFSET = 20,
      AAMSIZE_OFFSET = 20,
      AAPOSTINCREMENT_OFFSET = 19,
      POSTEXEC_OFFSET = 18,
      TRANSFER_OFFSET = 17,
      WRITE_OFFSET = 16,
      TARGET_SPECIFIC_OFFSET = 14,
      REGNO_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c command
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      CMDTYPE_SIZE = 8,
      CONTROL_SIZE = 24,
      AAMVIRTUAL_SIZE = 1,
      AARSIZE_SIZE = 3,
      AAMSIZE_SIZE = 1,
      AAPOSTINCREMENT_SIZE = 1,
      POSTEXEC_SIZE = 1,
      TRANSFER_SIZE = 1,
      WRITE_SIZE = 1,
      TARGET_SPECIFIC_SIZE = 2,
      REGNO_SIZE = 16,
    };

    /// \brief The address of the \c command register in the DMI.
    static const uint64_t DMI_ADDR = 0x17;

    /// \brief The reset value of the \c command register in the DMI.
    ///
    /// \todo  Is this the correct value for the reset value of the \c command
    ///        register in the DMI.
    static const uint32_t RESET_VALUE = 0;

    /// \brief Whether pretty printing is enabled for the \c command
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using.
    std::unique_ptr<IDtm> &mDtm;

    /// \brief the value of the Command register.
    uint32_t mCommandReg;
  };

  /// \brief The class modeling the \c abstractauto register.
  ///
  /// \note This is a write only register, thus no read method is needed.
  class Abstractauto
  {
  public:
    // Constructors & destructor
    Abstractauto () = delete;
    Abstractauto (std::unique_ptr<IDtm> &dtm_);
    ~Abstractauto () = default;

    // Delete copy operator
    Abstractauto &operator= (const Abstractauto &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    void prettyPrint (const bool flag);
    uint16_t autoexecprogbuf () const;
    void autoexecprogbuf (const uint16_t autoexecprogbufVal);
    uint16_t autoexecdata () const;
    void autoexecdata (const uint16_t autoexecdataVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Abstractauto> &p);

  private:
    /// \brief Masks for flag bits in \c abstractauto
    enum Masks
    {
      AUTOEXECPROGBUF_MASK = 0xffff0000,
      AUTOEXECDATA_MASK = 0x00000fff,
    };

    /// \brief Offsets for flag bits in \c abstractauto
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      AUTOEXECPROGBUF_OFFSET = 16,
      AUTOEXECDATA_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c abstractauto
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      AUTOEXECPROGBUF_SIZE = 16,
      AUTOEXECDATA_SIZE = 12,
    };

    /// \brief The address of the \c abstractauto register in the DMI.
    static const uint64_t DMI_ADDR = 0x18;

    /// \brief The reset value of the \c abstractauto register in the DMI.
    static const uint32_t RESET_VALUE = 0;

    /// \brief Whether pretty printing is enabled for the \c abstractauto
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using.
    std::unique_ptr<IDtm> &mDtm;

    /// \brief the value of the Abstractauto register.
    uint32_t mAbstractautoReg;
  };

  /// \brief The class modeling the \c confstrptr registers.
  class Confstrptr
  {
  public:
    /// \brief The number of \c confstrptr registers
    static const std::size_t NUM_REGS = 4;

    // Constructors & destructor
    Confstrptr () = delete;
    Confstrptr (std::unique_ptr<IDtm> &dtm_);
    ~Confstrptr () = default;

    // Delete copy operator
    Confstrptr &operator= (const Confstrptr &) = delete;

    // API
    void read (const std::size_t n);
    uint32_t confstrptr (const std::size_t n) const;

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Confstrptr> &p);

  private:
    /// \brief The address of the \c confstrptr registers in the DMI
    static constexpr uint64_t DMI_ADDR[NUM_REGS] = { 0x19, 0x1a, 0x1b, 0x1c };

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Confstrptr registers
    uint32_t mConfstrptrReg[NUM_REGS];
  };

  /// \brief The class modeling the \c nextdm register.
  class Nextdm
  {
  public:
    // Constructors & destructor
    Nextdm () = delete;
    Nextdm (std::unique_ptr<IDtm> &dtm_);
    ~Nextdm () = default;

    // Delete copy operator
    Nextdm &operator= (const Nextdm &) = delete;

    // API
    void read ();
    uint32_t nextdm () const;
    void nextdm (const uint32_t nextdmVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Nextdm> &p);

  private:
    /// \brief The address of the \c nextdm register in the DMI
    static const uint64_t DMI_ADDR = 0x1d;

    /// \brief The reset value of the \c nextdm register in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Nextdm register
    uint32_t mNextdmReg;
  };

  /// \brief The class modeling the \c progbuf registers.
  class Progbuf
  {
  public:
    /// \brief The number of \c progbuf registers
    static const std::size_t NUM_REGS = 16;

    // Constructors & destructor
    Progbuf () = delete;
    Progbuf (std::unique_ptr<IDtm> &dtm_);
    ~Progbuf () = default;

    // Delete copy operator
    Progbuf &operator= (const Progbuf &) = delete;

    // API
    void read (const std::size_t n);
    void reset (const std::size_t n);
    void write (const std::size_t n);
    uint32_t progbuf (const std::size_t n) const;
    void progbuf (const std::size_t n, const uint32_t progbufVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Progbuf> &p);

  private:
    /// \brief The address of the \c progbuf registers in the DMI
    static constexpr uint64_t DMI_ADDR[NUM_REGS]
        = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
            0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f };

    /// \brief The reset value of the \c progbuf registers in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Progbuf registers
    uint32_t mProgbufReg[NUM_REGS];
  };

  /// \brief The class modeling the \c authdata register.
  ///
  /// \warning  Since \c dmcontrol does not support authentication, it would be
  ///           wise to assume that this is really a dummy register.
  class Authdata
  {
  public:
    // Constructors & destructor
    Authdata () = delete;
    Authdata (std::unique_ptr<IDtm> &dtm_);
    ~Authdata () = default;

    // Delete copy operator
    Authdata &operator= (const Authdata &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    uint32_t authdata () const;
    void authdata (const uint32_t authdataVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Authdata> &p);

  private:
    /// \brief The address of the \c authdata register in the DMI
    static const uint64_t DMI_ADDR = 0x30;

    /// \brief The reset value of the \c authdata register in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Authdata register
    uint32_t mAuthdataReg;
  };

  /// \brief The class modeling the \c sbaddress registers.
  class Sbaddress
  {
  public:
    /// \brief The number of \c sbaddress registers
    static const std::size_t NUM_REGS = 4;

    // Constructors & destructor
    Sbaddress () = delete;
    Sbaddress (std::unique_ptr<IDtm> &dtm_);
    ~Sbaddress () = default;

    // Delete copy operator
    Sbaddress &operator= (const Sbaddress &) = delete;

    // API
    void read (const std::size_t n);
    void reset (const std::size_t n);
    void write (const std::size_t n);
    uint32_t sbaddress (const std::size_t n) const;
    void sbaddress (const std::size_t n, const uint32_t sbaddressVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Sbaddress> &p);

  private:
    /// \brief The address of the \c sbaddress registers in the DMI
    static constexpr uint64_t DMI_ADDR[NUM_REGS] = { 0x39, 0x3a, 0x3b, 0x37 };

    /// \brief The reset value of the \c sbaddress registers in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Sbaddress registers
    uint32_t mSbaddressReg[NUM_REGS];
  };

  /// \brief The class modeling the \c sbcs register.
  class Sbcs
  {
  public:
    // Public enumeration of sbaccess values
    enum SbaccessVal
    {
      SBACCESS_8 = 0,
      SBACCESS_16 = 1,
      SBACCESS_32 = 2,
      SBACCESS_64 = 3,
      SBACCESS_128 = 4,
      SBACCESS_UNKNOWN = 5
    };

    // Public enumeration of sberror values
    enum SberrorVal
    {
      SBERR_NONE = 0,
      SBERR_TIMEOUT = 1,
      SBERR_BAD_ADDR = 2,
      SBERR_ALIGNMENT = 3,
      SBERR_BAD_SIZE = 4,
      SBERR_OTHER = 7,
      SBERR_UNKNOWN = 8
    };

    // Constructors & destructor
    Sbcs () = delete;
    Sbcs (std::unique_ptr<IDtm> &dtm_);
    ~Sbcs () = default;

    // Delete copy operator
    Sbcs &operator= (const Sbcs &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    void prettyPrint (const bool flag);
    uint8_t sbversion () const;
    bool sbbusyerror () const;
    void sbbusyerrorClear ();
    bool sbbusy () const;
    bool sbreadonaddr () const;
    void sbreadonaddr (const bool flag);
    SbaccessVal sbaccess () const;
    void sbaccess (const uint8_t val);
    bool sbautoincrement () const;
    void sbautoincrement (const bool flag);
    bool sbreadondata () const;
    void sbreadondata (const bool flag);
    SberrorVal sberror () const;
    void sberrorClear ();
    uint8_t sbasize () const;
    bool sbaccess128 () const;
    bool sbaccess64 () const;
    bool sbaccess32 () const;
    bool sbaccess16 () const;
    bool sbaccess8 () const;
    static const char *sbversionName (uint8_t val);
    static const char *sbaccessName (SbaccessVal val);
    static const char *sberrorName (SberrorVal val);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Sbcs> &p);

  private:
    /// \brief Masks for flag bits in \c sbcs
    enum Masks
    {
      SBVERSION_MASK = 0xe0000000,
      SBBUSYERROR_MASK = 0x00400000,
      SBBUSY_MASK = 0x00200000,
      SBREADONADDR_MASK = 0x00100000,
      SBACCESS_MASK = 0x000e0000,
      SBAUTOINCREMENT_MASK = 0x00010000,
      SBREADONDATA_MASK = 0x00008000,
      SBERROR_MASK = 0x00007000,
      SBASIZE_MASK = 0x00000fe0,
      SBACCESS128_MASK = 0x00000010,
      SBACCESS64_MASK = 0x00000008,
      SBACCESS32_MASK = 0x00000004,
      SBACCESS16_MASK = 0x00000002,
      SBACCESS8_MASK = 0x00000001,
    };

    /// \brief Offsets for flag bits in \c sbcs
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      SBVERSION_OFFSET = 29,
      SBBUSYERROR_OFFSET = 22,
      SBBUSY_OFFSET = 21,
      SBREADONADDR_OFFSET = 20,
      SBACCESS_OFFSET = 17,
      SBAUTOINCREMENT_OFFSET = 16,
      SBREADONDATA_OFFSET = 15,
      SBERROR_OFFSET = 12,
      SBASIZE_OFFSET = 5,
      SBACCESS128_OFFSET = 4,
      SBACCESS64_OFFSET = 3,
      SBACCESS32_OFFSET = 2,
      SBACCESS16_OFFSET = 1,
      SBACCESS8_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c sbcs
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      SBVERSION_SIZE = 3,
      SBBUSYERROR_SIZE = 1,
      SBBUSY_SIZE = 1,
      SBREADONADDR_SIZE = 1,
      SBACCESS_SIZE = 3,
      SBAUTOINCREMENT_SIZE = 1,
      SBREADONDATA_SIZE = 1,
      SBERROR_SIZE = 3,
      SBASIZE_SIZE = 7,
      SBACCESS128_SIZE = 1,
      SBACCESS64_SIZE = 1,
      SBACCESS32_SIZE = 1,
      SBACCESS16_SIZE = 1,
      SBACCESS8_SIZE = 1,
    };

    /// \brief The address of the \c sbcs register in the DMI.
    static const uint64_t DMI_ADDR = 0x38;

    /// \brief The reset value of the \c sbcs register in the DMI.
    ///
    /// access size is 32-bit
    static const uint32_t RESET_VALUE = 0x00040000;

    /// \brief Whether pretty printing is enabled for the \c sbcs
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using.
    std::unique_ptr<IDtm> &mDtm;

    /// \brief the value of the Sbcs register.
    uint32_t mSbcsReg;
  };

  /// \brief The class modeling the \c sbdata registers.
  class Sbdata
  {
  public:
    /// \brief The number of \c sbdata registers
    static const std::size_t NUM_REGS = 4;

    // Constructors & destructor
    Sbdata () = delete;
    Sbdata (std::unique_ptr<IDtm> &dtm_);
    ~Sbdata () = default;

    // Delete copy operator
    Sbdata &operator= (const Sbdata &) = delete;

    // API
    void read (const std::size_t n);
    void reset (const std::size_t n);
    void write (const std::size_t n);
    uint32_t sbdata (const std::size_t n) const;
    void sbdata (const std::size_t n, const uint32_t sbdataVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Sbdata> &p);

  private:
    /// \brief The address of the \c sbdata registers in the DMI
    static constexpr uint64_t DMI_ADDR[NUM_REGS] = { 0x3c, 0x3d, 0x3e, 0x3f };

    /// \brief The reset value of the \c sbdata registers in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Sbdata registers
    uint32_t mSbdataReg[NUM_REGS];
  };

  /// \brief All the CSR addresses
  class Csr
  {
  public:
    // Standard user CSRs
    static const uint16_t FFLAGS = 0x1;
    static const uint16_t FRM = 0x2;
    static const uint16_t FCSR = 0x3;
    static const uint16_t CYCLE = 0xc00;
    static const uint16_t INSTRET = 0xc02;
    static const uint16_t HPMCOUNTER3 = 0xc03;
    static const uint16_t HPMCOUNTER4 = 0xc04;
    static const uint16_t HPMCOUNTER5 = 0xc05;
    static const uint16_t HPMCOUNTER6 = 0xc06;
    static const uint16_t HPMCOUNTER7 = 0xc07;
    static const uint16_t HPMCOUNTER8 = 0xc08;
    static const uint16_t HPMCOUNTER9 = 0xc09;
    static const uint16_t HPMCOUNTER10 = 0xc0a;
    static const uint16_t HPMCOUNTER11 = 0xc0b;
    static const uint16_t HPMCOUNTER12 = 0xc0c;
    static const uint16_t HPMCOUNTER13 = 0xc0d;
    static const uint16_t HPMCOUNTER14 = 0xc0e;
    static const uint16_t HPMCOUNTER15 = 0xc0f;
    static const uint16_t HPMCOUNTER16 = 0xc10;
    static const uint16_t HPMCOUNTER17 = 0xc11;
    static const uint16_t HPMCOUNTER18 = 0xc12;
    static const uint16_t HPMCOUNTER19 = 0xc13;
    static const uint16_t HPMCOUNTER20 = 0xc14;
    static const uint16_t HPMCOUNTER21 = 0xc15;
    static const uint16_t HPMCOUNTER22 = 0xc16;
    static const uint16_t HPMCOUNTER23 = 0xc17;
    static const uint16_t HPMCOUNTER24 = 0xc18;
    static const uint16_t HPMCOUNTER25 = 0xc19;
    static const uint16_t HPMCOUNTER26 = 0xc1a;
    static const uint16_t HPMCOUNTER27 = 0xc1b;
    static const uint16_t HPMCOUNTER28 = 0xc1c;
    static const uint16_t HPMCOUNTER29 = 0xc1d;
    static const uint16_t HPMCOUNTER30 = 0xc1e;
    static const uint16_t HPMCOUNTER31 = 0xc1f;
    static const uint16_t CYCLEH = 0xc80;
    static const uint16_t INSTRETH = 0xc82;
    static const uint16_t HPMCOUNTERH3 = 0xc83;
    static const uint16_t HPMCOUNTERH4 = 0xc84;
    static const uint16_t HPMCOUNTERH5 = 0xc85;
    static const uint16_t HPMCOUNTERH6 = 0xc86;
    static const uint16_t HPMCOUNTERH7 = 0xc87;
    static const uint16_t HPMCOUNTERH8 = 0xc88;
    static const uint16_t HPMCOUNTERH9 = 0xc89;
    static const uint16_t HPMCOUNTERH10 = 0xc8a;
    static const uint16_t HPMCOUNTERH11 = 0xc8b;
    static const uint16_t HPMCOUNTERH12 = 0xc8c;
    static const uint16_t HPMCOUNTERH13 = 0xc8d;
    static const uint16_t HPMCOUNTERH14 = 0xc8e;
    static const uint16_t HPMCOUNTERH15 = 0xc8f;
    static const uint16_t HPMCOUNTERH16 = 0xc90;
    static const uint16_t HPMCOUNTERH17 = 0xc91;
    static const uint16_t HPMCOUNTERH18 = 0xc92;
    static const uint16_t HPMCOUNTERH19 = 0xc93;
    static const uint16_t HPMCOUNTERH20 = 0xc94;
    static const uint16_t HPMCOUNTERH21 = 0xc95;
    static const uint16_t HPMCOUNTERH22 = 0xc96;
    static const uint16_t HPMCOUNTERH23 = 0xc97;
    static const uint16_t HPMCOUNTERH24 = 0xc98;
    static const uint16_t HPMCOUNTERH25 = 0xc99;
    static const uint16_t HPMCOUNTERH26 = 0xc9a;
    static const uint16_t HPMCOUNTERH27 = 0xc9b;
    static const uint16_t HPMCOUNTERH28 = 0xc9c;
    static const uint16_t HPMCOUNTERH29 = 0xc9d;
    static const uint16_t HPMCOUNTERH30 = 0xc9e;
    static const uint16_t HPMCOUNTERH31 = 0xc9f;

    // Custom user CSRs
    static const uint16_t LPSTART0 = 0x800;
    static const uint16_t LPEND0 = 0x801;
    static const uint16_t LPCOUNT0 = 0x802;
    static const uint16_t LPSTART1 = 0x804;
    static const uint16_t LPEND1 = 0x805;
    static const uint16_t LPCOUNT1 = 0x806;
    static const uint16_t UHARTID = 0xcc0;
    static const uint16_t PRIVLV = 0xcc1;

    // Standard machine CSRs
    static const uint16_t MSTATUS = 0x300;
    static const uint16_t MISA = 0x301;
    static const uint16_t MIE = 0x304;
    static const uint16_t MTVEC = 0x305;
    static const uint16_t MCOUNTINHIBIT = 0x320;
    static const uint16_t MHPMEVENT3 = 0x323;
    static const uint16_t MHPMEVENT4 = 0x324;
    static const uint16_t MHPMEVENT5 = 0x325;
    static const uint16_t MHPMEVENT6 = 0x326;
    static const uint16_t MHPMEVENT7 = 0x327;
    static const uint16_t MHPMEVENT8 = 0x328;
    static const uint16_t MHPMEVENT9 = 0x329;
    static const uint16_t MHPMEVENT10 = 0x32a;
    static const uint16_t MHPMEVENT11 = 0x32b;
    static const uint16_t MHPMEVENT12 = 0x32c;
    static const uint16_t MHPMEVENT13 = 0x32d;
    static const uint16_t MHPMEVENT14 = 0x32e;
    static const uint16_t MHPMEVENT15 = 0x32f;
    static const uint16_t MHPMEVENT16 = 0x330;
    static const uint16_t MHPMEVENT17 = 0x331;
    static const uint16_t MHPMEVENT18 = 0x332;
    static const uint16_t MHPMEVENT19 = 0x333;
    static const uint16_t MHPMEVENT20 = 0x334;
    static const uint16_t MHPMEVENT21 = 0x335;
    static const uint16_t MHPMEVENT22 = 0x336;
    static const uint16_t MHPMEVENT23 = 0x337;
    static const uint16_t MHPMEVENT24 = 0x338;
    static const uint16_t MHPMEVENT25 = 0x339;
    static const uint16_t MHPMEVENT26 = 0x33a;
    static const uint16_t MHPMEVENT27 = 0x33b;
    static const uint16_t MHPMEVENT28 = 0x33c;
    static const uint16_t MHPMEVENT29 = 0x33d;
    static const uint16_t MHPMEVENT30 = 0x33e;
    static const uint16_t MHPMEVENT31 = 0x33f;
    static const uint16_t MSCRATCH = 0x340;
    static const uint16_t MEPC = 0x341;
    static const uint16_t MCAUSE = 0x342;
    static const uint16_t MTVAL = 0x343;
    static const uint16_t MIP = 0x344;
    static const uint16_t TSELECT = 0x7a0;
    static const uint16_t TDATA1 = 0x7a1;
    static const uint16_t TDATA2 = 0x7a2;
    static const uint16_t TDATA3 = 0x7a3;
    static const uint16_t TINFO = 0x7a4;
    static const uint16_t MCONTEXT = 0x7a8;
    static const uint16_t SCONTEXT = 0x7aa;
    static const uint16_t DCSR = 0x7b0;
    static const uint16_t DPC = 0x7b1;
    static const uint16_t DSCRATCH0 = 0x7b2;
    static const uint16_t DSCRATCH1 = 0x7b3;
    static const uint16_t MCYCLE = 0xb00;
    static const uint16_t MINSTRET = 0xb02;
    static const uint16_t MHPMCOUNTER3 = 0xb03;
    static const uint16_t MHPMCOUNTER4 = 0xb04;
    static const uint16_t MHPMCOUNTER5 = 0xb05;
    static const uint16_t MHPMCOUNTER6 = 0xb06;
    static const uint16_t MHPMCOUNTER7 = 0xb07;
    static const uint16_t MHPMCOUNTER8 = 0xb08;
    static const uint16_t MHPMCOUNTER9 = 0xb09;
    static const uint16_t MHPMCOUNTER10 = 0xb0a;
    static const uint16_t MHPMCOUNTER11 = 0xb0b;
    static const uint16_t MHPMCOUNTER12 = 0xb0c;
    static const uint16_t MHPMCOUNTER13 = 0xb0d;
    static const uint16_t MHPMCOUNTER14 = 0xb0e;
    static const uint16_t MHPMCOUNTER15 = 0xb0f;
    static const uint16_t MHPMCOUNTER16 = 0xb10;
    static const uint16_t MHPMCOUNTER17 = 0xb11;
    static const uint16_t MHPMCOUNTER18 = 0xb12;
    static const uint16_t MHPMCOUNTER19 = 0xb13;
    static const uint16_t MHPMCOUNTER20 = 0xb14;
    static const uint16_t MHPMCOUNTER21 = 0xb15;
    static const uint16_t MHPMCOUNTER22 = 0xb16;
    static const uint16_t MHPMCOUNTER23 = 0xb17;
    static const uint16_t MHPMCOUNTER24 = 0xb18;
    static const uint16_t MHPMCOUNTER25 = 0xb19;
    static const uint16_t MHPMCOUNTER26 = 0xb1a;
    static const uint16_t MHPMCOUNTER27 = 0xb1b;
    static const uint16_t MHPMCOUNTER28 = 0xb1c;
    static const uint16_t MHPMCOUNTER29 = 0xb1d;
    static const uint16_t MHPMCOUNTER30 = 0xb1e;
    static const uint16_t MHPMCOUNTER31 = 0xb1f;
    static const uint16_t MCYCLEH = 0xb80;
    static const uint16_t MINSTRETH = 0xb82;
    static const uint16_t MHPMCOUNTERH3 = 0xb83;
    static const uint16_t MHPMCOUNTERH4 = 0xb84;
    static const uint16_t MHPMCOUNTERH5 = 0xb85;
    static const uint16_t MHPMCOUNTERH6 = 0xb86;
    static const uint16_t MHPMCOUNTERH7 = 0xb87;
    static const uint16_t MHPMCOUNTERH8 = 0xb88;
    static const uint16_t MHPMCOUNTERH9 = 0xb89;
    static const uint16_t MHPMCOUNTERH10 = 0xb8a;
    static const uint16_t MHPMCOUNTERH11 = 0xb8b;
    static const uint16_t MHPMCOUNTERH12 = 0xb8c;
    static const uint16_t MHPMCOUNTERH13 = 0xb8d;
    static const uint16_t MHPMCOUNTERH14 = 0xb8e;
    static const uint16_t MHPMCOUNTERH15 = 0xb8f;
    static const uint16_t MHPMCOUNTERH16 = 0xb90;
    static const uint16_t MHPMCOUNTERH17 = 0xb91;
    static const uint16_t MHPMCOUNTERH18 = 0xb92;
    static const uint16_t MHPMCOUNTERH19 = 0xb93;
    static const uint16_t MHPMCOUNTERH20 = 0xb94;
    static const uint16_t MHPMCOUNTERH21 = 0xb95;
    static const uint16_t MHPMCOUNTERH22 = 0xb96;
    static const uint16_t MHPMCOUNTERH23 = 0xb97;
    static const uint16_t MHPMCOUNTERH24 = 0xb98;
    static const uint16_t MHPMCOUNTERH25 = 0xb99;
    static const uint16_t MHPMCOUNTERH26 = 0xb9a;
    static const uint16_t MHPMCOUNTERH27 = 0xb9b;
    static const uint16_t MHPMCOUNTERH28 = 0xb9c;
    static const uint16_t MHPMCOUNTERH29 = 0xb9d;
    static const uint16_t MHPMCOUNTERH30 = 0xb9e;
    static const uint16_t MHPMCOUNTERH31 = 0xb9f;
    static const uint16_t MVENDORID = 0xf11;
    static const uint16_t MARCHID = 0xf12;
    static const uint16_t MIMPID = 0xf13;
    static const uint16_t MHARTID = 0xf14;
  };

  /// \brief An enumeration of the groups of CSRs
  enum CsrType
  {
    NONE, ///< Used for non-existent CSRs.
    ANY,  ///< All configurations
    FP,   ///< Only if FPU is present
    HWLP, ///< Only if hardware loop is present
  };

  // Constructor and destructor
  Dmi (std::unique_ptr<IDtm> dtm);
  Dmi () = delete;
  ~Dmi () = default;

  // Delete the copy assignment operator
  Dmi &operator= (const Dmi &) = delete;

  // Hart control API
  void selectHart (uint32_t h);
  uint32_t hartsellen ();
  void haltHart (uint32_t h);

  // Accessors for CSR fields
  const char *csrName (const uint16_t csrAddr) const;
  bool csrReadOnly (const uint16_t csrAddr) const;
  CsrType csrType (const uint16_t csrAddr) const;

  // Register access API
  Abstractcs::CmderrVal readCsr (uint16_t addr, uint32_t &res);
  Abstractcs::CmderrVal writeCsr (uint16_t addr, uint32_t val);
  Abstractcs::CmderrVal readGpr (std::size_t regNum, uint32_t &res);
  Abstractcs::CmderrVal writeGpr (std::size_t regNum, uint32_t val);
  Abstractcs::CmderrVal readFpr (std::size_t regNum, uint32_t &res);
  Abstractcs::CmderrVal writeFpr (std::size_t regNum, uint32_t val);

  // Memory access API
  Sbcs::SberrorVal readMem (uint64_t addr, std::size_t nBytes,
                            std::unique_ptr<uint8_t[]> &buf);
  Sbcs::SberrorVal writeMem (uint64_t addr, std::size_t nBytes,
                             std::unique_ptr<uint8_t[]> &buf);

  // API for the underlying DTM
  void dtmReset ();
  uint64_t simTimeNs () const;

  // Accessors for registers
  std::unique_ptr<Data> &data ();
  std::unique_ptr<Dmcontrol> &dmcontrol ();
  std::unique_ptr<Dmstatus> &dmstatus ();
  std::unique_ptr<Hartinfo> &hartinfo ();
  std::unique_ptr<Haltsum> &haltsum ();
  std::unique_ptr<Hawindowsel> &hawindowsel ();
  std::unique_ptr<Hawindow> &hawindow ();
  std::unique_ptr<Abstractcs> &abstractcs ();
  std::unique_ptr<Command> &command ();
  std::unique_ptr<Abstractauto> &abstractauto ();
  std::unique_ptr<Confstrptr> &confstrptr ();
  std::unique_ptr<Nextdm> &nextdm ();
  std::unique_ptr<Progbuf> &progbuf ();
  std::unique_ptr<Authdata> &authdata ();
  std::unique_ptr<Sbaddress> &sbaddress ();
  std::unique_ptr<Sbcs> &sbcs ();
  std::unique_ptr<Sbdata> &sbdata ();

private:
  /// \brief A structure representing a CSR
  ///
  /// CSRs will be placed into a std::map with this data keyed by the CSR
  /// address.
  struct CsrInfo
  {
    const char *name;    ///< The printable name of the CSR
    const bool readOnly; ///< True if the CSR is read only
    const CsrType type;  ///< Which CSR group
  };

  /// \brief Base address of the GPRs when reading/writing
  static const uint16_t GPR_BASE = 0x1000;

  /// \brief Base address of the FPRs when reading/writing
  static const uint16_t FPR_BASE = 0x1020;

  /// \brief A map of CSR address to name, readability and instruction gorup
  std::map<const uint16_t, CsrInfo> mCsrMap{
    // Standard user CSRs
    { Csr::FFLAGS, { "fflags", false, FP } },
    { Csr::FRM, { "frm", false, FP } },
    { Csr::FCSR, { "fcsr", false, FP } },
    { Csr::CYCLE, { "cycle", true, ANY } },
    { Csr::INSTRET, { "instret", true, ANY } },
    { Csr::HPMCOUNTER3, { "hpmcounter3", true, ANY } },
    { Csr::HPMCOUNTER4, { "hpmcounter4", true, ANY } },
    { Csr::HPMCOUNTER5, { "hpmcounter5", true, ANY } },
    { Csr::HPMCOUNTER6, { "hpmcounter6", true, ANY } },
    { Csr::HPMCOUNTER7, { "hpmcounter7", true, ANY } },
    { Csr::HPMCOUNTER8, { "hpmcounter8", true, ANY } },
    { Csr::HPMCOUNTER9, { "hpmcounter9", true, ANY } },
    { Csr::HPMCOUNTER10, { "hpmcounter10", true, ANY } },
    { Csr::HPMCOUNTER11, { "hpmcounter11", true, ANY } },
    { Csr::HPMCOUNTER12, { "hpmcounter12", true, ANY } },
    { Csr::HPMCOUNTER13, { "hpmcounter13", true, ANY } },
    { Csr::HPMCOUNTER14, { "hpmcounter14", true, ANY } },
    { Csr::HPMCOUNTER15, { "hpmcounter15", true, ANY } },
    { Csr::HPMCOUNTER16, { "hpmcounter16", true, ANY } },
    { Csr::HPMCOUNTER17, { "hpmcounter17", true, ANY } },
    { Csr::HPMCOUNTER18, { "hpmcounter18", true, ANY } },
    { Csr::HPMCOUNTER19, { "hpmcounter19", true, ANY } },
    { Csr::HPMCOUNTER20, { "hpmcounter20", true, ANY } },
    { Csr::HPMCOUNTER21, { "hpmcounter21", true, ANY } },
    { Csr::HPMCOUNTER22, { "hpmcounter22", true, ANY } },
    { Csr::HPMCOUNTER23, { "hpmcounter23", true, ANY } },
    { Csr::HPMCOUNTER24, { "hpmcounter24", true, ANY } },
    { Csr::HPMCOUNTER25, { "hpmcounter25", true, ANY } },
    { Csr::HPMCOUNTER26, { "hpmcounter26", true, ANY } },
    { Csr::HPMCOUNTER27, { "hpmcounter27", true, ANY } },
    { Csr::HPMCOUNTER28, { "hpmcounter28", true, ANY } },
    { Csr::HPMCOUNTER29, { "hpmcounter29", true, ANY } },
    { Csr::HPMCOUNTER30, { "hpmcounter30", true, ANY } },
    { Csr::HPMCOUNTER31, { "hpmcounter31", true, ANY } },
    { Csr::CYCLEH, { "cycleh", true, ANY } },
    { Csr::INSTRETH, { "instreth", true, ANY } },
    { Csr::HPMCOUNTERH3, { "hpmcounterh3", true, ANY } },
    { Csr::HPMCOUNTERH4, { "hpmcounterh4", true, ANY } },
    { Csr::HPMCOUNTERH5, { "hpmcounterh5", true, ANY } },
    { Csr::HPMCOUNTERH6, { "hpmcounterh6", true, ANY } },
    { Csr::HPMCOUNTERH7, { "hpmcounterh7", true, ANY } },
    { Csr::HPMCOUNTERH8, { "hpmcounterh8", true, ANY } },
    { Csr::HPMCOUNTERH9, { "hpmcounterh9", true, ANY } },
    { Csr::HPMCOUNTERH10, { "hpmcounterh10", true, ANY } },
    { Csr::HPMCOUNTERH11, { "hpmcounterh11", true, ANY } },
    { Csr::HPMCOUNTERH12, { "hpmcounterh12", true, ANY } },
    { Csr::HPMCOUNTERH13, { "hpmcounterh13", true, ANY } },
    { Csr::HPMCOUNTERH14, { "hpmcounterh14", true, ANY } },
    { Csr::HPMCOUNTERH15, { "hpmcounterh15", true, ANY } },
    { Csr::HPMCOUNTERH16, { "hpmcounterh16", true, ANY } },
    { Csr::HPMCOUNTERH17, { "hpmcounterh17", true, ANY } },
    { Csr::HPMCOUNTERH18, { "hpmcounterh18", true, ANY } },
    { Csr::HPMCOUNTERH19, { "hpmcounterh19", true, ANY } },
    { Csr::HPMCOUNTERH20, { "hpmcounterh20", true, ANY } },
    { Csr::HPMCOUNTERH21, { "hpmcounterh21", true, ANY } },
    { Csr::HPMCOUNTERH22, { "hpmcounterh22", true, ANY } },
    { Csr::HPMCOUNTERH23, { "hpmcounterh23", true, ANY } },
    { Csr::HPMCOUNTERH24, { "hpmcounterh24", true, ANY } },
    { Csr::HPMCOUNTERH25, { "hpmcounterh25", true, ANY } },
    { Csr::HPMCOUNTERH26, { "hpmcounterh26", true, ANY } },
    { Csr::HPMCOUNTERH27, { "hpmcounterh27", true, ANY } },
    { Csr::HPMCOUNTERH28, { "hpmcounterh28", true, ANY } },
    { Csr::HPMCOUNTERH29, { "hpmcounterh29", true, ANY } },
    { Csr::HPMCOUNTERH30, { "hpmcounterh30", true, ANY } },
    { Csr::HPMCOUNTERH31, { "hpmcounterh31", true, ANY } },
    // Custom user CSRs
    { Csr::LPSTART0, { "lpstart0", false, HWLP } },
    { Csr::LPEND0, { "lpend0", false, HWLP } },
    { Csr::LPCOUNT0, { "lpcount0", false, HWLP } },
    { Csr::LPSTART1, { "lpstart1", false, HWLP } },
    { Csr::LPEND1, { "lpend1", false, HWLP } },
    { Csr::LPCOUNT1, { "lpcount1", false, HWLP } },
    { Csr::UHARTID, { "uhartid", true, ANY } },
    { Csr::PRIVLV, { "privlv", true, ANY } },
    // Standard machine CSRs
    { Csr::MSTATUS, { "mstatus", false, ANY } },
    { Csr::MISA, { "misa", false, ANY } },
    { Csr::MIE, { "mie", false, ANY } },
    { Csr::MTVEC, { "mtvec", false, ANY } },
    { Csr::MCOUNTINHIBIT, { "mcountinhibit", false, ANY } },
    { Csr::MHPMEVENT3, { "mhpmevent3", false, ANY } },
    { Csr::MHPMEVENT4, { "mhpmevent4", false, ANY } },
    { Csr::MHPMEVENT5, { "mhpmevent5", false, ANY } },
    { Csr::MHPMEVENT6, { "mhpmevent6", false, ANY } },
    { Csr::MHPMEVENT7, { "mhpmevent7", false, ANY } },
    { Csr::MHPMEVENT8, { "mhpmevent8", false, ANY } },
    { Csr::MHPMEVENT9, { "mhpmevent9", false, ANY } },
    { Csr::MHPMEVENT10, { "mhpmevent10", false, ANY } },
    { Csr::MHPMEVENT11, { "mhpmevent11", false, ANY } },
    { Csr::MHPMEVENT12, { "mhpmevent12", false, ANY } },
    { Csr::MHPMEVENT13, { "mhpmevent13", false, ANY } },
    { Csr::MHPMEVENT14, { "mhpmevent14", false, ANY } },
    { Csr::MHPMEVENT15, { "mhpmevent15", false, ANY } },
    { Csr::MHPMEVENT16, { "mhpmevent16", false, ANY } },
    { Csr::MHPMEVENT17, { "mhpmevent17", false, ANY } },
    { Csr::MHPMEVENT18, { "mhpmevent18", false, ANY } },
    { Csr::MHPMEVENT19, { "mhpmevent19", false, ANY } },
    { Csr::MHPMEVENT20, { "mhpmevent20", false, ANY } },
    { Csr::MHPMEVENT21, { "mhpmevent21", false, ANY } },
    { Csr::MHPMEVENT22, { "mhpmevent22", false, ANY } },
    { Csr::MHPMEVENT23, { "mhpmevent23", false, ANY } },
    { Csr::MHPMEVENT24, { "mhpmevent24", false, ANY } },
    { Csr::MHPMEVENT25, { "mhpmevent25", false, ANY } },
    { Csr::MHPMEVENT26, { "mhpmevent26", false, ANY } },
    { Csr::MHPMEVENT27, { "mhpmevent27", false, ANY } },
    { Csr::MHPMEVENT28, { "mhpmevent28", false, ANY } },
    { Csr::MHPMEVENT29, { "mhpmevent29", false, ANY } },
    { Csr::MHPMEVENT30, { "mhpmevent30", false, ANY } },
    { Csr::MHPMEVENT31, { "mhpmevent31", false, ANY } },
    { Csr::MSCRATCH, { "mscratch", false, ANY } },
    { Csr::MEPC, { "mepc", false, ANY } },
    { Csr::MCAUSE, { "mcause", false, ANY } },
    { Csr::MTVAL, { "mtval", false, ANY } },
    { Csr::MIP, { "mip", false, ANY } },
    { Csr::TSELECT, { "tselect", false, ANY } },
    { Csr::TDATA1, { "tdata1", false, ANY } },
    { Csr::TDATA2, { "tdata2", false, ANY } },
    { Csr::TDATA3, { "tdata3", false, ANY } },
    { Csr::TINFO, { "tinfo", true, ANY } },
    { Csr::MCONTEXT, { "mcontext", false, ANY } },
    { Csr::SCONTEXT, { "scontext", false, ANY } },
    { Csr::DCSR, { "dcsr", false, ANY } },
    { Csr::DPC, { "dpc", false, ANY } },
    { Csr::DSCRATCH0, { "dscratch0", false, ANY } },
    { Csr::DSCRATCH1, { "dscratch1", false, ANY } },
    { Csr::MCYCLE, { "mcycle", false, ANY } },
    { Csr::MINSTRET, { "minstret", false, ANY } },
    { Csr::MHPMCOUNTER3, { "mhpmcounter3", false, ANY } },
    { Csr::MHPMCOUNTER4, { "mhpmcounter4", false, ANY } },
    { Csr::MHPMCOUNTER5, { "mhpmcounter5", false, ANY } },
    { Csr::MHPMCOUNTER6, { "mhpmcounter6", false, ANY } },
    { Csr::MHPMCOUNTER7, { "mhpmcounter7", false, ANY } },
    { Csr::MHPMCOUNTER8, { "mhpmcounter8", false, ANY } },
    { Csr::MHPMCOUNTER9, { "mhpmcounter9", false, ANY } },
    { Csr::MHPMCOUNTER10, { "mhpmcounter10", false, ANY } },
    { Csr::MHPMCOUNTER11, { "mhpmcounter11", false, ANY } },
    { Csr::MHPMCOUNTER12, { "mhpmcounter12", false, ANY } },
    { Csr::MHPMCOUNTER13, { "mhpmcounter13", false, ANY } },
    { Csr::MHPMCOUNTER14, { "mhpmcounter14", false, ANY } },
    { Csr::MHPMCOUNTER15, { "mhpmcounter15", false, ANY } },
    { Csr::MHPMCOUNTER16, { "mhpmcounter16", false, ANY } },
    { Csr::MHPMCOUNTER17, { "mhpmcounter17", false, ANY } },
    { Csr::MHPMCOUNTER18, { "mhpmcounter18", false, ANY } },
    { Csr::MHPMCOUNTER19, { "mhpmcounter19", false, ANY } },
    { Csr::MHPMCOUNTER20, { "mhpmcounter20", false, ANY } },
    { Csr::MHPMCOUNTER21, { "mhpmcounter21", false, ANY } },
    { Csr::MHPMCOUNTER22, { "mhpmcounter22", false, ANY } },
    { Csr::MHPMCOUNTER23, { "mhpmcounter23", false, ANY } },
    { Csr::MHPMCOUNTER24, { "mhpmcounter24", false, ANY } },
    { Csr::MHPMCOUNTER25, { "mhpmcounter25", false, ANY } },
    { Csr::MHPMCOUNTER26, { "mhpmcounter26", false, ANY } },
    { Csr::MHPMCOUNTER27, { "mhpmcounter27", false, ANY } },
    { Csr::MHPMCOUNTER28, { "mhpmcounter28", false, ANY } },
    { Csr::MHPMCOUNTER29, { "mhpmcounter29", false, ANY } },
    { Csr::MHPMCOUNTER30, { "mhpmcounter30", false, ANY } },
    { Csr::MHPMCOUNTER31, { "mhpmcounter31", false, ANY } },
    { Csr::MCYCLEH, { "mcycleh", false, ANY } },
    { Csr::MINSTRETH, { "minstreth", false, ANY } },
    { Csr::MHPMCOUNTERH3, { "mhpmcounterh3", false, ANY } },
    { Csr::MHPMCOUNTERH4, { "mhpmcounterh4", false, ANY } },
    { Csr::MHPMCOUNTERH5, { "mhpmcounterh5", false, ANY } },
    { Csr::MHPMCOUNTERH6, { "mhpmcounterh6", false, ANY } },
    { Csr::MHPMCOUNTERH7, { "mhpmcounterh7", false, ANY } },
    { Csr::MHPMCOUNTERH8, { "mhpmcounterh8", false, ANY } },
    { Csr::MHPMCOUNTERH9, { "mhpmcounterh9", false, ANY } },
    { Csr::MHPMCOUNTERH10, { "mhpmcounterh10", false, ANY } },
    { Csr::MHPMCOUNTERH11, { "mhpmcounterh11", false, ANY } },
    { Csr::MHPMCOUNTERH12, { "mhpmcounterh12", false, ANY } },
    { Csr::MHPMCOUNTERH13, { "mhpmcounterh13", false, ANY } },
    { Csr::MHPMCOUNTERH14, { "mhpmcounterh14", false, ANY } },
    { Csr::MHPMCOUNTERH15, { "mhpmcounterh15", false, ANY } },
    { Csr::MHPMCOUNTERH16, { "mhpmcounterh16", false, ANY } },
    { Csr::MHPMCOUNTERH17, { "mhpmcounterh17", false, ANY } },
    { Csr::MHPMCOUNTERH18, { "mhpmcounterh18", false, ANY } },
    { Csr::MHPMCOUNTERH19, { "mhpmcounterh19", false, ANY } },
    { Csr::MHPMCOUNTERH20, { "mhpmcounterh20", false, ANY } },
    { Csr::MHPMCOUNTERH21, { "mhpmcounterh21", false, ANY } },
    { Csr::MHPMCOUNTERH22, { "mhpmcounterh22", false, ANY } },
    { Csr::MHPMCOUNTERH23, { "mhpmcounterh23", false, ANY } },
    { Csr::MHPMCOUNTERH24, { "mhpmcounterh24", false, ANY } },
    { Csr::MHPMCOUNTERH25, { "mhpmcounterh25", false, ANY } },
    { Csr::MHPMCOUNTERH26, { "mhpmcounterh26", false, ANY } },
    { Csr::MHPMCOUNTERH27, { "mhpmcounterh27", false, ANY } },
    { Csr::MHPMCOUNTERH28, { "mhpmcounterh28", false, ANY } },
    { Csr::MHPMCOUNTERH29, { "mhpmcounterh29", false, ANY } },
    { Csr::MHPMCOUNTERH30, { "mhpmcounterh30", false, ANY } },
    { Csr::MHPMCOUNTERH31, { "mhpmcounterh31", false, ANY } },
    { Csr::MVENDORID, { "mvendorid", true, ANY } },
    { Csr::MARCHID, { "marchid", true, ANY } },
    { Csr::MIMPID, { "mimpid", true, ANY } },
    { Csr::MHARTID, { "mhartid", true, ANY } }
  };

  /// \brief The Debug Transport Module we use.
  std::unique_ptr<IDtm> mDtm;

  /// \brief The \c data register set.
  std::unique_ptr<Data> mData;

  /// \brief The \c dmcontrol register.
  std::unique_ptr<Dmcontrol> mDmcontrol;

  /// \brief The \c dmstatus register.
  std::unique_ptr<Dmstatus> mDmstatus;

  /// \brief The \c hartinfo register.
  std::unique_ptr<Hartinfo> mHartinfo;

  /// \brief The \c haltsum register set.
  std::unique_ptr<Haltsum> mHaltsum;

  /// \brief The \c hawindowsel register.
  std::unique_ptr<Hawindowsel> mHawindowsel;

  /// \brief The \c hawindow register.
  std::unique_ptr<Hawindow> mHawindow;

  /// \brief The \c abstractcs register.
  std::unique_ptr<Abstractcs> mAbstractcs;

  /// \brief The \c command register.
  std::unique_ptr<Command> mCommand;

  /// \brief The \c abstractauto register.
  std::unique_ptr<Abstractauto> mAbstractauto;

  /// \brief The \c confstrptr register set.
  std::unique_ptr<Confstrptr> mConfstrptr;

  /// \brief The \c nextdm register.
  std::unique_ptr<Nextdm> mNextdm;

  /// \brief The \c progbuf register set.
  std::unique_ptr<Progbuf> mProgbuf;

  /// \brief The \c authdata register.
  std::unique_ptr<Authdata> mAuthdata;

  /// \brief The \c sbaddress register set.
  std::unique_ptr<Sbaddress> mSbaddress;

  /// \brief The \c sbcs register.
  std::unique_ptr<Sbcs> mSbcs;

  /// \brief The \c sbdata register set.
  std::unique_ptr<Sbdata> mSbdata;
};

// Stream output operators for DMI register classes
std::ostream &operator<< (std::ostream &s, const std::unique_ptr<Dmi::Data> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Dmcontrol> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Dmstatus> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Hartinfo> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Haltsum> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Hawindowsel> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Hawindow> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Abstractcs> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Command> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Abstractauto> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Confstrptr> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Nextdm> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Progbuf> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Authdata> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Sbaddress> &p);
std::ostream &operator<< (std::ostream &s, const std::unique_ptr<Dmi::Sbcs> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Sbdata> &p);

#endif // DMI_H
