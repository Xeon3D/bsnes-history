auto Z80::wait(uint clocks) -> void {
  step(clocks);
}

auto Z80::opcode() -> uint8 {
  step(4);
  return bus->read(r.pc++);
}

auto Z80::operand() -> uint8 {
  step(3);
  return bus->read(r.pc++);
}

auto Z80::read(uint16 addr) -> uint8 {
  step(3);
  return bus->read(addr);
}

auto Z80::write(uint16 addr, uint8 data) -> void {
  step(3);
  return bus->write(addr, data);
}

auto Z80::in(uint8 addr) -> uint8 {
  step(4);
  return bus->in(addr);
}

auto Z80::out(uint8 addr, uint8 data) -> void {
  step(4);
  return bus->out(addr, data);
}
