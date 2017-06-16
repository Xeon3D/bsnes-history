#define op(id, name, ...) case id: return instruction##name(__VA_ARGS__);
#define fp(name) &HuC6280::algorithm##name

auto HuC6280::interrupt(uint16 vector) -> void {
  io();
  io();
  push(PC >> 8);
  push(PC >> 0);
  push(P);
  D = 0;
  I = 1;
  PC.byte(0) = load16(vector + 0);
L PC.byte(1) = load16(vector + 1);
}

auto HuC6280::instruction() -> void {
  auto code = opcode();

  if(T) {
    T = 0;
    switch(code) {
    op(0x09, Memory, fp(ORA))
    op(0x29, Memory, fp(AND))
    op(0x49, Memory, fp(EOR))
    op(0x69, Memory, fp(ADC))
    }
  }

  #define U
  switch(code) {
  op(0x00, BRK)
  op(0x01, IndirectRead, fp(ORA), A, X)
  op(0x02, Swap, X, Y)
  op(0x03, ST, 0)
  op(0x04, ZeroPageModify, fp(TSB))
  op(0x05, ZeroPageRead, fp(ORA), A)
  op(0x06, ZeroPageModify, fp(ASL))
  op(0x07, RMB, 0)
  op(0x08, Push, P)
  op(0x09, Immediate, fp(ORA), A)
  op(0x0a, Implied, fp(ASL), A)
U op(0x0b, NOP)
  op(0x0c, AbsoluteModify, fp(TSB))
  op(0x0d, AbsoluteRead, fp(ORA), A)
  op(0x0e, AbsoluteModify, fp(ASL))
  op(0x0f, BBR, 0)
  op(0x10, Branch, N == 0)
  op(0x11, IndirectYRead, fp(ORA), A)
  op(0x12, IndirectRead, fp(ORA), A)
  op(0x13, ST, 1)
  op(0x14, ZeroPageModify, fp(TRB))
  op(0x15, ZeroPageRead, fp(ORA), A, X)
  op(0x16, ZeroPageModify, fp(ASL), X)
  op(0x17, RMB, 1)
  op(0x18, Clear, C)
  op(0x19, AbsoluteRead, fp(ORA), A, Y)
  op(0x1a, Implied, fp(INC), A)
U op(0x1b, NOP)
  op(0x1c, AbsoluteModify, fp(TRB))
  op(0x1d, AbsoluteRead, fp(ORA), A, X)
  op(0x1e, AbsoluteModify, fp(ASL), X)
  op(0x1f, BBR, 1)
  op(0x20, JSR)
  op(0x21, IndirectRead, fp(AND), A, X)
  op(0x22, Swap, A, X)
  op(0x23, ST, 2)
  op(0x24, ZeroPageRead, fp(BIT), A)
  op(0x25, ZeroPageRead, fp(AND), A)
  op(0x26, ZeroPageModify, fp(ROL))
  op(0x27, RMB, 2)
  op(0x28, PLP)
  op(0x29, Immediate, fp(AND), A)
  op(0x2a, Implied, fp(ROL), A)
U op(0x2b, NOP)
  op(0x2c, AbsoluteRead, fp(BIT), A)
  op(0x2d, AbsoluteRead, fp(AND), A)
  op(0x2e, AbsoluteModify, fp(ROL))
  op(0x2f, BBR, 2)
  op(0x30, Branch, N == 1)
  op(0x31, IndirectYRead, fp(AND), A)
  op(0x32, IndirectRead, fp(AND), A)
U op(0x33, NOP)
  op(0x34, ZeroPageRead, fp(BIT), A, X)
  op(0x35, ZeroPageRead, fp(AND), A, X)
  op(0x36, ZeroPageModify, fp(ROL), X)
  op(0x37, RMB, 3)
  op(0x38, Set, C)
  op(0x39, AbsoluteRead, fp(AND), A, Y)
  op(0x3a, Implied, fp(DEC), A)
U op(0x3b, NOP)
  op(0x3c, AbsoluteRead, fp(BIT), A, X)
  op(0x3d, AbsoluteRead, fp(AND), A, X)
  op(0x3e, AbsoluteModify, fp(ROL), X)
  op(0x3f, BBR, 3)
  op(0x40, RTI)
  op(0x41, IndirectRead, fp(EOR), A, X)
  op(0x42, Swap, A, Y)
  op(0x43, TMA)
  op(0x44, BSR)
  op(0x45, ZeroPageRead, fp(EOR), A)
  op(0x46, ZeroPageModify, fp(LSR))
  op(0x47, RMB, 4)
  op(0x48, Push, A)
  op(0x49, Immediate, fp(EOR), A)
  op(0x4a, Implied, fp(LSR), A)
U op(0x4b, NOP)
  op(0x4c, JMPAbsolute)
  op(0x4d, AbsoluteRead, fp(EOR), A)
  op(0x4e, AbsoluteModify, fp(LSR))
  op(0x4f, BBR, 4)
  op(0x50, Branch, V == 0)
  op(0x51, IndirectYRead, fp(EOR), A)
  op(0x52, IndirectRead, fp(EOR), A)
  op(0x53, TAM)
  op(0x54, CSL)
  op(0x55, ZeroPageRead, fp(EOR), A, X)
  op(0x56, ZeroPageModify, fp(LSR), X)
  op(0x57, RMB, 5)
  op(0x58, Clear, I)
  op(0x59, AbsoluteRead, fp(EOR), A, Y)
  op(0x5a, Push, Y)
U op(0x5b, NOP)
U op(0x5c, NOP)
  op(0x5d, AbsoluteRead, fp(EOR), A, X)
  op(0x5e, AbsoluteModify, fp(LSR), X)
  op(0x5f, BBR, 5)
  op(0x60, RTS)
  op(0x61, IndirectRead, fp(ADC), A, X)
  op(0x62, Clear, A)
U op(0x63, NOP)
  op(0x64, ZeroPageWrite, 0)
  op(0x65, ZeroPageRead, fp(ADC), A)
  op(0x66, ZeroPageModify, fp(ROR))
  op(0x67, RMB, 6)
  op(0x68, Pull, A)
  op(0x69, Immediate, fp(ADC), A)
  op(0x6a, Implied, fp(ROR), A)
U op(0x6b, NOP)
  op(0x6c, JMPIndirect)
  op(0x6d, AbsoluteRead, fp(ADC), A)
  op(0x6e, AbsoluteModify, fp(ROR))
  op(0x6f, BBR, 6)
  op(0x70, Branch, V == 1)
  op(0x71, IndirectYRead, fp(ADC), A)
  op(0x72, IndirectRead, fp(ADC), A)
  op(0x73, BlockMove, fp(TII))
  op(0x74, ZeroPageWrite, 0, X)
  op(0x75, ZeroPageRead, fp(ADC), A, X)
  op(0x76, ZeroPageModify, fp(ROR), X)
  op(0x77, RMB, 7)
  op(0x78, Set, I)
  op(0x79, AbsoluteRead, fp(ADC), A, Y)
  op(0x7a, Pull, Y)
U op(0x7b, NOP)
  op(0x7c, JMPIndirect, X)
  op(0x7d, AbsoluteRead, fp(ADC), A, X)
  op(0x7e, AbsoluteModify, fp(ROR), X)
  op(0x7f, BBR, 7)
  op(0x80, Branch, 1)
  op(0x81, IndirectWrite, A, X)
  op(0x82, Clear, X)
  op(0x83, TSTZeroPage)
  op(0x84, ZeroPageWrite, Y)
  op(0x85, ZeroPageWrite, A)
  op(0x86, ZeroPageWrite, X)
  op(0x87, SMB, 0)
  op(0x88, Implied, fp(DEC), Y)
  op(0x89, Immediate, fp(BIT), A)
  op(0x8a, Transfer, X, A)
U op(0x8b, NOP)
  op(0x8c, AbsoluteWrite, Y)
  op(0x8d, AbsoluteWrite, A)
  op(0x8e, AbsoluteWrite, X)
  op(0x8f, BBS, 0)
  op(0x90, Branch, C == 0)
  op(0x91, IndirectYWrite, A)
  op(0x92, IndirectWrite, A)
  op(0x93, TSTAbsolute)
  op(0x94, ZeroPageWrite, Y, X)
  op(0x95, ZeroPageWrite, A, X)
  op(0x96, ZeroPageWrite, X, Y)
  op(0x97, SMB, 1)
  op(0x98, Transfer, Y, A)
  op(0x99, AbsoluteWrite, A, Y)
  op(0x9a, TXS)
U op(0x9b, NOP)
  op(0x9c, AbsoluteWrite, 0)
  op(0x9d, AbsoluteWrite, A, X)
  op(0x9e, AbsoluteWrite, 0, X)
  op(0x9f, BBS, 1)
  op(0xa0, Immediate, fp(LD), Y)
  op(0xa1, IndirectRead, fp(LD), A, X)
  op(0xa2, Immediate, fp(LD), X)
  op(0xa3, TSTZeroPage, X)
  op(0xa4, ZeroPageRead, fp(LD), Y)
  op(0xa5, ZeroPageRead, fp(LD), A)
  op(0xa6, ZeroPageRead, fp(LD), X)
  op(0xa7, SMB, 2)
  op(0xa8, Transfer, A, Y)
  op(0xa9, Immediate, fp(LD), A)
  op(0xaa, Transfer, A, X)
U op(0xab, NOP)
  op(0xac, AbsoluteRead, fp(LD), Y)
  op(0xad, AbsoluteRead, fp(LD), A)
  op(0xae, AbsoluteRead, fp(LD), X)
  op(0xaf, BBS, 2)
  op(0xb0, Branch, C == 1)
  op(0xb1, IndirectYRead, fp(LD), A)
  op(0xb2, IndirectRead, fp(LD), A)
  op(0xb3, TSTAbsolute, X)
  op(0xb4, ZeroPageRead, fp(LD), Y, X)
  op(0xb5, ZeroPageRead, fp(LD), A, X)
  op(0xb6, ZeroPageRead, fp(LD), X, Y)
  op(0xb7, SMB, 3)
  op(0xb8, Clear, V)
  op(0xb9, AbsoluteRead, fp(LD), A, Y)
  op(0xba, Transfer, S, X)
U op(0xbb, NOP)
  op(0xbc, AbsoluteRead, fp(LD), Y, X)
  op(0xbd, AbsoluteRead, fp(LD), A, X)
  op(0xbe, AbsoluteRead, fp(LD), X, Y)
  op(0xbf, BBS, 3)
  op(0xc0, Immediate, fp(CPY), Y)
  op(0xc1, IndirectRead, fp(CMP), A, X)
  op(0xc2, Clear, Y)
  op(0xc3, BlockMove, fp(TDD))
  op(0xc4, ZeroPageRead, fp(CPY), Y)
  op(0xc5, ZeroPageRead, fp(CMP), A)
  op(0xc6, ZeroPageModify, fp(DEC))
  op(0xc7, SMB, 4)
  op(0xc8, Implied, fp(INC), Y)
  op(0xc9, Immediate, fp(CMP), A)
  op(0xca, Implied, fp(DEC), X)
U op(0xcb, NOP)
  op(0xcc, AbsoluteRead, fp(CPY), Y)
  op(0xcd, AbsoluteRead, fp(CMP), A)
  op(0xce, AbsoluteModify, fp(DEC))
  op(0xcf, BBS, 4)
  op(0xd0, Branch, Z == 0)
  op(0xd1, IndirectYRead, fp(CMP), A)
  op(0xd2, IndirectRead, fp(CMP), A)
  op(0xd3, BlockMove, fp(TIN))
  op(0xd4, CSH)
  op(0xd5, ZeroPageRead, fp(CMP), A, X)
  op(0xd6, ZeroPageModify, fp(DEC), X)
  op(0xd7, SMB, 5)
  op(0xd8, Clear, D)
  op(0xd9, AbsoluteRead, fp(CMP), A, Y)
  op(0xda, Push, X)
U op(0xdb, NOP)
U op(0xdc, NOP)
  op(0xdd, AbsoluteRead, fp(CMP), A, X)
  op(0xde, AbsoluteModify, fp(DEC), X)
  op(0xdf, BBS, 5)
  op(0xe0, Immediate, fp(CPX), X)
  op(0xe1, IndirectRead, fp(SBC), A, X)
U op(0xe2, NOP)
  op(0xe3, BlockMove, fp(TIA))
  op(0xe4, ZeroPageRead, fp(CPX), X)
  op(0xe5, ZeroPageRead, fp(SBC), A)
  op(0xe6, ZeroPageModify, fp(INC))
  op(0xe7, SMB, 6)
  op(0xe8, Implied, fp(INC), X)
  op(0xe9, Immediate, fp(SBC), A)
  op(0xea, NOP)
U op(0xeb, NOP)
  op(0xec, AbsoluteRead, fp(CPX), X)
  op(0xed, AbsoluteRead, fp(SBC), A)
  op(0xee, AbsoluteModify, fp(INC))
  op(0xef, BBS, 6)
  op(0xf0, Branch, Z == 1)
  op(0xf1, IndirectYRead, fp(SBC), A)
  op(0xf2, IndirectRead, fp(SBC), A)
  op(0xf3, BlockMove, fp(TAI))
  op(0xf4, Set, T)
  op(0xf5, ZeroPageRead, fp(SBC), A, X)
  op(0xf6, ZeroPageModify, fp(INC), X)
  op(0xf7, SMB, 7)
  op(0xf8, Set, D)
  op(0xf9, AbsoluteRead, fp(SBC), A, Y)
  op(0xfa, Pull, X)
U op(0xfb, NOP)
U op(0xfc, NOP)
  op(0xfd, AbsoluteRead, fp(SBC), A, X)
  op(0xfe, AbsoluteModify, fp(INC), X)
  op(0xff, BBS, 7)
  }
  #undef U
}

#undef op
#undef fp
