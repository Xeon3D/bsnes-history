auto Bus::mirror(uint addr, uint size) -> uint {
  if(size == 0) return 0;
  uint base = 0;
  uint mask = 1 << 23;
  while(addr >= size) {
    while(!(addr & mask)) mask >>= 1;
    addr -= mask;
    if(size > mask) {
      size -= mask;
      base += mask;
    }
    mask >>= 1;
  }
  return base + addr;
}

auto Bus::reduce(uint addr, uint mask) -> uint {
  while(mask) {
    uint bits = (mask & -mask) - 1;
    addr = ((addr >> 1) & ~bits) | (addr & bits);
    mask = (mask & (mask - 1)) >> 1;
  }
  return addr;
}

auto Bus::read(uint24 addr, uint8 data) -> uint8 {
  data = reader[lookup[addr]](target[addr], data);
  if(cheat) {
    if(!(addr & 0x40e000)) addr = 0x7e0000 | (addr & 0x1fff);  //de-mirror WRAM
    if(auto result = cheat.find(addr, data)) return result();
  }
  return data;
}

auto Bus::write(uint24 addr, uint8 data) -> void {
  return writer[lookup[addr]](target[addr], data);
}
