#include <gba/gba.hpp>

namespace GBA {

#include "registers.cpp"
#include "mmio.cpp"
APU apu;

void APU::Enter() { apu.enter(); }

void APU::enter() {
  while(true) {
    interface->audioSample(0, 0);
    step(512);
  }
}

void APU::step(unsigned clocks) {
  clock += clocks;
  if(clock >= 0) co_switch(cpu.thread);
}

void APU::power() {
  create(APU::Enter, 16777216);

  regs.bias = 0x0200;

  for(unsigned n = 0x060; n <= 0x0a7; n++) bus.mmio[n] = this;
}

}
