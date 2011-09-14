struct Interface {
  virtual void video_refresh(const uint16_t *data) {}
  virtual void audio_sample(int16_t sample) {}
  virtual int16_t input_poll(bool port, unsigned device, unsigned id) { return 0; }

  virtual void message(const string &text) { print(text, "\n"); }
};
