struct Debugger {
  Debugger();
  void run();
  void cpuExec(uint24 addr);
  void echoHex(unsigned addr, signed size);

  enum class Mode : unsigned { Break, Run, Step } mode = Mode::Break;
  unsigned stepDuration = 0;
};

extern Debugger* debugger;
