#include <nes/nes.hpp>

namespace NES {

Cartridge cartridge;

void Cartridge::load(const string &xml, const uint8_t *data, unsigned size) {
  rom_size = size - 16;
  rom_data = new uint8[rom_size];
  memcpy(rom_data, data + 16, size - 16);

  prg_size = data[4] * 0x4000;
  chr_size = data[5] * 0x2000;

  prg_data = new uint8[prg_size];
  memcpy(prg_data, data + 16, prg_size);

  if(chr_size) {
    chr_ram = false;
    chr_data = new uint8[chr_size];
    memcpy(chr_data, data + 16 + prg_size, chr_size);
  } else {
    chr_ram = true;
    chr_size = 0x2000;
    chr_data = new uint8[chr_size]();
  }

  mirroring = ((data[6] & 0x08) >> 2) | (data[6] & 0x01);

  uint8 mapperNumber = ((data[7] >> 4) << 4) | (data[6] >> 4);
  switch(mapperNumber) {
  default : mapper = &Mapper::none; break;
  case   1: mapper = &Mapper::mmc1; break;
  case   3: mapper = &Mapper::cnrom; break;
  case   7: mapper = &Mapper::aorom; break;
  case  16: mapper = &Mapper::bandaiFCG; break;
  }

  loaded = true;
}

void Cartridge::unload() {
  if(loaded == false) return;

  delete[] rom_data;
  delete[] prg_data;
  delete[] chr_data;

  loaded = false;
}

unsigned Cartridge::ram_size() {
  return mapper->ram_size();
}

uint8* Cartridge::ram_data() {
  return mapper->ram_data();
}

void Cartridge::power() {
  mapper->power();
}

void Cartridge::reset() {
  mapper->reset();
}

Cartridge::Cartridge() {
  loaded = false;
}

uint8 Cartridge::prg_read(uint16 addr) {
  return mapper->prg_read(addr);
}

void Cartridge::prg_write(uint16 addr, uint8 data) {
  return mapper->prg_write(addr, data);
}

uint8 Cartridge::chr_read(uint16 addr) {
  return mapper->chr_read(addr);
}

void Cartridge::chr_write(uint16 addr, uint8 data) {
  return mapper->chr_write(addr, data);
}

uint8 Cartridge::ciram_read(uint13 addr) {
  return mapper->ciram_read(addr);
}

void Cartridge::ciram_write(uint13 addr, uint8 data) {
  return mapper->ciram_write(addr, data);
}

}
