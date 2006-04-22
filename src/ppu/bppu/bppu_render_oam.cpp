void bPPU::build_sprite_list() {
uint8 *tableA = oam, *tableB = oam + 512;

  for(int i = 0; i < 128; i++) {
  uint x    = bool(*tableB & (1 << ((i & 3) << 1))); //0x01, 0x04, 0x10, 0x40
  bool size = bool(*tableB & (2 << ((i & 3) << 1))); //0x02, 0x08, 0x20, 0x80

    switch(regs.oam_basesize) {
    case 0: sprite_list[i].width  = (!size) ?  8 : 16;
            sprite_list[i].height = (!size) ?  8 : 16;
            break;
    case 1: sprite_list[i].width  = (!size) ?  8 : 32;
            sprite_list[i].height = (!size) ?  8 : 32;
            break;
    case 2: sprite_list[i].width  = (!size) ?  8 : 64;
            sprite_list[i].height = (!size) ?  8 : 64;
            break;
    case 3: sprite_list[i].width  = (!size) ? 16 : 32;
            sprite_list[i].height = (!size) ? 16 : 32;
            break;
    case 4: sprite_list[i].width  = (!size) ? 16 : 64;
            sprite_list[i].height = (!size) ? 16 : 64;
            break;
    case 5: sprite_list[i].width  = (!size) ? 32 : 64;
            sprite_list[i].height = (!size) ? 32 : 64;
            break;
    case 6: sprite_list[i].width  = (!size) ? 16 : 32;
            sprite_list[i].height = (!size) ? 32 : 64;
            break;
    case 7: sprite_list[i].width  = (!size) ? 16 : 32;
            sprite_list[i].height = (!size) ? 32 : 32;
            break;
    }

    sprite_list[i].x              = (x << 8) + tableA[0];
    sprite_list[i].y              = tableA[1] + 1;
    sprite_list[i].character      = tableA[2];
    sprite_list[i].vflip          = bool(tableA[3] & 0x80);
    sprite_list[i].hflip          = bool(tableA[3] & 0x40);
    sprite_list[i].priority       = (tableA[3] >> 4) & 3;
    sprite_list[i].palette        = (tableA[3] >> 1) & 7;
    sprite_list[i].use_nameselect = tableA[3] & 1;

    tableA += 4;
    if((i & 3) == 3)tableB++;
  }
}

bool bPPU::is_sprite_on_scanline() {
//if sprite is entirely offscreen and doesn't wrap around to the left side of the screen,
//then it is not counted. 256 is correct, and not 255 -- as one might first expect
  if(spr->x > 256 && (spr->x + spr->width) < 512)return false;

  if(regs.oam_interlace == false) {
    if(line.y >= spr->y && line.y < (spr->y + spr->height)) {
      return true;
    } else if((spr->y + spr->height) >= 256 && line.y < ((spr->y + spr->height) & 255)) {
      return true;
    }
  } else {
    if(line.y >= spr->y && line.y < (spr->y + (spr->height >> 1))) {
      return true;
    } else if((spr->y + (spr->height >> 1)) >= 256 && line.y < ((spr->y + (spr->height >> 1)) & 255)) {
      return true;
    }
  }

  return false;
}

void bPPU::load_oam_tiles() {
uint16 tile_width = spr->width >> 3;
int x = spr->x;
int y = (spr->vflip) ? ((spr->height - 1) - (line.y - spr->y)) : (line.y - spr->y);

//todo: verify below code is hardware-accurate
//specifically, is interlace_field added for oam_interlace mode?
  if(regs.oam_interlace == true) {
    y <<= 1;
    if(line.interlace && line.width == 512) {
      y += line.interlace_field;
    }
  }

  x &= 511;
  y &= 255;

uint16 tdaddr = regs.oam_tdaddr;
uint16 chrx   = (spr->character     ) & 15;
uint16 chry   = (spr->character >> 4) & 15;
  if(spr->use_nameselect == true) {
    tdaddr += (256 * 32) + (regs.oam_nameselect << 13);
  }
  chry  += (y >> 3);
  chry  &= 15;
  chry <<= 4;

int i, n, sx, mx, pos;
  for(i=0;i<tile_width;i++) {
    sx  = x;
    sx += i << 3;
    sx &= 511;

  //ignore sprites that are offscreen
  //sprites at 256 are still counted, even though they aren't visible onscreen
    if(sx >= 257 && (sx + 7) < 512)continue;

    if(regs.oam_tilecount++ > 34)break;
    n = regs.oam_tilecount - 1;
    oam_tilelist[n].x     = sx;
    oam_tilelist[n].y     = y;
    oam_tilelist[n].pri   = spr->priority;
    oam_tilelist[n].pal   = (spr->palette << 4) + 128;
    oam_tilelist[n].hflip = spr->hflip;

    mx  = (oam_tilelist[n].hflip) ? ((tile_width - 1) - i) : i;
    pos = tdaddr + ((chry + ((chrx + mx) & 15)) << 5);
    oam_tilelist[n].tile = (pos >> 5) & 0x07ff;
  }
}

void bPPU::render_oam_tile(int tile_num) {
uint8 *oam_td, *oam_td_state, *tile_ptr;
oam_tileitem *t = &oam_tilelist[tile_num];
  oam_td       = (uint8*)bg_tiledata[COLORDEPTH_16];
  oam_td_state = (uint8*)bg_tiledata_state[COLORDEPTH_16];

  if(oam_td_state[t->tile] == 1) {
    render_bg_tile(COLORDEPTH_16, t->tile);
  }

int x, sx, col;
  sx = t->x;
  tile_ptr = (uint8*)oam_td + (t->tile << 6) + ((t->y & 7) << 3);
  for(x=0;x<8;x++) {
    sx &= 511;
    if(sx < 256) {
      col = *(tile_ptr + ((t->hflip) ? (7 - x) : x));
      if(col) {
        col += t->pal;
        oam_line_pal[sx] = col;
        oam_line_pri[sx] = t->pri;
      }
    }
    sx++;
  }
}

void bPPU::render_line_oam(uint8 pri0_pos, uint8 pri1_pos, uint8 pri2_pos, uint8 pri3_pos) {
  build_sprite_list();

  regs.oam_itemcount = 0;
  regs.oam_tilecount = 0;
  memset(oam_line_pri, OAM_PRI_NONE, 256);

  memset(oam_itemlist, 0xff, 32);
  for(int s = 0; s < 128; s++) {
    spr = &sprite_list[(s + regs.oam_firstsprite) & 127];
    if(is_sprite_on_scanline() == false)continue;
    if(regs.oam_itemcount++ > 32)break;
    oam_itemlist[regs.oam_itemcount - 1] = (s + regs.oam_firstsprite) & 127;
  }

  for(int s = 0; s < 34; s++)oam_tilelist[s].tile = 0xffff;

  for(int s = 31; s >= 0; s--) {
    if(oam_itemlist[s] == 0xff)continue;
    spr = &sprite_list[oam_itemlist[s]];
    load_oam_tiles();
  }

  for(int s = 0; s < 34; s++) {
    if(oam_tilelist[s].tile == 0xffff)continue;
    render_oam_tile(s);
  }

  regs.time_over  |= (regs.oam_tilecount > 34);
  regs.range_over |= (regs.oam_itemcount > 32);

  if(regs.bg_enabled[OAM] == false && regs.bgsub_enabled[OAM] == false)return;

//are layers disabled by user?
  if(render_enabled(OAM, 0) == false)pri0_pos = 0;
  if(render_enabled(OAM, 1) == false)pri1_pos = 0;
  if(render_enabled(OAM, 2) == false)pri2_pos = 0;
  if(render_enabled(OAM, 3) == false)pri3_pos = 0;
//nothing to render?
  if(!pri0_pos && !pri1_pos && !pri2_pos && !pri3_pos)return;

  render_line_oam_lores(pri0_pos, pri1_pos, pri2_pos, pri3_pos);
}

#define setpixel_main(x) \
  if(pixel_cache[x].pri_main < pri) { \
    pixel_cache[x].pri_main = pri; \
    pixel_cache[x].bg_main  = OAM; \
    pixel_cache[x].src_main = get_palette(oam_line_pal[x]); \
    pixel_cache[x].ce_main  = (oam_line_pal[x] < 192); \
  }
#define setpixel_sub(x) \
  if(pixel_cache[x].pri_sub < pri) { \
    pixel_cache[x].pri_sub = pri; \
    pixel_cache[x].bg_sub  = OAM; \
    pixel_cache[x].src_sub = get_palette(oam_line_pal[x]); \
    pixel_cache[x].ce_sub  = (oam_line_pal[x] < 192); \
  }

void bPPU::render_line_oam_lores(uint8 pri0_pos, uint8 pri1_pos, uint8 pri2_pos, uint8 pri3_pos) {
bool bg_enabled    = regs.bg_enabled[OAM];
bool bgsub_enabled = regs.bgsub_enabled[OAM];

  build_window_tables(OAM);
uint8 *wt_main = window[OAM].main;
uint8 *wt_sub  = window[OAM].sub;

int pri;
  for(int x = 0; x < 256; x++) {
    if(oam_line_pri[x] == OAM_PRI_NONE)continue;

    switch(oam_line_pri[x]) {
    case 0: pri = pri0_pos; break;
    case 1: pri = pri1_pos; break;
    case 2: pri = pri2_pos; break;
    case 3: pri = pri3_pos; break;
    }

    if(bg_enabled    == true && !wt_main[x]) { setpixel_main(x); }
    if(bgsub_enabled == true && !wt_sub[x])  { setpixel_sub(x); }
  }
}

#undef setpixel_main
#undef setpixel_sub
