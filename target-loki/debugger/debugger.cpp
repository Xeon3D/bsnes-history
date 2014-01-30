#include "../loki.hpp"
Debugger* debugger = nullptr;

Debugger::Debugger() {
  debugger = this;
  SFC::cpu.debugger.op_exec = {&Debugger::cpuExec, this};
}

void Debugger::run() {
  switch(mode) {
  case Mode::Break:
    usleep(20 * 1000);
    break;

  case Mode::Run:
    emulator->run();
    break;

  case Mode::Step:
    emulator->run();
    break;
  }
}

void Debugger::cpuExec(uint24 addr) {
  if(mode == Mode::Step) {
    char text[4096];
    SFC::cpu.disassemble_opcode(text, addr);
    echo(text, "\n");
    if(--stepDuration == 0) {
      mode = Mode::Break;
      SFC::scheduler.debug();
    }
  }
}

void Debugger::echoHex(unsigned addr, signed size) {
  while(size > 0) {
    string hexdata, asciidata;
    for(unsigned n = 0; n < 16; n++) {
      unsigned offset = addr;
      if((offset & 0x40e000) == 0x002000 || (offset & 0x40e000) == 0x004000) {
        //$00-3f,80-bf:2000-5fff
        //reading MMIO registers can negatively impact emulation, so disallow these reads
        hexdata.append("?? ");
        asciidata.append("?");
      } else {
        uint8_t byte = SFC::bus.read(addr + n);
        hexdata.append(hex<2>(byte), " ");
        asciidata.append(byte >= 0x20 && byte <= 0x7e ? (char)byte : '.');
      }
    }
    echo(hex<6>(addr), " [ ", hexdata, "] ", asciidata, "\n");
    addr += 16, size -= 16;
  }
}
