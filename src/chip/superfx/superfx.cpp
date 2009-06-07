#include <../base.hpp>

#define SUPERFX_CPP
namespace SNES {

#include "bus/bus.cpp"
#include "core/core.cpp"
#include "memory/memory.cpp"
#include "mmio/mmio.cpp"
#include "timing/timing.cpp"
#include "disasm/disasm.cpp"

SuperFX superfx;

void SuperFX::enter() {
  while(true) {
    while(regs.sfr.g == 0) add_clocks(2);
    exec_opcode();
  }
}

void SuperFX::init() {
  initialize_opcode_table();
  regs.r[14].on_modify = bind(&SuperFX::r14_modify, this);
  regs.r[15].on_modify = bind(&SuperFX::r15_modify, this);
}

void SuperFX::enable() {
  for(uint16_t i = 0x3000; i <= 0x32ff; i++) memory::mmio.map(i, *this);
}

void SuperFX::power() {
  reset();
}

void SuperFX::reset() {
  superfxbus.init();

  for(unsigned n = 0; n < 16; n++) regs.r[n] = 0x0000;
  regs.sfr   = 0x0000;
  regs.pbr   = 0x00;
  regs.rombr = 0x00;
  regs.rambr = 0;
  regs.cbr   = 0x0000;
  regs.scbr  = 0x00;
  regs.scmr  = 0x00;
  regs.colr  = 0x00;
  regs.por   = 0x00;
  regs.bramr = 0;
  regs.vcr   = 0x04;
  regs.cfgr  = 0x00;
  regs.clsr  = 0;
  regs.pipeline = 0x01;  //nop
  regs.ramaddr = 0x0000;
  regs.reset();

  memory_reset();
  timing_reset();
}

};
