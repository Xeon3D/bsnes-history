#include <msx/msx.hpp>

namespace higan::MSX {

Options option;
Properties property;

auto MSXInterface::information() -> Information {
  Information information;
  information.manufacturer = "";
  information.name         = "MSX";
  information.extension    = "msx";
  return information;
}

auto MSXInterface::display() -> Display {
  Display display;
  display.type   = Display::Type::CRT;
  display.colors = 1 << 4;
  display.width  = 256;
  display.height = 192;
  display.internalWidth  = 256;
  display.internalHeight = 192;
  display.aspectCorrection = 1.0;
  return display;
}

auto MSXInterface::color(uint32 color) -> uint64 {
  switch(color.bits(0,3)) {
  case  0: return 0x0000'0000'0000ull;  //transparent
  case  1: return 0x0000'0000'0000ull;  //black
  case  2: return 0x2121'c8c8'4242ull;  //medium green
  case  3: return 0x5e5e'dcdc'7878ull;  //light green
  case  4: return 0x5454'5555'ededull;  //dark blue
  case  5: return 0x7d7d'7676'fcfcull;  //light blue
  case  6: return 0xd4d4'5252'4d4dull;  //dark red
  case  7: return 0x4242'ebeb'f5f5ull;  //cyan
  case  8: return 0xfcfc'5555'5454ull;  //medium red
  case  9: return 0xffff'7979'7878ull;  //light red
  case 10: return 0xd4d4'c1c1'5454ull;  //dark yellow
  case 11: return 0xe6e6'cece'8080ull;  //light yellow
  case 12: return 0x2121'b0b0'3b3bull;  //dark green
  case 13: return 0xc9c9'5b5b'babaull;  //magenta
  case 14: return 0xcccc'cccc'ccccull;  //gray
  case 15: return 0xffff'ffff'ffffull;  //white
  }
  return 0;
}

auto MSXInterface::loaded() -> bool {
  return system.loaded();
}

auto MSXInterface::hashes() -> vector<string> {
  return {cartridge.hash()};
}

auto MSXInterface::manifests() -> vector<string> {
  return {cartridge.manifest()};
}

auto MSXInterface::titles() -> vector<string> {
  return {cartridge.title()};
}

auto MSXInterface::load() -> bool {
  return system.load(this, System::Model::MSX);
}

auto MSXInterface::save() -> void {
  system.save();
}

auto MSXInterface::unload() -> void {
  save();
  system.unload();
}

auto MSXInterface::ports() -> vector<Port> { return {
  {ID::Port::Controller1, "Controller Port 1"},
  {ID::Port::Controller2, "Controller Port 2"}};
}

auto MSXInterface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Controller1) return {
    {ID::Device::None,    "None"   },
    {ID::Device::Gamepad, "Gamepad"}
  };

  if(port == ID::Port::Controller2) return {
    {ID::Device::None,    "None"   },
    {ID::Device::Gamepad, "Gamepad"}
  };

  return {};
}

auto MSXInterface::inputs(uint device) -> vector<Input> {
  using Type = Input::Type;

  if(device == ID::Device::Gamepad) return {
    {Type::Hat,    "Up"   },
    {Type::Hat,    "Down" },
    {Type::Hat,    "Left" },
    {Type::Hat,    "Right"},
    {Type::Button, "1"    },
    {Type::Button, "2"    }
  };

  return {};
}

auto MSXInterface::power() -> void {
  system.power();
}

auto MSXInterface::run() -> void {
  system.run();
}

auto MSXInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto MSXInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto MSXInterface::options() -> Settings& {
  return option;
}

auto MSXInterface::properties() -> Settings& {
  return property;
}

}
