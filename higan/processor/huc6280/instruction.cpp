#define op(id, name, ...) case id: instruction_##name(__VA_ARGS__); return;
#define fp(name) &HuC6280::name

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
    op(0x09, memory, fp(ORA))
    op(0x29, memory, fp(AND))
    op(0x49, memory, fp(EOR))
    op(0x69, memory, fp(ADC))
    }
  }

  #define U
  switch(code) {
  op(0x00, BRK)
  op(0x01, indirectLoad, fp(ORA), A, X)
  op(0x02, swap, X, Y)
  op(0x03, ST, 0)
  op(0x04, zeropageModify, fp(TSB))
  op(0x05, zeropageLoad, fp(ORA), A)
  op(0x06, zeropageModify, fp(ASL))
  op(0x07, RMB, 0)
  op(0x08, push, P)
  op(0x09, immediate, fp(ORA), A)
  op(0x0a, implied, fp(ASL), A)
U op(0x0b, NOP)
  op(0x0c, absoluteModify, fp(TSB))
  op(0x0d, absoluteLoad, fp(ORA), A)
  op(0x0e, absoluteModify, fp(ASL))
  op(0x0f, BBR, 0)
  op(0x10, branch, N == 0)
  op(0x11, indirectYLoad, fp(ORA), A)
  op(0x12, indirectLoad, fp(ORA), A)
  op(0x13, ST, 1)
  op(0x14, zeropageModify, fp(TRB))
  op(0x15, zeropageLoad, fp(ORA), A, X)
  op(0x16, zeropageModify, fp(ASL), X)
  op(0x17, RMB, 1)
  op(0x18, clear, C)
  op(0x19, absoluteLoad, fp(ORA), A, Y)
  op(0x1a, implied, fp(INC), A)
U op(0x1b, NOP)
  op(0x1c, absoluteModify, fp(TRB))
  op(0x1d, absoluteLoad, fp(ORA), A, X)
  op(0x1e, absoluteModify, fp(ASL), X)
  op(0x1f, BBR, 1)
  op(0x20, JSR)
  op(0x21, indirectLoad, fp(AND), A, X)
  op(0x22, swap, A, X)
  op(0x23, ST, 2)
  op(0x24, zeropageLoad, fp(BIT), A)
  op(0x25, zeropageLoad, fp(AND), A)
  op(0x26, zeropageModify, fp(ROL))
  op(0x27, RMB, 2)
  op(0x28, pullP)
  op(0x29, immediate, fp(AND), A)
  op(0x2a, implied, fp(ROL), A)
U op(0x2b, NOP)
  op(0x2c, absoluteLoad, fp(BIT), A)
  op(0x2d, absoluteLoad, fp(AND), A)
  op(0x2e, absoluteModify, fp(ROL))
  op(0x2f, BBR, 2)
  op(0x30, branch, N == 1)
  op(0x31, indirectYLoad, fp(AND), A)
  op(0x32, indirectLoad, fp(AND), A)
U op(0x33, NOP)
  op(0x34, zeropageLoad, fp(BIT), A, X)
  op(0x35, zeropageLoad, fp(AND), A, X)
  op(0x36, zeropageModify, fp(ROL), X)
  op(0x37, RMB, 3)
  op(0x38, set, C)
  op(0x39, absoluteLoad, fp(AND), A, Y)
  op(0x3a, implied, fp(DEC), A)
U op(0x3b, NOP)
  op(0x3c, absoluteLoad, fp(BIT), A, X)
  op(0x3d, absoluteLoad, fp(AND), A, X)
  op(0x3e, absoluteModify, fp(ROL), X)
  op(0x3f, BBR, 3)
  op(0x40, RTI)
  op(0x41, indirectLoad, fp(EOR), A, X)
  op(0x42, swap, A, Y)
  op(0x43, TMA)
  op(0x44, BSR)
  op(0x45, zeropageLoad, fp(EOR), A)
  op(0x46, zeropageModify, fp(LSR))
  op(0x47, RMB, 4)
  op(0x48, push, A)
  op(0x49, immediate, fp(EOR), A)
  op(0x4a, implied, fp(LSR), A)
U op(0x4b, NOP)
  op(0x4c, JMP_absolute)
  op(0x4d, absoluteLoad, fp(EOR), A)
  op(0x4e, absoluteModify, fp(LSR))
  op(0x4f, BBR, 4)
  op(0x50, branch, V == 0)
  op(0x51, indirectYLoad, fp(EOR), A)
  op(0x52, indirectLoad, fp(EOR), A)
  op(0x53, TAM)
  op(0x54, CSL)
  op(0x55, zeropageLoad, fp(EOR), A, X)
  op(0x56, zeropageModify, fp(LSR), X)
  op(0x57, RMB, 5)
  op(0x58, clear, I)
  op(0x59, absoluteLoad, fp(EOR), A, Y)
  op(0x5a, push, Y)
U op(0x5b, NOP)
U op(0x5c, NOP)
  op(0x5d, absoluteLoad, fp(EOR), A, X)
  op(0x5e, absoluteModify, fp(LSR), X)
  op(0x5f, BBR, 5)
  op(0x60, RTS)
  op(0x61, indirectLoad, fp(ADC), A, X)
  op(0x62, clear, A)
U op(0x63, NOP)
  op(0x64, zeropageStore, 0)
  op(0x65, zeropageLoad, fp(ADC), A)
  op(0x66, zeropageModify, fp(ROR))
  op(0x67, RMB, 6)
  op(0x68, pull, A)
  op(0x69, immediate, fp(ADC), A)
  op(0x6a, implied, fp(ROR), A)
U op(0x6b, NOP)
  op(0x6c, JMP_indirect)
  op(0x6d, absoluteLoad, fp(ADC), A)
  op(0x6e, absoluteModify, fp(ROR))
  op(0x6f, BBR, 6)
  op(0x70, branch, V == 1)
  op(0x71, indirectYLoad, fp(ADC), A)
  op(0x72, indirectLoad, fp(ADC), A)
  op(0x73, blockmove, fp(TII))
  op(0x74, zeropageStore, 0, X)
  op(0x75, zeropageLoad, fp(ADC), A, X)
  op(0x76, zeropageModify, fp(ROR), X)
  op(0x77, RMB, 7)
  op(0x78, set, I)
  op(0x79, absoluteLoad, fp(ADC), A, Y)
  op(0x7a, pull, Y)
U op(0x7b, NOP)
  op(0x7c, JMP_indirect, X)
  op(0x7d, absoluteLoad, fp(ADC), A, X)
  op(0x7e, absoluteModify, fp(ROR), X)
  op(0x7f, BBR, 7)
  op(0x80, branch, 1)
  op(0x81, indirectStore, A, X)
  op(0x82, clear, X)
  op(0x83, TST_zeropage)
  op(0x84, zeropageStore, Y)
  op(0x85, zeropageStore, A)
  op(0x86, zeropageStore, X)
  op(0x87, SMB, 0)
  op(0x88, implied, fp(DEC), Y)
  op(0x89, immediate, fp(BIT), A)
  op(0x8a, transfer, X, A)
U op(0x8b, NOP)
  op(0x8c, absoluteStore, Y)
  op(0x8d, absoluteStore, A)
  op(0x8e, absoluteStore, X)
  op(0x8f, BBS, 0)
  op(0x90, branch, C == 0)
  op(0x91, indirectYStore, A)
  op(0x92, indirectStore, A)
  op(0x93, TST_absolute)
  op(0x94, zeropageStore, Y, X)
  op(0x95, zeropageStore, A, X)
  op(0x96, zeropageStore, X, Y)
  op(0x97, SMB, 1)
  op(0x98, transfer, Y, A)
  op(0x99, absoluteStore, A, Y)
  op(0x9a, TXS)
U op(0x9b, NOP)
  op(0x9c, absoluteStore, 0)
  op(0x9d, absoluteStore, A, X)
  op(0x9e, absoluteStore, 0, X)
  op(0x9f, BBS, 1)
  op(0xa0, immediate, fp(LD), Y)
  op(0xa1, indirectLoad, fp(LD), A, X)
  op(0xa2, immediate, fp(LD), X)
  op(0xa3, TST_zeropage, X)
  op(0xa4, zeropageLoad, fp(LD), Y)
  op(0xa5, zeropageLoad, fp(LD), A)
  op(0xa6, zeropageLoad, fp(LD), X)
  op(0xa7, SMB, 2)
  op(0xa8, transfer, A, Y)
  op(0xa9, immediate, fp(LD), A)
  op(0xaa, transfer, A, X)
U op(0xab, NOP)
  op(0xac, absoluteLoad, fp(LD), Y)
  op(0xad, absoluteLoad, fp(LD), A)
  op(0xae, absoluteLoad, fp(LD), X)
  op(0xaf, BBS, 2)
  op(0xb0, branch, C == 1)
  op(0xb1, indirectYLoad, fp(LD), A)
  op(0xb2, indirectLoad, fp(LD), A)
  op(0xb3, TST_absolute, X)
  op(0xb4, zeropageLoad, fp(LD), Y, X)
  op(0xb5, zeropageLoad, fp(LD), A, X)
  op(0xb6, zeropageLoad, fp(LD), X, Y)
  op(0xb7, SMB, 3)
  op(0xb8, clear, V)
  op(0xb9, absoluteLoad, fp(LD), A, Y)
  op(0xba, transfer, S, X)
U op(0xbb, NOP)
  op(0xbc, absoluteLoad, fp(LD), Y, X)
  op(0xbd, absoluteLoad, fp(LD), A, X)
  op(0xbe, absoluteLoad, fp(LD), X, Y)
  op(0xbf, BBS, 3)
  op(0xc0, immediate, fp(CPY), Y)
  op(0xc1, indirectLoad, fp(CMP), A, X)
  op(0xc2, clear, Y)
  op(0xc3, blockmove, fp(TDD))
  op(0xc4, zeropageLoad, fp(CPY), Y)
  op(0xc5, zeropageLoad, fp(CMP), A)
  op(0xc6, zeropageModify, fp(DEC))
  op(0xc7, SMB, 4)
  op(0xc8, implied, fp(INC), Y)
  op(0xc9, immediate, fp(CMP), A)
  op(0xca, implied, fp(DEC), X)
U op(0xcb, NOP)
  op(0xcc, absoluteLoad, fp(CPY), Y)
  op(0xcd, absoluteLoad, fp(CMP), A)
  op(0xce, absoluteModify, fp(DEC))
  op(0xcf, BBS, 4)
  op(0xd0, branch, Z == 0)
  op(0xd1, indirectYLoad, fp(CMP), A)
  op(0xd2, indirectLoad, fp(CMP), A)
  op(0xd3, blockmove, fp(TIN))
  op(0xd4, CSH)
  op(0xd5, zeropageLoad, fp(CMP), A, X)
  op(0xd6, zeropageModify, fp(DEC), X)
  op(0xd7, SMB, 5)
  op(0xd8, clear, D)
  op(0xd9, absoluteLoad, fp(CMP), A, Y)
  op(0xda, push, X)
U op(0xdb, NOP)
U op(0xdc, NOP)
  op(0xdd, absoluteLoad, fp(CMP), A, X)
  op(0xde, absoluteModify, fp(DEC), X)
  op(0xdf, BBS, 5)
  op(0xe0, immediate, fp(CPX), X)
  op(0xe1, indirectLoad, fp(SBC), A, X)
U op(0xe2, NOP)
  op(0xe3, blockmove, fp(TIA))
  op(0xe4, zeropageLoad, fp(CPX), X)
  op(0xe5, zeropageLoad, fp(SBC), A)
  op(0xe6, zeropageModify, fp(INC))
  op(0xe7, SMB, 6)
  op(0xe8, implied, fp(INC), X)
  op(0xe9, immediate, fp(SBC), A)
  op(0xea, NOP)
U op(0xeb, NOP)
  op(0xec, absoluteLoad, fp(CPX), X)
  op(0xed, absoluteLoad, fp(SBC), A)
  op(0xee, absoluteModify, fp(INC))
  op(0xef, BBS, 6)
  op(0xf0, branch, Z == 1)
  op(0xf1, indirectYLoad, fp(SBC), A)
  op(0xf2, indirectLoad, fp(SBC), A)
  op(0xf3, blockmove, fp(TAI))
  op(0xf4, set, T)
  op(0xf5, zeropageLoad, fp(SBC), A, X)
  op(0xf6, zeropageModify, fp(INC), X)
  op(0xf7, SMB, 7)
  op(0xf8, set, D)
  op(0xf9, absoluteLoad, fp(SBC), A, Y)
  op(0xfa, pull, X)
U op(0xfb, NOP)
U op(0xfc, NOP)
  op(0xfd, absoluteLoad, fp(SBC), A, X)
  op(0xfe, absoluteModify, fp(INC), X)
  op(0xff, BBS, 7)
  }
  #undef U
}

#undef op
#undef fp
