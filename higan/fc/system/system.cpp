#include <fc/fc.hpp>

namespace Famicom {

#include "peripherals.cpp"
#include "video.cpp"
#include "serialization.cpp"
System system;
Scheduler scheduler;
Cheat cheat;

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(apu);
  scheduler.synchronize(ppu);
  scheduler.synchronize(cartridge);
  for(auto peripheral : cpu.peripherals) scheduler.synchronize(*peripheral);
}

auto System::load(Emulator::Interface* interface) -> bool {
  information = Information();
  if(auto fp = platform->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else {
    return false;
  }
  auto document = BML::unserialize(information.manifest);
  if(!cartridge.load()) return false;

  this->interface = interface;
  information.colorburst = Emulator::Constants::Colorburst::NTSC;
  serializeInit();
  return information.loaded = true;
}

auto System::save() -> void {
  cartridge.save();
}

auto System::unload() -> void {
  if(!loaded()) return;
  peripherals.unload();
  cartridge.unload();
  information.loaded = false;
}

auto System::power() -> void {
  Emulator::video.reset();
  Emulator::video.setInterface(interface);
  configureVideoPalette();
  configureVideoEffects();

  Emulator::audio.reset();
  Emulator::audio.setInterface(interface);

  scheduler.reset();
  cartridge.power();
  cpu.power();
  apu.power();
  ppu.power();
  scheduler.primary(cpu);
  peripherals.reset();
}

auto System::init() -> void {
  assert(interface != nullptr);
}

auto System::term() -> void {
}

}
