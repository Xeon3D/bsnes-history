struct InterfaceSNES : InterfaceCore, SNES::Interface {
  void initialize();

  string markup();

  void setController(bool port, unsigned device);

  bool cartridgeLoaded();
  vector<uint8_t> loadCartridge(const string &filename, CartridgePath &cartridge);
  bool loadCartridge(string basename);
  bool loadSatellaviewSlottedCartridge(string basename, string slotname);
  bool loadSatellaviewCartridge(string basename, string slotname);
  bool loadSufamiTurboCartridge(string basename, string slotAname, string slotBname);
  bool loadSuperGameBoyCartridge(string basename, string slotname);
  void unloadCartridge();

  void power();
  void reset();
  void run();

  string memoryName(SNES::Cartridge::NonVolatileRAM &memory);
  void loadMemory();
  void saveMemory();

  serializer serialize();
  bool unserialize(serializer&);

  void setCheats(const lstring &list = lstring{});

  uint32_t videoColor(uint19_t source, uint16_t red, uint16_t green, uint16_t blue);
  void videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan);
  void audioSample(int16_t lsample, int16_t rsample);
  int16_t inputPoll(bool port, SNES::Input::Device device, unsigned index, unsigned id);

  string path(SNES::Cartridge::Slot slot, const string &hint);
  void message(const string &text);
};
