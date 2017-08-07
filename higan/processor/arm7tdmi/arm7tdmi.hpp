//ARMv3 (ARM60)
//ARMv4 (ARM7TDMI)

#pragma once

namespace Processor {

struct ARM7TDMI {
  enum : uint {
    Nonsequential = 1 << 0,  //N cycle
    Sequential    = 1 << 1,  //S cycle
    Prefetch      = 1 << 2,  //instruction fetch
    Byte          = 1 << 3,  // 8-bit access
    Half          = 1 << 4,  //16-bit access
    Word          = 1 << 5,  //32-bit access
    Load          = 1 << 6,  //load operation
    Store         = 1 << 7,  //store operation
    Signed        = 1 << 8,  //sign-extend
  };

  virtual auto step(uint clocks) -> void = 0;
  virtual auto sleep() -> void = 0;
  virtual auto get(uint mode, uint32 address) -> uint32 = 0;
  virtual auto set(uint mode, uint32 address, uint32 word) -> void = 0;

  //arm7tdmi.cpp
  ARM7TDMI();
  auto power() -> void;

  //registers.cpp
  struct GPR;
  struct PSR;
  inline auto r(uint4) -> GPR&;
  inline auto u(uint4) -> GPR&;
  inline auto cpsr() -> PSR&;
  inline auto spsr() -> PSR&;
  inline auto privileged() const -> bool;
  inline auto exception() const -> bool;

  //memory.cpp
  auto idle() -> void;
  auto read(uint mode, uint32 address) -> uint32;
  auto load(uint mode, uint32 address) -> uint32;
  auto write(uint mode, uint32 address, uint32 word) -> void;
  auto store(uint mode, uint32 address, uint32 word) -> void;

  //algorithms.cpp
  auto ADD(uint32, uint32, bool) -> uint32;
  auto ASR(uint32, uint8) -> uint32;
  auto BIT(uint32) -> uint32;
  auto LSL(uint32, uint8) -> uint32;
  auto LSR(uint32, uint8) -> uint32;
  auto MUL(uint32, uint32, uint32) -> uint32;
  auto ROR(uint32, uint8) -> uint32;
  auto RRX(uint32) -> uint32;
  auto SUB(uint32, uint32, bool) -> uint32;
  auto TST(uint4) -> bool;

  //instruction.cpp
  auto fetch() -> void;
  auto instruction() -> void;
  auto interrupt(uint mode, uint32 address) -> void;
  auto armInitialize() -> void;
  auto thumbInitialize() -> void;

  //instructions-arm.cpp
  auto armALU(uint4 mode, uint4 target, uint4 source, uint32 data) -> void;
  auto armMoveToStatus(uint4 field, uint1 source, uint32 data) -> void;

  auto armInstructionBranchExchangeRegister(uint4) -> void;
  auto armInstructionLoadImmediate(uint8, uint1, uint4, uint4, uint1, uint1, uint1) -> void;
  auto armInstructionLoadRegister(uint4, uint1, uint4, uint4, uint1, uint1, uint1) -> void;
  auto armInstructionMemorySwap(uint4, uint4, uint4, uint1) -> void;
  auto armInstructionMoveHalfImmediate(uint8, uint4, uint4, uint1, uint1, uint1, uint1) -> void;
  auto armInstructionMoveHalfRegister(uint4, uint4, uint4, uint1, uint1, uint1, uint1) -> void;
  auto armInstructionMoveToRegisterFromStatus(uint4, uint1) -> void;
  auto armInstructionMoveToStatusFromImmediate(uint8, uint4, uint4, uint1) -> void;
  auto armInstructionMoveToStatusFromRegister(uint4, uint4, uint1) -> void;
  auto armInstructionMultiply(uint4, uint4, uint4, uint4, uint1, uint1) -> void;
  auto armInstructionMultiplyLong(uint4, uint4, uint4, uint4, uint1, uint1, uint1) -> void;

  //instructions-thumb.cpp
  auto thumbALU(uint4 mode, uint4 target, uint4 source) -> void;

  auto thumbInstructionAdjustRegister(uint3, uint3, uint3, uint1) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  //disassembler.cpp
  auto disassemble(uint32 pc) -> string;

  struct GPR {
    inline operator uint32_t() const {
      return data;
    }

    inline auto operator=(uint32 value) -> GPR& {
      data = value;
      if(modify) modify();
      return *this;
    }

    uint32 data;
    function<auto () -> void> modify;
  };

  struct PSR {
    enum : uint {
      USR26 = 0x00,  //26-bit user
      FIQ26 = 0x01,  //26-bit fast interrupt
      IRQ26 = 0x02,  //26-bit interrupt
      SVC26 = 0x03,  //26-bit service

      USR = 0x10,  //user
      FIQ = 0x11,  //fast interrupt
      IRQ = 0x12,  //interrupt
      SVC = 0x13,  //service
      ABT = 0x17,  //abort
      UND = 0x1b,  //undefined
      SYS = 0x1f,  //system
    };

    inline operator uint32_t() const {
      return m << 0 | t << 5 | f << 6 | i << 7 | v << 28 | c << 29 | z << 30 | n << 31;
    }

    inline auto operator=(uint32 data) -> PSR& {
      m = data.bits(0,4);
      t = data.bit(5);
      f = data.bit(6);
      i = data.bit(7);
      v = data.bit(28);
      c = data.bit(29);
      z = data.bit(30);
      n = data.bit(31);
      return *this;
    }

    //serialization.cpp
    auto serialize(serializer&) -> void;

    uint5 m;  //mode
    uint1 t;  //thumb
    uint1 f;  //fiq
    uint1 i;  //irq
    uint1 v;  //overflow
    uint1 c;  //carry
    uint1 z;  //zero
    uint1 n;  //negative
  };

  struct Processor {
    //serialization.cpp
    auto serialize(serializer&) -> void;

    GPR r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15;
    PSR cpsr;

    struct FIQ {
      GPR r8, r9, r10, r11, r12, r13, r14;
      PSR spsr;
    } fiq;

    struct IRQ {
      GPR r13, r14;
      PSR spsr;
    } irq;

    struct SVC {
      GPR r13, r14;
      PSR spsr;
    } svc;

    struct ABT {
      GPR r13, r14;
      PSR spsr;
    } abt;

    struct UND {
      GPR r13, r14;
      PSR spsr;
    } und;
  } processor;

  struct Pipeline {
    //serialization.cpp
    auto serialize(serializer&) -> void;

    struct Instruction {
      uint32 address;
      uint32 instruction;
    };

    uint1 reload = 1;
    uint1 nonsequential = 1;
    Instruction fetch;
    Instruction decode;
    Instruction execute;
  } pipeline;

  uint32 opcode;
  boolean carry;
  boolean irq;

  function<void (uint32 opcode)> armInstruction[4096];
  function<void ()> thumbInstruction[65536];

  function<string (uint32 opcode)> armDisassemble[4096];
  function<string ()> thumbDisassemble[65536];

  //disassembler.cpp
  auto armDisassembleBranchExchangeRegister(uint4) -> string;
  auto armDisassembleLoadImmediate(uint8, uint1, uint4, uint4, uint1, uint1, uint1) -> string;
  auto armDisassembleLoadRegister(uint4, uint1, uint4, uint4, uint1, uint1, uint1) -> string;
  auto armDisassembleMemorySwap(uint4, uint4, uint4, uint1) -> string;
  auto armDisassembleMoveHalfImmediate(uint8, uint4, uint4, uint1, uint1, uint1, uint1) -> string;
  auto armDisassembleMoveHalfRegister(uint4, uint4, uint4, uint1, uint1, uint1, uint1) -> string;
  auto armDisassembleMoveToRegisterFromStatus(uint4, uint1) -> string;
  auto armDisassembleMoveToStatusFromImmediate(uint8, uint4, uint4, uint1) -> string;
  auto armDisassembleMoveToStatusFromRegister(uint4, uint4, uint1) -> string;
  auto armDisassembleMultiply(uint4, uint4, uint4, uint4, uint1, uint1) -> string;
  auto armDisassembleMultiplyLong(uint4, uint4, uint4, uint4, uint1, uint1, uint1) -> string;

  auto thumbDisassembleAdjustRegister(uint3, uint3, uint3, uint1) -> string;
};

}
