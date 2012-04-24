#include <gb/gb.hpp>

#define SYSTEM_CPP
namespace GB {

#include "serialization.cpp"
System system;

void System::run() {
  scheduler.sync = Scheduler::SynchronizeMode::None;

  scheduler.enter();
  if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
  }
}

void System::runtosave() {
  scheduler.sync = Scheduler::SynchronizeMode::CPU;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.active_thread = lcd.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.active_thread = apu.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::None;
}

void System::runthreadtosave() {
  while(true) {
    scheduler.enter();
    if(scheduler.exit_reason() == Scheduler::ExitReason::SynchronizeEvent) break;
    if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
    }
  }
}

void System::init() {
  file fp;
  fp.open("/home/byuu/Desktop/boot.rom", file::mode::write);
  fp.write(bootROM.sgb, 256);
  fp.close();

  assert(interface != 0);
}

void System::load(Revision revision) {
  this->revision = revision;
  serialize_init();
}

void System::power() {
  bus.power();
  cartridge.power();
  cpu.power();
  apu.power();
  lcd.power();
  scheduler.init();

  clocks_executed = 0;
}

System::System() {
  for(auto &byte : bootROM.dmg) byte = 0;
  for(auto &byte : bootROM.sgb) byte = 0;
  for(auto &byte : bootROM.cgb) byte = 0;
}

}
