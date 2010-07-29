class MSU1 : public Processor, public MMIO {
public:
  //synchronization
  alwaysinline void create();
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_cpu();

  void enter();
  void init();
  void enable();
  void power();
  void reset();

  uint8 mmio_read(unsigned addr);
  void mmio_write(unsigned addr, uint8 data);

  void serialize(serializer&);

private:
  file datafile;
  file audiofile;

  enum Flag {
    DataBusy       = 0x80,
    AudioBusy      = 0x40,
    AudioRepeating = 0x20,
    AudioPlaying   = 0x10,
    Revision       = 0x01,
  };

  struct MMIO {
    uint32 data_offset;
    uint32 audio_offset;
    uint16 audio_track;
    uint8 audio_volume;
    bool data_busy;
    bool audio_busy;
    bool audio_repeat;
    bool audio_play;
  } mmio;
};

extern MSU1 msu1;
