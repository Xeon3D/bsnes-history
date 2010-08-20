#include "SPC_DSP.h"

class DSP : public Processor, public ChipDebugger {
public:
  enum : bool { Threaded = false };
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_smp();

  uint8 read(uint8 addr);
  void write(uint8 addr, uint8 data);

  void enter();
  void power();
  void reset();

  void serialize(serializer&);
  bool property(unsigned id, string &name, string &value) { return false; }

private:
  SPC_DSP spc_dsp;
  int16 samplebuffer[8192];
};

extern DSP dsp;
