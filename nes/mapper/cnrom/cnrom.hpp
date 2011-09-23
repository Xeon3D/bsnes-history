struct CNROM : Mapper {
  uint8 prg_read(uint16 addr);
  void prg_write(uint16 addr, uint8 data);

  uint8 chr_read(uint16 addr);
  void chr_write(uint16 addr, uint8 data);

  uint8 ciram_read(uint13 addr);
  void ciram_write(uint13 addr, uint8 data);

  void power();
  void reset();

  void serialize(serializer&);

private:
  uint2 chr_bank;
};

extern CNROM cnrom;
