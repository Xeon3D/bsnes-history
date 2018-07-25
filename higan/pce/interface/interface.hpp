#if defined(CORE_PCE)

namespace PCEngine {

struct ID {
  enum : uint {
    System,
    PCEngine,
    SuperGrafx,
  };

  struct Port { enum : uint {
    Controller,
  };};

  struct Device { enum : uint {
    None,
    Gamepad,
  };};
};

struct Interface : Emulator::Interface {
  auto displays() -> vector<Display> override;
  auto color(uint32 color) -> uint64 override;

  auto loaded() -> bool override;
  auto hashes() -> vector<string> override;
  auto manifests() -> vector<string> override;
  auto titles() -> vector<string> override;
  auto save() -> void override;
  auto unload() -> void override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto connected(uint port) -> uint override;
  auto connect(uint port, uint device) -> void override;
  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheats(const vector<string>& list) -> void override;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;
};

struct PCEngineInterface : Interface {
  auto information() -> Information override;

  auto load() -> bool override;
};

struct SuperGrafxInterface : Interface {
  auto information() -> Information override;

  auto load() -> bool override;
};

struct Settings {
  uint controllerPort = ID::Device::Gamepad;
};

extern Settings settings;

}

#endif
