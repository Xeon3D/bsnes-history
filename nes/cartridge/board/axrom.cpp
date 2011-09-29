//NES-AMROM
//NES-ANROM
//NES-AN1ROM
//NES-AOROM

struct AxROM : Board {

uint4 prg_bank;
bool mirror_select;

uint8 prg_read(unsigned addr) {
  if(addr & 0x8000) return Board::prg_read((prg_bank << 15) | (addr & 0x7fff));
  return cpu.mdr();
}

void prg_write(unsigned addr, uint8 data) {
  if(addr & 0x8000) {
    prg_bank = data & 0x0f;
    mirror_select = data & 0x10;
  }
}

uint8 chr_read(unsigned addr) {
  if(addr & 0x2000) return ppu.ciram_read((mirror_select << 10) | (addr & 0x03ff));
  return Board::chr_read(addr);
}

void chr_write(unsigned addr, uint8 data) {
  if(addr & 0x2000) return ppu.ciram_write((mirror_select << 10) | (addr & 0x03ff), data);
  return Board::chr_write(addr, data);
}

void power() {
  reset();
}

void reset() {
  prg_bank = 0x0f;
  mirror_select = 0;
}

void serialize(serializer &s) {
  s.integer(prg_bank);
  s.integer(mirror_select);
}

AxROM(BML::Node &board, const uint8_t *data, unsigned size) : Board(board, data, size) {
}

};
