//legend:
//  a   = r.a
//  e   = relative operand
//  in  = (operand)
//  inn = (operand-word)
//  irr = (register-word)
//  o   = opcode bits
//  n   = operand
//  nn  = operand-word
//  r   = register

auto Z80::CP(uint8 x) -> void {
  uint9 y = r.a - x;

  r.p.c = y.bit(8);
  r.p.n = 1;
  r.p.v = (r.a ^ x) & (r.a ^ y) & 0x80;
  r.p.x = y.bit(3);
  r.p.h = (r.a ^ y ^ x) & 0x10;
  r.p.y = y.bit(5);
  r.p.z = (uint8)y == 0;
  r.p.s = y.bit(7);
}

auto Z80::instructionCP_irr(uint16& x) -> void {
  auto addr = displace(x);
  CP(read(addr));
}

auto Z80::instructionCP_n() -> void {
  CP(operand());
}

auto Z80::instructionCP_r(uint8& x) -> void {
  CP(x);
}

auto Z80::instructionDI() -> void {
  r.iff1 = 0;
  r.iff2 = 0;
}

auto Z80::instructionEI() -> void {
  r.iff1 = 1;
  r.iff2 = 1;
}

auto Z80::instructionIM_o(uint2 code) -> void {
  wait(4);
  r.im = code;
}

auto Z80::instructionIN_a_in() -> void {
  r.a = in(operand());
}

auto Z80::instructionJP_c_nn(bool c) -> void {
  auto lo = operand();
  auto hi = operand();
  if(c) r.pc = hi << 8 | lo << 0;
}

auto Z80::instructionJR_c_e(bool c) -> void {
  auto e = operand();
  if(!c) return;
  wait(5);
  r.pc += (int8)e;
}

auto Z80::instructionLD_inn_a() -> void {
  auto lo = operand();
  auto hi = operand();
  write(hi << 8 | lo << 0, r.a);
}

auto Z80::instructionLD_irr_n(uint16& x) -> void {
  auto addr = displace(x);
  write(addr, operand());
}

auto Z80::instructionLD_irr_r(uint16& x, uint8& y) -> void {
  auto addr = displace(x);
  write(addr, y);
}

auto Z80::instructionLD_r_n(uint8& x) -> void {
  x = operand();
}

auto Z80::instructionLD_rr_nn(uint16& x) -> void {
  auto lo = operand();
  auto hi = operand();
  x = hi << 8 | lo << 0;
}

auto Z80::instructionNOP() -> void {
}
