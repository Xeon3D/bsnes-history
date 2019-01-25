#if defined(CORE_MSX)

namespace higan::MSX {

struct ID {
  enum : uint {
    System,
    MSX,
    MSX2,
    MSX2Plus,
    MSXTurboR,
  };

  struct Port { enum : uint {
    Controller1,
    Controller2,
  };};

  struct Device { enum : uint {
    None,
    Gamepad,
  };};
};

struct MSXInterface : Interface {
  auto information() -> Information override;

  auto display() -> Display override;
  auto color(uint32 color) -> uint64 override;

  auto loaded() -> bool override;
  auto hashes() -> vector<string> override;
  auto manifests() -> vector<string> override;
  auto titles() -> vector<string> override;
  auto load() -> bool override;
  auto save() -> void override;
  auto unload() -> void override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto options() -> Settings& override;
  auto properties() -> Settings& override;
};

#include "options.hpp"
#include "properties.hpp"

}

#endif
