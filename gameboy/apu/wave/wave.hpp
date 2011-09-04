struct Wave {
  bool enable;

  bool dac_enable;
  unsigned volume;
  unsigned frequency;
  bool counter;
  uint8 pattern[32];

  int16 output;
  unsigned length;
  unsigned period;
  unsigned pattern_offset;
  unsigned pattern_sample;

  void run();
  void clock_length();
  void write(unsigned r, uint8 data);
  void write_pattern(unsigned p, uint8 data);
  void power();
  void serialize(serializer&);
};
