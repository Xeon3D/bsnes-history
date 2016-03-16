#include <ws/ws.hpp>

namespace WonderSwan {

CPU cpu;
#include "io.cpp"
#include "interrupt.cpp"
#include "dma.cpp"

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  poll();
  exec();
}

auto CPU::step(uint clocks) -> void {
  ppu.clock -= clocks;
  if(ppu.clock < 0) co_switch(ppu.thread);

  apu.clock -= clocks;
  if(apu.clock < 0) co_switch(apu.thread);
}

auto CPU::wait(uint clocks) -> void {
  step(clocks);
}

auto CPU::read(uint20 addr) -> uint8 {
  return bus.read(addr);
}

auto CPU::write(uint20 addr, uint8 data) -> void {
  return bus.write(addr, data);
}

auto CPU::in(uint16 port) -> uint8 {
  return bus.portRead(port);
}

auto CPU::out(uint16 port, uint8 data) -> void {
  return bus.portWrite(port, data);
}

auto CPU::power() -> void {
  V30MZ::power();
  create(CPU::Enter, 3'072'000);

  bus.map(this, 0x00a0);
  bus.map(this, 0x00b0);
  bus.map(this, 0x00b2);
  bus.map(this, 0x00b4);
  bus.map(this, 0x00b5);
  bus.map(this, 0x00b6);

  if(system.model() != Model::WonderSwan) {
    bus.map(this, 0x0040, 0x0049);
    bus.map(this, 0x0062);
  }

  r.dmaSource = 0;
  r.dmaTarget = 0;
  r.dmaLength = 0;
  r.dmaEnable = 0;
  r.dmaMode = 0;
  r.interruptBase = 0;
  r.interruptEnable = 0;
  r.interruptStatus = 0;
  r.ypadEnable = 0;
  r.xpadEnable = 0;
  r.buttonEnable = 0;
}

}
