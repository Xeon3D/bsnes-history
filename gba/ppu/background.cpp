void PPU::render_backgrounds() {
  switch(regs.control.bgmode) {
  case 0:
    render_background_linear(3);
    render_background_linear(2);
    render_background_linear(1);
    render_background_linear(0);
    break;
  case 1:
    render_background_affine(2);
    render_background_linear(1);
    render_background_linear(0);
    break;
  case 2:
    render_background_affine(3);
    render_background_affine(2);
    break;
  case 3: case 4: case 5:
    render_background_bitmap(2);
    break;
  }
}

void PPU::render_background_linear(unsigned bgnumber) {
  if(regs.control.enablebg[bgnumber] == false) return;
  auto &bg = regs.bg[bgnumber];

  uint9 voffset = regs.vcounter + bg.voffset;
  uint9 hoffset = bg.hoffset;

  unsigned basemap = bg.control.screenbaseblock    << 11;
  unsigned basechr = bg.control.characterbaseblock << 14;
  unsigned px = hoffset & 7, py = voffset & 7;

  Tile tile;
  uint8 data[8];

  for(unsigned x = 0; x < 240; x++) {
    if(x == 0 || px & 8) {
      px &= 7;

      unsigned tx = hoffset / 8, ty = voffset / 8;
      unsigned offset = (ty & 31) * 32 + (tx & 31);
      if(bg.control.screensize & 1) if(tx & 32) offset += 32 * 32;
      if(bg.control.screensize & 2) if(ty & 32) offset += 32 * 32 * (1 + (bg.control.screensize & 1));
      offset = basemap + offset * 2;
      uint16 mapdata = vram_read(offset, Half);

      tile.character = mapdata >>  0;
      tile.hflip     = mapdata >> 10;
      tile.vflip     = mapdata >> 11;
      tile.palette   = mapdata >> 12;

      if(bg.control.colormode == 0) {
        offset = basechr + tile.character * 32 + (py ^ (tile.vflip ? 7 : 0)) * 4;
        uint32 word = vram_read(offset, Word);
        for(unsigned n = 0; n < 8; n++) data[n] = (word >> (n * 4)) & 15;
      } else {
        offset = basechr + tile.character * 64 + (py ^ (tile.vflip ? 7 : 0)) * 8;
        uint32 wordlo = vram_read(offset + 0, Word);
        uint32 wordhi = vram_read(offset + 4, Word);
        for(unsigned n = 0; n < 4; n++) data[0 + n] = (wordlo >> (n * 8)) & 255;
        for(unsigned n = 0; n < 4; n++) data[4 + n] = (wordhi >> (n * 8)) & 255;
      }
    }

    hoffset++;
    uint8 color = data[px++ ^ (tile.hflip ? 7 : 0)];

    if(color) {
      if(bg.control.colormode == 0) layer[bg.control.priority][x] = { true, pram[tile.palette * 16 + color] };
      if(bg.control.colormode == 1) layer[bg.control.priority][x] = { true, pram[color] };
    }
  }
}

void PPU::render_background_affine(unsigned bgnumber) {
  if(regs.control.enablebg[bgnumber] == false) return;
  auto &bg = regs.bg[bgnumber];

  unsigned basemap = bg.control.screenbaseblock    << 11;
  unsigned basechr = bg.control.characterbaseblock << 14;
  unsigned screensize = 16 << bg.control.screensize;
  unsigned screenwrap = (1 << (bg.control.affinewrap ? 7 + bg.control.screensize : 20)) - 1;

  int28 fx = bg.lx;
  int28 fy = bg.ly;

  for(unsigned x = 0; x < 240; x++) {
    unsigned cx = (fx >> 8) & screenwrap, tx = cx / 8, px = cx & 7;
    unsigned cy = (fy >> 8) & screenwrap, ty = cy / 8, py = cy & 7;

    if(tx < screensize && ty < screensize) {
      uint8 character = vram[basemap + ty * screensize + tx];
      uint8 color = vram[basechr + (character * 64) + py * 8 + px];
      if(color) layer[bg.control.priority][x] = { true, pram[color] };
    }

    fx += bg.pa;
    fy += bg.pc;
  }

  bg.lx += bg.pb;
  bg.ly += bg.pd;
}

void PPU::render_background_bitmap(unsigned bgnumber) {
  if(regs.control.enablebg[bgnumber] == false) return;
  auto &bg = regs.bg[bgnumber];

  uint1 depth = regs.control.bgmode != 4;  //0 = 8-bit (Mode 4), 1 = 15-bit (Mode 3, Mode 5)
  unsigned basemap = regs.control.bgmode == 3 ? 0 : 0xa000 * regs.control.frame;

  unsigned width  = regs.control.bgmode == 5 ? 160 : 240;
  unsigned height = regs.control.bgmode == 5 ? 128 : 160;
  unsigned size   = depth ? Half : Byte;

  int28 fx = bg.lx;
  int28 fy = bg.ly;

  for(unsigned x = 0; x < 240; x++) {
    unsigned px = fx >> 8;
    unsigned py = fy >> 8;

    if(px < width && py < height) {
      unsigned offset = py * width + px;
      unsigned color  = vram_read(basemap + (offset << depth), size);

      if(depth || color) {  //8bpp color 0 is transparent; 15bpp color is always opaque
        if(depth == 0) color = pram[color];
        if(depth == 1) color = color & 0x7fff;
        layer[bg.control.priority][x] = { true, color };
      }
    }

    fx += bg.pa;
    fy += bg.pc;
  }

  bg.lx += bg.pb;
  bg.ly += bg.pd;
}
