#if !defined(PROFILE_CSNES)
  #include "../snes_spc/SPC_DSP.h"
#endif

class DSP : public Processor {
public:
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_smp();
  void run();

  uint8 read(uint8 addr);
  void write(uint8 addr, uint8 data);

  void power();
  void reset();

  void serialize(serializer&);
  DSP();
  ~DSP();

private:
  SPC_DSP spc_dsp;
  int16 samplebuffer[8192];
};

extern DSP dsp;
