#ifdef SPC7110_CPP

uint8 SPC7110::datarom_read(unsigned addr) {
  unsigned mask = (1 << (r4834 & 3)) - 1;  //8mbit, 16mbit, 32mbit, 64mbit
  unsigned range = 0x100000 * (1 + mask);
  unsigned offset = addr & 0x7fffff;

  //mirroring behavior is non-sensical. assuming a 32mbit data ROM:
  //banks 4-7 with mask 0-2 returns 0x00; banks 4-7 with mask 3 mirrors banks 0-3
  if(range <= drom_size && offset >= drom_size) return 0x00;

  return cartridge.rom.read(drom_base + Bus::mirror(offset, drom_size));
}

unsigned SPC7110::data_offset() { return r4811 | r4812 << 8 | r4813 << 16; }
unsigned SPC7110::data_adjust() { return r4814 | r4815 << 8; }
unsigned SPC7110::data_stride() { return r4816 | r4817 << 8; }
void SPC7110::set_data_offset(unsigned addr) { r4811 = addr; r4812 = addr >> 8; r4813 = addr >> 16; }
void SPC7110::set_data_adjust(unsigned addr) { r4814 = addr; r4815 = addr >> 8; }

void SPC7110::data_port_read() {
  unsigned offset = data_offset();
  unsigned adjust = r4818 & 2 ? data_adjust() : 0;
  if(r4818 & 8) adjust = (int16)adjust;
  r4810 = datarom_read(offset + adjust);
}

void SPC7110::data_port_increment_4810() {
  unsigned offset = data_offset();
  unsigned stride = r4818 & 1 ? data_stride() : 1;
  unsigned adjust = data_adjust();
  if(r4818 & 4) stride = (int16)stride;
  if(r4818 & 8) adjust = (int16)adjust;
  if((r4818 & 16) == 0) set_data_offset(offset + stride);
  if((r4818 & 16) != 0) set_data_adjust(adjust + stride);
}

void SPC7110::data_port_increment_4814() {
  if(r4818 >> 5 != 1) return;
  unsigned offset = data_offset();
  unsigned adjust = data_adjust();
  if(r4818 & 8) adjust = (int16)adjust;
  set_data_offset(offset + adjust);
}

void SPC7110::data_port_increment_4815() {
  if(r4818 >> 5 != 2) return;
  unsigned offset = data_offset();
  unsigned adjust = data_adjust();
  if(r4818 & 8) adjust = (int16)adjust;
  set_data_offset(offset + adjust);
}

void SPC7110::data_port_increment_481a() {
  if(r4818 >> 5 != 3) return;
  unsigned offset = data_offset();
  unsigned adjust = data_adjust();
  if(r4818 & 8) adjust = (int16)adjust;
  set_data_offset(offset + adjust);
}

#endif
