struct InterfaceNES : NES::Interface {
  bool loadCartridge(const string &filename);
  void unloadCartridge();

  void video_refresh(const uint32_t *data);
  void audio_sample(int16_t lsample, int16_t rsample);
  int16_t input_poll(bool port, unsigned device, unsigned id);
};
