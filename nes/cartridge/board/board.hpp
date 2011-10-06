struct Board {
  struct Memory {
    uint8_t *data;
    unsigned size;
    inline uint8 read(unsigned addr) const;
    inline void write(unsigned addr, uint8 data);
    inline Memory(uint8_t *data, unsigned size) : data(data), size(size) {}
    inline Memory() : data(nullptr), size(0u) {}
  };

  static unsigned mirror(unsigned addr, unsigned size);

  virtual void main();
  virtual void tick();

  virtual uint8 prg_read(unsigned addr);
  virtual void prg_write(unsigned addr, uint8 data);

  virtual uint8 chr_read(unsigned addr);
  virtual void chr_write(unsigned addr, uint8 data);

  virtual Memory memory();

  virtual void power();
  virtual void reset();

  virtual void serialize(serializer&);
  Board(BML::Node &board, const uint8_t *data, unsigned size);
  ~Board();

  static Board* load(const string &markup, const uint8_t *data, unsigned size);

  struct Information {
    string type;
    bool battery;
  } information;

  Memory prgrom;
  Memory prgram;
  Memory chrrom;
  Memory chrram;
};
