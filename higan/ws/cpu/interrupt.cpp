auto CPU::poll() -> void {
  if(!state.poll || !(r.interruptStatus & r.interruptEnable)) return;
  state.halt = false;
  if(!V30MZ::r.f.i) return;

  //find and execute first pending interrupt in order of priority (7-0)
  for(int n = 7; n >= 0; n--) {
    if(r.interruptStatus & r.interruptEnable & (1 << n)) {
      return interrupt(r.interruptBase + n);
    }
  }
}

auto CPU::raise(Interrupt i) -> void {
  auto mask = 1 << (uint)i;
  r.interruptStatus |= mask;
}

auto CPU::lower(Interrupt i) -> void {
  auto mask = 1 << (uint)i;
  r.interruptStatus &= ~mask;
}
