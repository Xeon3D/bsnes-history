#ifdef BPPU_CPP

#include "bppu_render_cache.cpp"
#include "bppu_render_windows.cpp"
#include "bppu_render_bg.cpp"
#include "bppu_render_oam.cpp"
#include "bppu_render_mode7.cpp"
#include "bppu_render_addsub.cpp"
#include "bppu_render_line.cpp"

bool bPPU::render_enabled(uint8 bg, uint8 pri) {
  switch(bg) {
  case BG1:
    switch(pri) {
    case 0: return config::ppu.bg1_pri0_enable;
    case 1: return config::ppu.bg1_pri1_enable;
    } break;
  case BG2:
    switch(pri) {
    case 0: return config::ppu.bg2_pri0_enable;
    case 1: return config::ppu.bg2_pri1_enable;
    } break;
  case BG3:
    switch(pri) {
    case 0: return config::ppu.bg3_pri0_enable;
    case 1: return config::ppu.bg3_pri1_enable;
    } break;
  case BG4:
    switch(pri) {
    case 0: return config::ppu.bg4_pri0_enable;
    case 1: return config::ppu.bg4_pri1_enable;
    } break;
  case OAM:
    switch(pri) {
    case 0: return config::ppu.oam_pri0_enable;
    case 1: return config::ppu.oam_pri1_enable;
    case 2: return config::ppu.oam_pri2_enable;
    case 3: return config::ppu.oam_pri3_enable;
    } break;
  }

  return true;
}

/*
Mode 0: ->
     1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12
  BG4B, BG3B, OAM0, BG4A, BG3A, OAM1, BG2B, BG1B, OAM2, BG2A, BG1A, OAM3
*/
void bPPU::render_line_mode0() {
  render_line_bg(BG1, COLORDEPTH_4, 8, 11);
  render_line_bg(BG2, COLORDEPTH_4, 7, 10);
  render_line_bg(BG3, COLORDEPTH_4, 2,  5);
  render_line_bg(BG4, COLORDEPTH_4, 1,  4);
  render_line_oam(3, 6, 9, 12);
}

/*
Mode 1 (pri=1): ->
     1,    2,    3,    4,    5,    6,    7,    8,    9,   10
  BG3B, OAM0, OAM1, BG2B, BG1B, OAM2, BG2A, BG1A, OAM3, BG3A

Mode 1 (pri=0): ->
     1,    2,    3,    4,    5,    6,    7,    8,    9,   10
  BG3B, OAM0, BG3A, OAM1, BG2B, BG1B, OAM2, BG2A, BG1A, OAM3
*/
void bPPU::render_line_mode1() {
  if(regs.bg3_priority) {
    render_line_bg(BG1, COLORDEPTH_16, 5,  8);
    render_line_bg(BG2, COLORDEPTH_16, 4,  7);
    render_line_bg(BG3, COLORDEPTH_4,  1, 10);
    render_line_oam(2, 3, 6, 9);
  } else {
    render_line_bg(BG1, COLORDEPTH_16, 6,  9);
    render_line_bg(BG2, COLORDEPTH_16, 5,  8);
    render_line_bg(BG3, COLORDEPTH_4,  1,  3);
    render_line_oam(2, 4, 7, 10);
  }
}

/*
Mode 2: ->
     1,    2,    3,    4,    5,    6,    7,    8
  BG2B, OAM0, BG1B, OAM1, BG2A, OAM2, BG1A, OAM3
*/
void bPPU::render_line_mode2() {
  render_line_bg(BG1, COLORDEPTH_16, 3, 7);
  render_line_bg(BG2, COLORDEPTH_16, 1, 5);
  render_line_oam(2, 4, 6, 8);
}

/*
Mode 3: ->
     1,    2,    3,    4,    5,    6,    7,    8
  BG2B, OAM0, BG1B, OAM1, BG2A, OAM2, BG1A, OAM3
*/
void bPPU::render_line_mode3() {
  render_line_bg(BG1, COLORDEPTH_256, 3, 7);
  render_line_bg(BG2, COLORDEPTH_16,  1, 5);
  render_line_oam(2, 4, 6, 8);
}

/*
Mode 4: ->
     1,    2,    3,    4,    5,    6,    7,    8
  BG2B, OAM0, BG1B, OAM1, BG2A, OAM2, BG1A, OAM3
*/
void bPPU::render_line_mode4() {
  render_line_bg(BG1, COLORDEPTH_256, 3, 7);
  render_line_bg(BG2, COLORDEPTH_4,   1, 5);
  render_line_oam(2, 4, 6, 8);
}

/*
Mode 5: ->
     1,    2,    3,    4,    5,    6,    7,    8
  BG2B, OAM0, BG1B, OAM1, BG2A, OAM2, BG1A, OAM3
*/
void bPPU::render_line_mode5() {
  render_line_bg(BG1, COLORDEPTH_16, 3, 7);
  render_line_bg(BG2, COLORDEPTH_4,  1, 5);
  render_line_oam(2, 4, 6, 8);
}

/*
Mode 6: ->
     1,    2,    3,    4,    5,    6
  OAM0, BG1B, OAM1, OAM2, BG1A, OAM3
*/
void bPPU::render_line_mode6() {
  render_line_bg(BG1, COLORDEPTH_16, 2, 5);
  render_line_oam(1, 3, 4, 6);
}

/*
Mode7: ->
     1,    2,    3,    4,    5
  OAM0, BG1n, OAM1, OAM2, OAM3

Mode 7 EXTBG: ->
     1,    2,    3,    4,    5,    6,    7
  BG2B, OAM0, BG1n, OAM1, BG2A, OAM2, OAM3
*/
void bPPU::render_line_mode7() {
  if(regs.mode7_extbg == false) {
    render_line_mode7(BG1, 2, 2);
    render_line_oam(1, 3, 4, 5);
  } else {
    render_line_mode7(BG1, 3, 3);
    render_line_mode7(BG2, 1, 5);
    render_line_oam(2, 4, 6, 7);
  }
}

void bPPU::render_line() {
  if(regs.display_disabled == true) {
    render_line_clear();
    return;
  }

  clear_pixel_cache();
  build_window_tables(COL);
  update_bg_info();

  switch(regs.bg_mode) {
  case 0: render_line_mode0(); break;
  case 1: render_line_mode1(); break;
  case 2: render_line_mode2(); break;
  case 3: render_line_mode3(); break;
  case 4: render_line_mode4(); break;
  case 5: render_line_mode5(); break;
  case 6: render_line_mode6(); break;
  case 7: render_line_mode7(); break;
  }

  render_line_output();
}

#endif //ifdef BPPU_CPP
