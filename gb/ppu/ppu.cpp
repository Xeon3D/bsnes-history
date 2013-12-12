#include <gb/gb.hpp>

//LY     =   0-153
//Raster =   0-143
//Vblank = 144-153

//LX     =   0-455

#define PPU_CPP
namespace GameBoy {

#include "mmio.cpp"
#include "dmg.cpp"
#include "cgb.cpp"
#include "serialization.cpp"
PPU ppu;

void PPU::Main() {
  ppu.main();
}

void PPU::main() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    //X = 0
    if(status.display_enable) {
      if(status.interrupt_oam) cpu.interrupt_raise(CPU::Interrupt::Stat);
    }

    add_clocks(92);

    if(status.ly < 144) {
      for(unsigned n = 0; n < 160; n++) {
        system.cgb() ? cgb_run() : dmg_run();
        add_clocks(1);
      }
    } else {
      //Vblank
      add_clocks(160);
    }

    //X = 252
    if(status.display_enable) {
      if(status.interrupt_hblank) cpu.interrupt_raise(CPU::Interrupt::Stat);
      cpu.hblank();
    }

    add_clocks(204);
    scanline();
  }
}

void PPU::add_clocks(unsigned clocks) {
  status.lx += clocks;
  clock += clocks * cpu.frequency;
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) {
    co_switch(scheduler.active_thread = cpu.thread);
  }
}

void PPU::scanline() {
  status.lx = 0;
  if(++status.ly == 154) frame();

  if(status.ly < 144) {
    interface->lcdScanline();  //Super Game Boy rendering notification
    system.cgb() ? cgb_scanline() : dmg_scanline();
  }

  if(status.display_enable && status.interrupt_lyc == true) {
    if(status.ly == status.lyc) cpu.interrupt_raise(CPU::Interrupt::Stat);
  }

  if(status.display_enable && status.ly == 144) {
    cpu.interrupt_raise(CPU::Interrupt::Vblank);
    if(status.interrupt_vblank) cpu.interrupt_raise(CPU::Interrupt::Stat);
  }
}

void PPU::frame() {
  status.ly = 0;
  scheduler.exit(Scheduler::ExitReason::FrameEvent);
}

unsigned PPU::hflip(unsigned data) const {
  return ((data & 0x8080) >> 7) | ((data & 0x4040) >> 5)
       | ((data & 0x2020) >> 3) | ((data & 0x1010) >> 1)
       | ((data & 0x0808) << 1) | ((data & 0x0404) << 3)
       | ((data & 0x0202) << 5) | ((data & 0x0101) << 7);
}

void PPU::power() {
  create(Main, 4 * 1024 * 1024);

  for(unsigned n = 0x8000; n <= 0x9fff; n++) bus.mmio[n] = this;  //VRAM
  for(unsigned n = 0xfe00; n <= 0xfe9f; n++) bus.mmio[n] = this;  //OAM

  bus.mmio[0xff40] = this;  //LCDC
  bus.mmio[0xff41] = this;  //STAT
  bus.mmio[0xff42] = this;  //SCY
  bus.mmio[0xff43] = this;  //SCX
  bus.mmio[0xff44] = this;  //LY
  bus.mmio[0xff45] = this;  //LYC
  bus.mmio[0xff47] = this;  //BGP
  bus.mmio[0xff48] = this;  //OBP0
  bus.mmio[0xff49] = this;  //OBP1
  bus.mmio[0xff4a] = this;  //WY
  bus.mmio[0xff4b] = this;  //WX

  if(system.cgb()) {
  bus.mmio[0xff4f] = this;  //VBK
  bus.mmio[0xff68] = this;  //BGPI
  bus.mmio[0xff69] = this;  //BGPD
  bus.mmio[0xff6a] = this;  //OBPI
  bus.mmio[0xff6b] = this;  //OBPD
  }

  for(auto& n : vram) n = 0x00;
  for(auto& n : oam) n = 0x00;
  for(auto& n : bgp) n = 0x00;
  for(auto& n : obp[0]) n = 0x00;
  for(auto& n : obp[1]) n = 0x00;
  for(auto& n : bgpd) n = 0x0000;
  for(auto& n : obpd) n = 0x0000;

  status.lx = 0;

  status.display_enable = 0;
  status.window_tilemap_select = 0;
  status.window_display_enable = 0;
  status.bg_tiledata_select = 0;
  status.bg_tilemap_select = 0;
  status.ob_size = 0;
  status.ob_enable = 0;
  status.bg_enable = 0;

  status.interrupt_lyc = 0;
  status.interrupt_oam = 0;
  status.interrupt_vblank = 0;
  status.interrupt_hblank = 0;

  status.scy = 0;
  status.scx = 0;
  status.ly = 0;
  status.lyc = 0;
  status.wy = 0;
  status.wx = 0;

  status.vram_bank = 0;

  status.bgpi_increment = 0;
  status.bgpi = 0;

  status.obpi_increment = 0;
  status.obpi = 0;

  for(auto& n : screen) n = 0x0000;

  bg.color = 0;
  bg.palette = 0;
  bg.priority = 0;

  ob.color = 0;
  ob.palette = 0;
  ob.priority = 0;

  for(auto& s : sprite) {
    s.x = 0;
    s.y = 0;
    s.tile = 0;
    s.attr = 0;
    s.data = 0;
  }
  sprites = 0;

  background.attr = 0;
  background.data = 0;

  window.attr = 0;
  window.data = 0;
}

PPU::PPU() {
}

}
