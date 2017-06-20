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
  information = {};

  if(auto fp = platform->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else {
    return false;
  }
  auto document = BML::unserialize(information.manifest);
  if(!cartridge.load()) return false;

  if(cartridge.region() == "NTSC") {
    information.region = Region::NTSC;
    information.colorburst = Emulator::Constants::Colorburst::NTSC;
  }
  if(cartridge.region() == "PAL") {
    information.region = Region::PAL;
    information.colorburst = Emulator::Constants::Colorburst::PAL * 4.0 / 5.0;
  }

  this->interface = interface;
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
