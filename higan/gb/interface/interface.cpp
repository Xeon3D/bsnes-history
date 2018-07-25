#include <gb/gb.hpp>

namespace GameBoy {

SuperGameBoyInterface* superGameBoy = nullptr;
Settings settings;
#include "game-boy.cpp"
#include "game-boy-color.cpp"

auto Interface::displays() -> vector<Display> {
  Display display;
  display.type   = Display::Type::LCD;
  display.colors = Model::GameBoyColor() ? 1 << 15 : 1 << 2;
  display.width  = 160;
  display.height = 144;
  display.internalWidth  = 160;
  display.internalHeight = 144;
  display.aspectCorrection = 1.0;
  display.refreshRate = (4.0 * 1024.0 * 1024.0) / (154.0 * 456.0);
  return {display};
}

auto Interface::loaded() -> bool {
  return system.loaded();
}

auto Interface::hashes() -> vector<string> {
  return {cartridge.hash()};
}

auto Interface::manifests() -> vector<string> {
  return {cartridge.manifest()};
}

auto Interface::titles() -> vector<string> {
  return {cartridge.title()};
}

auto Interface::save() -> void {
  system.save();
}

auto Interface::unload() -> void {
  save();
  system.unload();
}

auto Interface::ports() -> vector<Port> { return {
  {ID::Port::Hardware, "Hardware"}};
}

auto Interface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Hardware) return {
    {ID::Device::Controls, "Controls"}
  };

  return {};
}

auto Interface::inputs(uint device) -> vector<Input> {
  using Type = Input::Type;

  if(device == ID::Device::Controls) return {
    {Type::Hat,     "Up"    },
    {Type::Hat,     "Down"  },
    {Type::Hat,     "Left"  },
    {Type::Hat,     "Right" },
    {Type::Button,  "B"     },
    {Type::Button,  "A"     },
    {Type::Control, "Select"},
    {Type::Control, "Start" },
    {Type::Axis,    "X-axis"},
    {Type::Axis,    "Y-axis"},
    {Type::Rumble,  "Rumble"}
  };

  return {};
}

auto Interface::power() -> void {
  system.power();
}

auto Interface::run() -> void {
  system.run();
}

auto Interface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto Interface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto Interface::cheats(const vector<string>& list) -> void {
  cheat.assign(list);
}

auto Interface::cap(const string& name) -> bool {
  if(name == "Blur Emulation") return true;
  if(name == "Color Emulation") return true;
  return false;
}

auto Interface::get(const string& name) -> any {
  if(name == "Blur Emulation") return settings.blurEmulation;
  if(name == "Color Emulation") return settings.colorEmulation;
  return {};
}

auto Interface::set(const string& name, const any& value) -> bool {
  if(name == "Blur Emulation" && value.is<bool>()) {
    settings.blurEmulation = value.get<bool>();
    if(Model::SuperGameBoy()) return true;
    Emulator::video.setEffect(Emulator::Video::Effect::InterframeBlending, settings.blurEmulation);
    return true;
  }

  if(name == "Color Emulation" && value.is<bool>()) {
    settings.colorEmulation = value.get<bool>();
    if(Model::SuperGameBoy()) return true;
    Emulator::video.setPalette();
    return true;
  }

  return false;
}

}
