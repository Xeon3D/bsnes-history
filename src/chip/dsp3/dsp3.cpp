#include "../../base.h"

namespace DSP3i {
  #define bool8 uint8
  #include "dsp3emu.c"
  #undef bool8
};

void DSP3::init() {
}

void DSP3::enable() {
}

void DSP3::power() {
  reset();
}

void DSP3::reset() {
  DSP3i::DSP3_Reset();
}

uint8 DSP3::read(uint addr) {
  DSP3i::dsp3_address = addr & 0xffff;
  DSP3i::DSP3GetByte();
  return DSP3i::dsp3_byte;
}

void DSP3::write(uint addr, uint8 data) {
  DSP3i::dsp3_address = addr & 0xffff;
  DSP3i::dsp3_byte = data;
  DSP3i::DSP3SetByte();
}
