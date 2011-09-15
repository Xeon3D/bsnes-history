#include <gameboy/gameboy.hpp>

namespace GameBoy {

Interface *interface = 0;

void Interface::lcdScanline() {
}

void Interface::joypWrite(bool p15, bool p14) {
}

void Interface::videoRefresh(const uint8_t *data) {
}

void Interface::audioSample(int16_t center, int16_t left, int16_t right) {
}

bool Interface::inputPoll(unsigned id) {
  return false;
}

void Interface::initialize(Interface *derived_interface) {
  interface = derived_interface;
  system.init();
}

bool Interface::cartridgeLoaded() {
  return cartridge.loaded();
}

void Interface::loadCartridge(const string &xml, const uint8_t *data, unsigned size) {
  cartridge.load(xml, data, size);
  system.power();
}

void Interface::unloadCartridge() {
  cartridge.unload();
}

void Interface::power() {
  system.power();
}

void Interface::run() {
  do {
    system.run();
  } while(scheduler.exit_reason() != Scheduler::ExitReason::FrameEvent);
}

serializer Interface::serialize() {
  system.runtosave();
  return system.serialize();
}

bool Interface::unserialize(serializer &s) {
  return system.unserialize(s);
}

void Interface::setCheats(const lstring &list) {
  cheat.reset();
  foreach(code, list) {
    lstring codelist;
    codelist.split("+", code);
    foreach(part, codelist) {
      unsigned addr, data, comp;
      if(Cheat::decode(part, addr, data, comp)) {
        cheat.append({ addr, data, comp });
      }
    }
  }
  cheat.synchronize();
}

void Interface::message(const string &text) {
  print(text, "\n");
}

}
