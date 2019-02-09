namespace higan::Core {
struct Object;
  struct System;
  struct Component;
  struct Video;
  struct Audio;
  struct Peripheral;
  struct Port;
  struct Input;
    struct Button;
    struct Axis;
    struct Trigger;
  struct Settings;
  struct Setting;
    struct Boolean;
    struct Natural;
    struct Integer;
    struct Real;
    struct String;
}

namespace higan::Node {
using Object       = shared_pointer<Core::Object>;
  using System     = shared_pointer<Core::System>;
  using Component  = shared_pointer<Core::Component>;
  using Video      = shared_pointer<Core::Video>;
  using Audio      = shared_pointer<Core::Audio>;
  using Peripheral = shared_pointer<Core::Peripheral>;
  using Port       = shared_pointer<Core::Port>;
  using Input      = shared_pointer<Core::Input>;
    using Button   = shared_pointer<Core::Button>;
    using Axis     = shared_pointer<Core::Axis>;
    using Trigger  = shared_pointer<Core::Trigger>;
  using Settings   = shared_pointer<Core::Settings>;
  using Setting    = shared_pointer<Core::Setting>;
    using Boolean  = shared_pointer<Core::Boolean>;
    using Natural  = shared_pointer<Core::Natural>;
    using Integer  = shared_pointer<Core::Integer>;
    using Real     = shared_pointer<Core::Real>;
    using String   = shared_pointer<Core::String>;
}

namespace higan::Core {
  // <emulator/platform.hpp> forward declarations (dependent on <emulator/node.hpp>)
  static auto PlatformAttach(Node::Object) -> void;
  static auto PlatformDetach(Node::Object) -> void;
}

#include <emulator/node/property.hpp>
#include <emulator/node/class.hpp>
#include <emulator/node/object.hpp>
#include <emulator/node/system.hpp>
#include <emulator/node/component.hpp>
#include <emulator/node/video.hpp>
#include <emulator/node/audio.hpp>
#include <emulator/node/peripheral.hpp>
#include <emulator/node/port.hpp>
#include <emulator/node/input.hpp>
#include <emulator/node/setting.hpp>

namespace higan::Node {
  static inline auto create(string identifier) -> Object {
    return Core::Class::create(identifier);
  }

  static inline auto serialize(Object node) -> string {
    string result;
    node->serialize(result, {});
    return result;
  }

  static inline auto unserialize(string markup) -> Object {
    auto document = BML::unserialize(markup);
    auto node = Core::Class::create(document["node"].text());
    node->unserialize(document["node"]);
    return node;
  }

  static inline auto copy(Object target, string markup) -> void {
    auto source = unserialize(markup);
    target->copy(source);
  }
}
