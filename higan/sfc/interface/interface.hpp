#if defined(CORE_SFC)

namespace higan::SuperFamicom {

extern Interface* interface;

struct SuperFamicomInterface : Interface {
  SuperFamicomInterface() { interface = this; }

  auto name() -> string override;
  auto colors() -> uint32 override;
  auto color(uint32 color) -> uint64 override;

  auto root() -> Node::Object override;
  auto load(string tree = {}) -> void override;
  auto power() -> void override;
  auto reset() -> void override;
  auto run() -> void override;
  auto save() -> void override;
  auto unload() -> void override;

  auto rtc() -> bool override;
  auto synchronize(uint64 timestamp) -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheats(const vector<string>&) -> void override;
};

}

#endif
