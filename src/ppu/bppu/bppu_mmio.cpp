#ifdef BPPU_CPP

void bPPU::latch_counters() {
  regs.hcounter = hdot();
  regs.vcounter = vcounter();
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
    uint16 v = vcounter();
    uint16 h = hcounter();
    uint16 ls = ((snes.region() == SNES::NTSC ? 525 : 625) >> 1) - 1;
    if(interlace() && !field()) ls++;

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
    uint16 v = vcounter();
    uint16 h = hcounter();
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

//NOTE: OAM accesses during active display are rerouted to 0x0218 ... this can be considered
//a hack. The actual address varies during rendering, as the S-PPU reads in data itself for
//processing. Unfortunately, we have yet to determine how this works. The algorithm cannot be
//reverse engineered using a scanline renderer such as this, and at this time, there does not
//exist a more accurate SNES PPU emulator to work from. The only known game to actually access
//OAM during active display is Uniracers. It expects accesses to map to offset 0x0218.
//It was decided by public consensus to map writes to this address to match Uniracers, primarily
//because it is the only game observed to do this, but also because mapping to this address does
//not contradict any of our findings, because we have no findings whatsoever on this behavior.
//Think of this what you will, I openly admit that this is a hack. But it is more accurate than
//writing to the 'expected' address set by $2102,$2103, and will catch problems in software that
//accidentally accesses OAM during active display by virtue of not returning the expected data.

uint8 bPPU::oam_mmio_read(uint16 addr) {
  addr &= 0x03ff;
  if(addr & 0x0200) addr &= 0x021f;
  uint8 data;

  if(regs.display_disabled == true) {
    data = memory::oam[addr];
  } else {
    if(vcounter() < (!overscan() ? 225 : 240)) {
      data = memory::oam[0x0218];
    } else {
      data = memory::oam[addr];
    }
  }

  return data;
}

void bPPU::oam_mmio_write(uint16 addr, uint8 data) {
  addr &= 0x03ff;
  if(addr & 0x0200) addr &= 0x021f;

  if(regs.display_disabled == true) {
    memory::oam[addr] = data;
  } else {
    if(vcounter() < (!overscan() ? 225 : 240)) {
      memory::oam[0x0218] = data;
    } else {
      memory::oam[addr] = data;
    }
  }
}

//NOTE: CGRAM writes during hblank are valid. During active display, the actual address the
//data is written to varies, as the S-PPU itself changes the address. Like OAM, we do not know
//the exact algorithm used, but we have zero known examples of any commercial software that
//attempts to do this. Therefore, the addresses are mapped to 0x01ff. There is nothing special
//about this address, it is simply more accurate to invalidate the 'expected' address than not.

uint8 bPPU::cgram_mmio_read(uint16 addr) {
  addr &= 0x01ff;
  uint8 data;

  if(regs.display_disabled == true) {
    data = memory::cgram[addr];
  } else {
    uint16 v = vcounter();
    uint16 h = hcounter();
    if(v < (!overscan() ? 225 : 240) && h >= 72 && h < 1096) {
      data = memory::cgram[0x01ff] & 0x7f;
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

  if(regs.display_disabled == true) {
    memory::cgram[addr] = data;
  } else {
    uint16 v = vcounter();
    uint16 h = hcounter();
    if(v < (!overscan() ? 225 : 240) && h >= 72 && h < 1096) {
      memory::cgram[0x01ff] = data & 0x7f;
    } else {
      memory::cgram[addr] = data;
    }
  }
}

//INIDISP
void bPPU::mmio_w2100(uint8 value) {
  if(regs.display_disabled == true && vcounter() == (!overscan() ? 225 : 240)) {
    regs.oam_addr = regs.oam_baseaddr << 1;
    regs.oam_firstsprite = (regs.oam_priority == false) ? 0 : (regs.oam_addr >> 2) & 127;
  }

  regs.display_disabled   = !!(value & 0x80);
  regs.display_brightness = value & 15;
}

//OBSEL
void bPPU::mmio_w2101(uint8 value) {
  regs.oam_basesize   = (value >> 5) & 7;
  regs.oam_nameselect = (value >> 3) & 3;
  regs.oam_tdaddr     = (value & 3) << 14;
}

//OAMADDL
void bPPU::mmio_w2102(uint8 data) {
  regs.oam_baseaddr    = (regs.oam_baseaddr & ~0xff) | (data << 0);
  regs.oam_baseaddr   &= 0x01ff;
  regs.oam_addr        = regs.oam_baseaddr << 1;
  regs.oam_firstsprite = (regs.oam_priority == false) ? 0 : (regs.oam_addr >> 2) & 127;
}

//OAMADDH
void bPPU::mmio_w2103(uint8 data) {
  regs.oam_priority    = !!(data & 0x80);
  regs.oam_baseaddr    = (regs.oam_baseaddr &  0xff) | (data << 8);
  regs.oam_baseaddr   &= 0x01ff;
  regs.oam_addr        = regs.oam_baseaddr << 1;
  regs.oam_firstsprite = (regs.oam_priority == false) ? 0 : (regs.oam_addr >> 2) & 127;
}

//OAMDATA
void bPPU::mmio_w2104(uint8 data) {
  if(regs.oam_addr & 0x0200) {
    oam_mmio_write(regs.oam_addr, data);
  } else if((regs.oam_addr & 1) == 0) {
    regs.oam_latchdata = data;
  } else {
    oam_mmio_write((regs.oam_addr & ~1) + 0, regs.oam_latchdata);
    oam_mmio_write((regs.oam_addr & ~1) + 1, data);
  }

  regs.oam_addr++;
  regs.oam_addr &= 0x03ff;
  regs.oam_firstsprite = (regs.oam_priority == false) ? 0 : (regs.oam_addr >> 2) & 127;
}

//BGMODE
void bPPU::mmio_w2105(uint8 value) {
  regs.bg_tilesize[BG4] = !!(value & 0x80);
  regs.bg_tilesize[BG3] = !!(value & 0x40);
  regs.bg_tilesize[BG2] = !!(value & 0x20);
  regs.bg_tilesize[BG1] = !!(value & 0x10);
  regs.bg3_priority     = !!(value & 0x08);
  regs.bg_mode          = (value & 7);
}

//MOSAIC
void bPPU::mmio_w2106(uint8 value) {
  regs.mosaic_size         = (value >> 4) & 15;
  regs.mosaic_enabled[BG4] = !!(value & 0x08);
  regs.mosaic_enabled[BG3] = !!(value & 0x04);
  regs.mosaic_enabled[BG2] = !!(value & 0x02);
  regs.mosaic_enabled[BG1] = !!(value & 0x01);
}

//BG1SC
void bPPU::mmio_w2107(uint8 value) {
  regs.bg_scaddr[BG1] = (value & 0x7c) << 9;
  regs.bg_scsize[BG1] = value & 3;
}

//BG2SC
void bPPU::mmio_w2108(uint8 value) {
  regs.bg_scaddr[BG2] = (value & 0x7c) << 9;
  regs.bg_scsize[BG2] = value & 3;
}

//BG3SC
void bPPU::mmio_w2109(uint8 value) {
  regs.bg_scaddr[BG3] = (value & 0x7c) << 9;
  regs.bg_scsize[BG3] = value & 3;
}

//BG4SC
void bPPU::mmio_w210a(uint8 value) {
  regs.bg_scaddr[BG4] = (value & 0x7c) << 9;
  regs.bg_scsize[BG4] = value & 3;
}

//BG12NBA
void bPPU::mmio_w210b(uint8 value) {
  regs.bg_tdaddr[BG1] = (value & 0x07) << 13;
  regs.bg_tdaddr[BG2] = (value & 0x70) <<  9;
}

//BG34NBA
void bPPU::mmio_w210c(uint8 value) {
  regs.bg_tdaddr[BG3] = (value & 0x07) << 13;
  regs.bg_tdaddr[BG4] = (value & 0x70) <<  9;
}

//BG1HOFS
void bPPU::mmio_w210d(uint8 value) {
  regs.m7_hofs  = (value << 8) | regs.m7_latch;
  regs.m7_latch = value;

  regs.bg_hofs[BG1] = (value << 8) | (regs.bg_ofslatch & ~7) | ((regs.bg_hofs[BG1] >> 8) & 7);
  regs.bg_ofslatch  = value;
}

//BG1VOFS
void bPPU::mmio_w210e(uint8 value) {
  regs.m7_vofs  = (value << 8) | regs.m7_latch;
  regs.m7_latch = value;

  regs.bg_vofs[BG1] = (value << 8) | (regs.bg_ofslatch);
  regs.bg_ofslatch  = value;
}

//BG2HOFS
void bPPU::mmio_w210f(uint8 value) {
  regs.bg_hofs[BG2] = (value << 8) | (regs.bg_ofslatch & ~7) | ((regs.bg_hofs[BG2] >> 8) & 7);
  regs.bg_ofslatch  = value;
}

//BG2VOFS
void bPPU::mmio_w2110(uint8 value) {
  regs.bg_vofs[BG2] = (value << 8) | (regs.bg_ofslatch);
  regs.bg_ofslatch  = value;
}

//BG3HOFS
void bPPU::mmio_w2111(uint8 value) {
  regs.bg_hofs[BG3] = (value << 8) | (regs.bg_ofslatch & ~7) | ((regs.bg_hofs[BG3] >> 8) & 7);
  regs.bg_ofslatch  = value;
}

//BG3VOFS
void bPPU::mmio_w2112(uint8 value) {
  regs.bg_vofs[BG3] = (value << 8) | (regs.bg_ofslatch);
  regs.bg_ofslatch  = value;
}

//BG4HOFS
void bPPU::mmio_w2113(uint8 value) {
  regs.bg_hofs[BG4] = (value << 8) | (regs.bg_ofslatch & ~7) | ((regs.bg_hofs[BG4] >> 8) & 7);
  regs.bg_ofslatch  = value;
}

//BG4VOFS
void bPPU::mmio_w2114(uint8 value) {
  regs.bg_vofs[BG4] = (value << 8) | (regs.bg_ofslatch);
  regs.bg_ofslatch  = value;
}

//VMAIN
void bPPU::mmio_w2115(uint8 value) {
  regs.vram_incmode = !!(value & 0x80);
  regs.vram_mapping = (value >> 2) & 3;
  switch(value & 3) {
  case 0: regs.vram_incsize =   1; break;
  case 1: regs.vram_incsize =  32; break;
  case 2: regs.vram_incsize = 128; break;
  case 3: regs.vram_incsize = 128; break;
  }
}

//VMADDL
void bPPU::mmio_w2116(uint8 value) {
  regs.vram_addr = (regs.vram_addr & 0xff00) | value;
uint16 addr = get_vram_address();
  regs.vram_readbuffer  = vram_mmio_read(addr + 0);
  regs.vram_readbuffer |= vram_mmio_read(addr + 1) << 8;
}

//VMADDH
void bPPU::mmio_w2117(uint8 value) {
  regs.vram_addr = (value << 8) | (regs.vram_addr & 0x00ff);
uint16 addr = get_vram_address();
  regs.vram_readbuffer  = vram_mmio_read(addr + 0);
  regs.vram_readbuffer |= vram_mmio_read(addr + 1) << 8;
}

//VMDATAL
void bPPU::mmio_w2118(uint8 value) {
uint16 addr = get_vram_address();
  vram_mmio_write(addr, value);
  bg_tiledata_state[TILE_2BIT][(addr >> 4)] = 1;
  bg_tiledata_state[TILE_4BIT][(addr >> 5)] = 1;
  bg_tiledata_state[TILE_8BIT][(addr >> 6)] = 1;

  if(regs.vram_incmode == 0) {
    regs.vram_addr += regs.vram_incsize;
  }
}

//VMDATAH
void bPPU::mmio_w2119(uint8 value) {
uint16 addr = get_vram_address() + 1;
  vram_mmio_write(addr, value);
  bg_tiledata_state[TILE_2BIT][(addr >> 4)] = 1;
  bg_tiledata_state[TILE_4BIT][(addr >> 5)] = 1;
  bg_tiledata_state[TILE_8BIT][(addr >> 6)] = 1;

  if(regs.vram_incmode == 1) {
    regs.vram_addr += regs.vram_incsize;
  }
}

//M7SEL
void bPPU::mmio_w211a(uint8 value) {
  regs.mode7_repeat = (value >> 6) & 3;
  regs.mode7_vflip  = !!(value & 0x02);
  regs.mode7_hflip  = !!(value & 0x01);
}

//M7A
void bPPU::mmio_w211b(uint8 value) {
  regs.m7a      = (value << 8) | regs.m7_latch;
  regs.m7_latch = value;
}

//M7B
void bPPU::mmio_w211c(uint8 value) {
  regs.m7b      = (value << 8) | regs.m7_latch;
  regs.m7_latch = value;
}

//M7C
void bPPU::mmio_w211d(uint8 value) {
  regs.m7c      = (value << 8) | regs.m7_latch;
  regs.m7_latch = value;
}

//M7D
void bPPU::mmio_w211e(uint8 value) {
  regs.m7d      = (value << 8) | regs.m7_latch;
  regs.m7_latch = value;
}

//M7X
void bPPU::mmio_w211f(uint8 value) {
  regs.m7x      = (value << 8) | regs.m7_latch;
  regs.m7_latch = value;
}

//M7Y
void bPPU::mmio_w2120(uint8 value) {
  regs.m7y      = (value << 8) | regs.m7_latch;
  regs.m7_latch = value;
}

//CGADD
void bPPU::mmio_w2121(uint8 value) {
  regs.cgram_addr = value << 1;
}

//CGDATA
//note: CGRAM palette data format is 15-bits
//(0,bbbbb,ggggg,rrrrr). Highest bit is ignored,
//as evidenced by $213b CGRAM data reads.
//
//anomie indicates writes to CGDATA work the same
//as writes to OAMDATA's low table. need to verify
//this on hardware.
void bPPU::mmio_w2122(uint8 value) {
  if(!(regs.cgram_addr & 1)) {
    regs.cgram_latchdata = value;
  } else {
    cgram_mmio_write((regs.cgram_addr & 0x01fe),     regs.cgram_latchdata);
    cgram_mmio_write((regs.cgram_addr & 0x01fe) + 1, value & 0x7f);
  }
  regs.cgram_addr++;
  regs.cgram_addr &= 0x01ff;
}

//W12SEL
void bPPU::mmio_w2123(uint8 value) {
  regs.window2_enabled[BG2] = !!(value & 0x80);
  regs.window2_invert [BG2] = !!(value & 0x40);
  regs.window1_enabled[BG2] = !!(value & 0x20);
  regs.window1_invert [BG2] = !!(value & 0x10);
  regs.window2_enabled[BG1] = !!(value & 0x08);
  regs.window2_invert [BG1] = !!(value & 0x04);
  regs.window1_enabled[BG1] = !!(value & 0x02);
  regs.window1_invert [BG1] = !!(value & 0x01);
}

//W34SEL
void bPPU::mmio_w2124(uint8 value) {
  regs.window2_enabled[BG4] = !!(value & 0x80);
  regs.window2_invert [BG4] = !!(value & 0x40);
  regs.window1_enabled[BG4] = !!(value & 0x20);
  regs.window1_invert [BG4] = !!(value & 0x10);
  regs.window2_enabled[BG3] = !!(value & 0x08);
  regs.window2_invert [BG3] = !!(value & 0x04);
  regs.window1_enabled[BG3] = !!(value & 0x02);
  regs.window1_invert [BG3] = !!(value & 0x01);
}

//WOBJSEL
void bPPU::mmio_w2125(uint8 value) {
  regs.window2_enabled[COL] = !!(value & 0x80);
  regs.window2_invert [COL] = !!(value & 0x40);
  regs.window1_enabled[COL] = !!(value & 0x20);
  regs.window1_invert [COL] = !!(value & 0x10);
  regs.window2_enabled[OAM] = !!(value & 0x08);
  regs.window2_invert [OAM] = !!(value & 0x04);
  regs.window1_enabled[OAM] = !!(value & 0x02);
  regs.window1_invert [OAM] = !!(value & 0x01);
}

//WH0
void bPPU::mmio_w2126(uint8 value) {
  regs.window1_left = value;
}

//WH1
void bPPU::mmio_w2127(uint8 value) {
  regs.window1_right = value;
}

//WH2
void bPPU::mmio_w2128(uint8 value) {
  regs.window2_left = value;
}

//WH3
void bPPU::mmio_w2129(uint8 value) {
  regs.window2_right = value;
}

//WBGLOG
void bPPU::mmio_w212a(uint8 value) {
  regs.window_mask[BG4] = (value >> 6) & 3;
  regs.window_mask[BG3] = (value >> 4) & 3;
  regs.window_mask[BG2] = (value >> 2) & 3;
  regs.window_mask[BG1] = (value     ) & 3;
}

//WOBJLOG
void bPPU::mmio_w212b(uint8 value) {
  regs.window_mask[COL] = (value >> 2) & 3;
  regs.window_mask[OAM] = (value     ) & 3;
}

//TM
void bPPU::mmio_w212c(uint8 value) {
  regs.bg_enabled[OAM] = !!(value & 0x10);
  regs.bg_enabled[BG4] = !!(value & 0x08);
  regs.bg_enabled[BG3] = !!(value & 0x04);
  regs.bg_enabled[BG2] = !!(value & 0x02);
  regs.bg_enabled[BG1] = !!(value & 0x01);
}

//TS
void bPPU::mmio_w212d(uint8 value) {
  regs.bgsub_enabled[OAM] = !!(value & 0x10);
  regs.bgsub_enabled[BG4] = !!(value & 0x08);
  regs.bgsub_enabled[BG3] = !!(value & 0x04);
  regs.bgsub_enabled[BG2] = !!(value & 0x02);
  regs.bgsub_enabled[BG1] = !!(value & 0x01);
}

//TMW
void bPPU::mmio_w212e(uint8 value) {
  regs.window_enabled[OAM] = !!(value & 0x10);
  regs.window_enabled[BG4] = !!(value & 0x08);
  regs.window_enabled[BG3] = !!(value & 0x04);
  regs.window_enabled[BG2] = !!(value & 0x02);
  regs.window_enabled[BG1] = !!(value & 0x01);
}

//TSW
void bPPU::mmio_w212f(uint8 value) {
  regs.sub_window_enabled[OAM] = !!(value & 0x10);
  regs.sub_window_enabled[BG4] = !!(value & 0x08);
  regs.sub_window_enabled[BG3] = !!(value & 0x04);
  regs.sub_window_enabled[BG2] = !!(value & 0x02);
  regs.sub_window_enabled[BG1] = !!(value & 0x01);
}

//CGWSEL
void bPPU::mmio_w2130(uint8 value) {
  regs.color_mask    = (value >> 6) & 3;
  regs.colorsub_mask = (value >> 4) & 3;
  regs.addsub_mode   = !!(value & 0x02);
  regs.direct_color  = !!(value & 0x01);
}

//CGADDSUB
void bPPU::mmio_w2131(uint8 value) {
  regs.color_mode          = !!(value & 0x80);
  regs.color_halve         = !!(value & 0x40);
  regs.color_enabled[BACK] = !!(value & 0x20);
  regs.color_enabled[OAM]  = !!(value & 0x10);
  regs.color_enabled[BG4]  = !!(value & 0x08);
  regs.color_enabled[BG3]  = !!(value & 0x04);
  regs.color_enabled[BG2]  = !!(value & 0x02);
  regs.color_enabled[BG1]  = !!(value & 0x01);
}

//COLDATA
void bPPU::mmio_w2132(uint8 value) {
  if(value & 0x80) regs.color_b = value & 0x1f;
  if(value & 0x40) regs.color_g = value & 0x1f;
  if(value & 0x20) regs.color_r = value & 0x1f;

  regs.color_rgb = (regs.color_r)
                 | (regs.color_g << 5)
                 | (regs.color_b << 10);
}

//SETINI
void bPPU::mmio_w2133(uint8 value) {
  regs.mode7_extbg   = !!(value & 0x40);
  regs.pseudo_hires  = !!(value & 0x08);
  regs.overscan      = !!(value & 0x04);
  regs.oam_interlace = !!(value & 0x02);
  regs.interlace     = !!(value & 0x01);

  display.overscan = regs.overscan;
}

//MPYL
uint8 bPPU::mmio_r2134() {
uint32 r;
  r = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
  regs.ppu1_mdr = r;
  return regs.ppu1_mdr;
}

//MPYM
uint8 bPPU::mmio_r2135() {
uint32 r;
  r = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
  regs.ppu1_mdr = r >> 8;
  return regs.ppu1_mdr;
}

//MPYH
uint8 bPPU::mmio_r2136() {
uint32 r;
  r = ((int16)regs.m7a * (int8)(regs.m7b >> 8));
  regs.ppu1_mdr = r >> 16;
  return regs.ppu1_mdr;
}

//SLHV
uint8 bPPU::mmio_r2137() {
  if(cpu.pio() & 0x80) {
    latch_counters();
  }
  return cpu.regs.mdr;
}

//OAMDATAREAD
uint8 bPPU::mmio_r2138() {
  regs.ppu1_mdr = oam_mmio_read(regs.oam_addr);

  regs.oam_addr++;
  regs.oam_addr &= 0x03ff;
  regs.oam_firstsprite = (regs.oam_priority == false) ? 0 : (regs.oam_addr >> 2) & 127;

  return regs.ppu1_mdr;
}

//VMDATALREAD
uint8 bPPU::mmio_r2139() {
uint16 addr = get_vram_address();
  regs.ppu1_mdr = regs.vram_readbuffer;
  if(regs.vram_incmode == 0) {
    addr &= 0xfffe;
    regs.vram_readbuffer  = vram_mmio_read(addr + 0);
    regs.vram_readbuffer |= vram_mmio_read(addr + 1) << 8;
    regs.vram_addr += regs.vram_incsize;
  }
  return regs.ppu1_mdr;
}

//VMDATAHREAD
uint8 bPPU::mmio_r213a() {
uint16 addr = get_vram_address() + 1;
  regs.ppu1_mdr = regs.vram_readbuffer >> 8;
  if(regs.vram_incmode == 1) {
    addr &= 0xfffe;
    regs.vram_readbuffer  = vram_mmio_read(addr + 0);
    regs.vram_readbuffer |= vram_mmio_read(addr + 1) << 8;
    regs.vram_addr += regs.vram_incsize;
  }
  return regs.ppu1_mdr;
}

//CGDATAREAD
//note: CGRAM palette data is 15-bits (0,bbbbb,ggggg,rrrrr)
//therefore, the high byte read from each color does not
//update bit 7 of the PPU2 MDR.
uint8 bPPU::mmio_r213b() {
  if(!(regs.cgram_addr & 1)) {
    regs.ppu2_mdr  = cgram_mmio_read(regs.cgram_addr) & 0xff;
  } else {
    regs.ppu2_mdr &= 0x80;
    regs.ppu2_mdr |= cgram_mmio_read(regs.cgram_addr) & 0x7f;
  }
  regs.cgram_addr++;
  regs.cgram_addr &= 0x01ff;
  return regs.ppu2_mdr;
}

//OPHCT
uint8 bPPU::mmio_r213c() {
  if(!regs.latch_hcounter) {
    regs.ppu2_mdr  = regs.hcounter & 0xff;
  } else {
    regs.ppu2_mdr &= 0xfe;
    regs.ppu2_mdr |= (regs.hcounter >> 8) & 1;
  }
  regs.latch_hcounter ^= 1;
  return regs.ppu2_mdr;
}

//OPVCT
uint8 bPPU::mmio_r213d() {
  if(!regs.latch_vcounter) {
    regs.ppu2_mdr  = regs.vcounter & 0xff;
  } else {
    regs.ppu2_mdr &= 0xfe;
    regs.ppu2_mdr |= (regs.vcounter >> 8) & 1;
  }
  regs.latch_vcounter ^= 1;
  return regs.ppu2_mdr;
}

//STAT77
uint8 bPPU::mmio_r213e() {
uint8 r = 0x00;
  r |= (regs.time_over)  ? 0x80 : 0x00;
  r |= (regs.range_over) ? 0x40 : 0x00;
  r |= (regs.ppu1_mdr & 0x10);
  r |= (ppu1_version & 0x0f);
  regs.ppu1_mdr = r;
  return regs.ppu1_mdr;
}

//STAT78
uint8 bPPU::mmio_r213f() {
uint8 r = 0x00;
  regs.latch_hcounter = 0;
  regs.latch_vcounter = 0;

  r |= field() << 7;
  if(!(cpu.pio() & 0x80)) {
    r |= 0x40;
  } else if(regs.counters_latched == true) {
    r |= 0x40;
    regs.counters_latched = false;
  }
  r |= (regs.ppu2_mdr & 0x20);
  r |= (region << 4); //0 = NTSC, 1 = PAL
  r |= (ppu2_version & 0x0f);
  regs.ppu2_mdr = r;
  return regs.ppu2_mdr;
}

uint8 bPPU::mmio_read(unsigned addr) {
  scheduler.sync_cpuppu();

  switch(addr & 0xffff) {
    case 0x2104:
    case 0x2105:
    case 0x2106:
    case 0x2108:
    case 0x2109:
    case 0x210a:
    case 0x2114:
    case 0x2115:
    case 0x2116:
    case 0x2118:
    case 0x2119:
    case 0x211a:
    case 0x2124:
    case 0x2125:
    case 0x2126:
    case 0x2128:
    case 0x2129:
    case 0x212a: return regs.ppu1_mdr;
    case 0x2134: return mmio_r2134(); //MPYL
    case 0x2135: return mmio_r2135(); //MPYM
    case 0x2136: return mmio_r2136(); //MPYH
    case 0x2137: return mmio_r2137(); //SLHV
    case 0x2138: return mmio_r2138(); //OAMDATAREAD
    case 0x2139: return mmio_r2139(); //VMDATALREAD
    case 0x213a: return mmio_r213a(); //VMDATAHREAD
    case 0x213b: return mmio_r213b(); //CGDATAREAD
    case 0x213c: return mmio_r213c(); //OPHCT
    case 0x213d: return mmio_r213d(); //OPVCT
    case 0x213e: return mmio_r213e(); //STAT77
    case 0x213f: return mmio_r213f(); //STAT78
  }

  //return 0x00;
  return cpu.regs.mdr;
}

void bPPU::mmio_write(unsigned addr, uint8 data) {
  scheduler.sync_cpuppu();

  switch(addr & 0xffff) {
    case 0x2100: mmio_w2100(data); return; //INIDISP
    case 0x2101: mmio_w2101(data); return; //OBSEL
    case 0x2102: mmio_w2102(data); return; //OAMADDL
    case 0x2103: mmio_w2103(data); return; //OAMADDH
    case 0x2104: mmio_w2104(data); return; //OAMDATA
    case 0x2105: mmio_w2105(data); return; //BGMODE
    case 0x2106: mmio_w2106(data); return; //MOSAIC
    case 0x2107: mmio_w2107(data); return; //BG1SC
    case 0x2108: mmio_w2108(data); return; //BG2SC
    case 0x2109: mmio_w2109(data); return; //BG3SC
    case 0x210a: mmio_w210a(data); return; //BG4SC
    case 0x210b: mmio_w210b(data); return; //BG12NBA
    case 0x210c: mmio_w210c(data); return; //BG34NBA
    case 0x210d: mmio_w210d(data); return; //BG1HOFS
    case 0x210e: mmio_w210e(data); return; //BG1VOFS
    case 0x210f: mmio_w210f(data); return; //BG2HOFS
    case 0x2110: mmio_w2110(data); return; //BG2VOFS
    case 0x2111: mmio_w2111(data); return; //BG3HOFS
    case 0x2112: mmio_w2112(data); return; //BG3VOFS
    case 0x2113: mmio_w2113(data); return; //BG4HOFS
    case 0x2114: mmio_w2114(data); return; //BG4VOFS
    case 0x2115: mmio_w2115(data); return; //VMAIN
    case 0x2116: mmio_w2116(data); return; //VMADDL
    case 0x2117: mmio_w2117(data); return; //VMADDH
    case 0x2118: mmio_w2118(data); return; //VMDATAL
    case 0x2119: mmio_w2119(data); return; //VMDATAH
    case 0x211a: mmio_w211a(data); return; //M7SEL
    case 0x211b: mmio_w211b(data); return; //M7A
    case 0x211c: mmio_w211c(data); return; //M7B
    case 0x211d: mmio_w211d(data); return; //M7C
    case 0x211e: mmio_w211e(data); return; //M7D
    case 0x211f: mmio_w211f(data); return; //M7X
    case 0x2120: mmio_w2120(data); return; //M7Y
    case 0x2121: mmio_w2121(data); return; //CGADD
    case 0x2122: mmio_w2122(data); return; //CGDATA
    case 0x2123: mmio_w2123(data); return; //W12SEL
    case 0x2124: mmio_w2124(data); return; //W34SEL
    case 0x2125: mmio_w2125(data); return; //WOBJSEL
    case 0x2126: mmio_w2126(data); return; //WH0
    case 0x2127: mmio_w2127(data); return; //WH1
    case 0x2128: mmio_w2128(data); return; //WH2
    case 0x2129: mmio_w2129(data); return; //WH3
    case 0x212a: mmio_w212a(data); return; //WBGLOG
    case 0x212b: mmio_w212b(data); return; //WOBJLOG
    case 0x212c: mmio_w212c(data); return; //TM
    case 0x212d: mmio_w212d(data); return; //TS
    case 0x212e: mmio_w212e(data); return; //TMW
    case 0x212f: mmio_w212f(data); return; //TSW
    case 0x2130: mmio_w2130(data); return; //CGWSEL
    case 0x2131: mmio_w2131(data); return; //CGADDSUB
    case 0x2132: mmio_w2132(data); return; //COLDATA
    case 0x2133: mmio_w2133(data); return; //SETINI
  }
}

#endif  //ifdef BPPU_CPP
