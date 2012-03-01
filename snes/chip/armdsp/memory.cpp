#ifdef ARMDSP_CPP

uint8 ArmDSP::bus_iread(uint32 addr) {
  if(addr >= 0x00000000 && addr <= 0x0001ffff) {
    return programROM[addr & 0x0001ffff];
  }

  if(addr >= 0x40000000 && addr <= 0x400000ff) {
    if(addr == 0x40000000) return 0x00;

    if(addr == 0x40000010) {
      if(bridge.cputoarm.ready) {
        bridge.cputoarm.ready = false;
        return bridge.cputoarm.data;
      }
    }

    if(addr == 0x40000020) {
      return bridge.status();
    }

    if(addr == 0x40000024) return 0x00;
    if(addr == 0x40000028) return 0x00;
    if(addr == 0x4000002c) return 0x00;
  }

  if(addr >= 0xa0000000 && addr <= 0xa0007fff) {
    return dataROM[addr & 0x00007fff];
  }

  if(addr >= 0xe0000000 && addr <= 0xe0003fff) {
    return programRAM[addr & 0x00003fff];
  }

  if((addr & 3) == 0) print("* ARM r", hex<8>(addr), "\n");
  return 0x00;
}

void ArmDSP::bus_iwrite(uint32 addr, uint8 data) {
  if(addr >= 0x40000000 && addr <= 0x400000ff) {
    if(addr == 0x40000000) {
      bridge.armtocpu.ready = true;
      bridge.armtocpu.data = data;
      return;
    }

    if(addr == 0x40000020) return;
    if(addr == 0x40000024) return;
    if(addr == 0x40000028) return;
    if(addr == 0x4000002c) return print("* w4000002c = ", hex<2>(data), "\n");
  }

  if(addr >= 0xe0000000 && addr <= 0xe0003fff) {
    programRAM[addr & 0x00003fff] = data;
    return;
  }

  if((addr & 3) == 0) print("* ARM w", hex<8>(addr), " = ", hex<2>(data), "\n");
}

uint32 ArmDSP::bus_readbyte(uint32 addr) {
  return bus_iread(addr);
}

uint32 ArmDSP::bus_readword(uint32 addr) {
  addr &= ~3;
  return (
    (bus_iread(addr + 0) <<  0)
  | (bus_iread(addr + 1) <<  8)
  | (bus_iread(addr + 2) << 16)
  | (bus_iread(addr + 3) << 24)
  );
}

void ArmDSP::bus_writebyte(uint32 addr, uint32 data) {
  return bus_iwrite(addr, data);
}

void ArmDSP::bus_writeword(uint32 addr, uint32 data) {
  addr &= ~3;
  bus_iwrite(addr + 0, data >>  0);
  bus_iwrite(addr + 1, data >>  8);
  bus_iwrite(addr + 2, data >> 16);
  bus_iwrite(addr + 3, data >> 24);
}

#endif
