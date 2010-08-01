#ifdef BPPU_CPP

void bPPU::latch_counters() {
  regs.hcounter = cpu.hdot();
  regs.vcounter = cpu.vcounter();
  regs.counters_latched = true;
}

uint16 bPPU::get_vram_address() {
  uint16 addr = regs.vram_addr;
  switch(regs.vram_mapping) {
    case 0: break;  //direct mapping
    case 1: addr = (addr & 0xff00) | ((addr & 0x001f) << 3) | ((addr >> 5) & 7); break;
    case 2: addr = (addr & 0xfe00) | ((addr & 0x003f) << 3) | ((addr >> 6) & 7); break;
    case 3: addr = (addr & 0xfc00) | ((addr & 0x007f) << 3) | ((addr >> 7) & 7); break;
  }
  return (addr << 1);
}

//NOTE: all VRAM writes during active display are invalid. Unlike OAM and CGRAM, they will
//not be written anywhere at all. The below address ranges for where writes are invalid have
//been validated on hardware, as has the edge case where the S-CPU MDR can be written if the
//write occurs during the very last clock cycle of vblank.

uint8 bPPU::vram_mmio_read(uint16 addr) {
  uint8 data;

  if(regs.display_disabled == true) {
    data = memory::vram[addr];
  } else {
    uint16 v = cpu.vcounter();
    uint16 h = cpu.hcounter();
    uint16 ls = ((system.region() == System::Region::NTSC ? 525 : 625) >> 1) - 1;
    if(interlace() && !cpu.field()) ls++;

    if(v == ls && h == 1362) {
      data = 0x00;
    } else if(v < (!overscan() ? 224 : 239)) {
      data = 0x00;
    } else if(v == (!overscan() ? 224 : 239)) {
      if(h == 1362) {
        data = memory::vram[addr];
      } else {
        data = 0x00;
      }
    } else {
      data = memory::vram[addr];
    }
  }

  return data;
}

void bPPU::vram_mmio_write(uint16 addr, uint8 data) {
  if(regs.display_disabled == true) {
    memory::vram[addr] = data;
  } else {
    uint16 v = cpu.vcounter();
    uint16 h = cpu.hcounter();
    if(v == 0) {
      if(h <= 4) {
        memory::vram[addr] = data;
      } else if(h == 6) {
        memory::vram[addr] = cpu.regs.mdr;
      } else {
        //no write
      }
    } else if(v < (!overscan() ? 225 : 240)) {
      //no write
    } else if(v == (!overscan() ? 225 : 240)) {
      if(h <= 4) {
        //no write
      } else {
        memory::vram[addr] = data;
      }
    } else {
      memory::vram[addr] = data;
    }
  }
}

uint8 bPPU::oam_mmio_read(uint16 addr) {
  addr &= 0x03ff;
  if(addr & 0x0200) addr &= 0x021f;
  uint8 data;

  if(regs.display_disabled == true) {
    data = memory::oam[addr];
  } else {
    if(cpu.vcounter() < (!overscan() ? 225 : 240)) {
      data = memory::oam[regs.ioamaddr];
    } else {
      data = memory::oam[addr];
    }
  }

  return data;
}

void bPPU::oam_mmio_write(uint16 addr, uint8 data) {
  addr &= 0x03ff;
  if(addr & 0x0200) addr &= 0x021f;

  sprite_list_valid = false;

  if(regs.display_disabled == true) {
    memory::oam[addr] = data;
  } else {
    if(cpu.vcounter() < (!overscan() ? 225 : 240)) {
      memory::oam[regs.ioamaddr] = data;
    } else {
      memory::oam[addr] = data;
    }
  }
}

uint8 bPPU::cgram_mmio_read(uint16 addr) {
  addr &= 0x01ff;
  uint8 data;

  if(1 || regs.display_disabled == true) {
    data = memory::cgram[addr];
  } else {
    uint16 v = cpu.vcounter();
    uint16 h = cpu.hcounter();
    if(v < (!overscan() ? 225 : 240) && h >= 128 && h < 1096) {
      data = memory::cgram[regs.icgramaddr] & 0x7f;
    } else {
      data = memory::cgram[addr];
    }
  }

  if(addr & 1) data &= 0x7f;
  return data;
}

void bPPU::cgram_mmio_write(uint16 addr, uint8 data) {
  addr &= 0x01ff;
  if(addr & 1) data &= 0x7f;

  if(1 || regs.display_disabled == true) {
    memory::cgram[addr] = data;
  } else {
    uint16 v = cpu.vcounter();
    uint16 h = cpu.hcounter();
    if(v < (!overscan() ? 225 : 240) && h >= 128 && h < 1096) {
      memory::cgram[regs.icgramaddr] = data & 0x7f;
    } else {
      memory::cgram[addr] = data;
    }
  }
}

#endif
