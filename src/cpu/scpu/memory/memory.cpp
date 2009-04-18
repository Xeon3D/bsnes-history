#ifdef SCPU_CPP

void sCPU::op_io() {
  regs.bus = -1U;
  status.clock_count = 6;
  precycle_edge();
  add_clocks(6);
  if(regs.wai) scheduler.sync_cpucop();
  cycle_edge();
}

uint8 sCPU::op_read(uint32 addr) {
  status.clock_count = speed(regs.bus = addr);
  precycle_edge();
  add_clocks(status.clock_count - 4);

  scheduler.sync_cpucop();
  scheduler.sync_cpuppu();
  regs.mdr = bus.read(addr);
  add_clocks(4);
  cycle_edge();
  return regs.mdr;
}

void sCPU::op_write(uint32 addr, uint8 data) {
  status.clock_count = speed(regs.bus = addr);
  precycle_edge();
  add_clocks(status.clock_count);

  scheduler.sync_cpucop();
  scheduler.sync_cpuppu();
  bus.write(addr, regs.mdr = data);
  cycle_edge();
}

unsigned sCPU::speed(unsigned addr) const {
  if(addr & 0x408000) {
    if(addr & 0x800000) return status.rom_speed;
    return 8;
  }
  if((addr + 0x6000) & 0x4000) return 8;
  if((addr - 0x4000) & 0x7e00) return 6;
  return 12;
}

#endif
