#include <processor/processor.hpp>
#include "arm.hpp"

namespace Processor {

#include "registers.cpp"
#include "algorithms.cpp"
#include "instructions-arm.cpp"
#include "instructions-thumb.cpp"
#include "disassembler.cpp"

void ARM::power() {
  processor.power();
  vector(0x00000000, Processor::Mode::SVC);
  pipeline.reload = true;
  crash = false;
  r(15).modify = [&] {
    pipeline.reload = true;
  };

  trace = false;
  instructions = 0;
}

void ARM::exec() {
  if(processor.irqline && cpsr().i == 0) {
    vector(0x00000018, Processor::Mode::IRQ);
    r(14) += 4;
  }

  cpsr().t ? thumb_step() : arm_step();
}

uint32 ARM::read(uint32 addr, uint32 size) {
  uint32 word = bus_read(addr, size);
//uint32 rotate = (addr & 3) << 3;
//word = (word >> rotate) | (word << (32 - rotate));
//word = word & (~0u >> (32 - size));
  return word;
}

void ARM::write(uint32 addr, uint32 size, uint32 word) {
  return bus_write(addr, size, word);
}

void ARM::vector(uint32 addr, Processor::Mode mode) {
  auto psr = cpsr();
  processor.setMode(mode);
  spsr() = psr;
  cpsr().i = 1;
  cpsr().f = mode == Processor::Mode::FIQ;
  cpsr().t = 0;
  r(14) = pipeline.decode.address;
  r(15) = addr;
}

void ARM::serialize(serializer &s) {
}

}
