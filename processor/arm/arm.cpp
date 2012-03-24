#include <processor/processor.hpp>
#include "arm.hpp"

namespace Processor {

#include "registers.cpp"
#include "instructions-arm.cpp"
#include "instructions-thumb.cpp"
#include "disassembler.cpp"

void ARM::power() {
  processor.power();
  vector(0x00000000, Processor::Mode::SVC);
  pipeline.reload = true;
  exception = false;
  r(15).modify = [&] {
    pipeline.reload = true;
    r(15).data &= cpsr().t ? ~1 : ~3;
  };
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
