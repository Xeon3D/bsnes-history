namespace SNES {
  namespace Info {
    static const char Name[] = "bsnes";
    static const char Version[] = "067.21";
    static const unsigned SerializerVersion = 12;
  }
}

//#define DEBUGGER
#define CHEAT_SYSTEM

#include <libco/libco.h>

#include <nall/algorithm.hpp>
#include <nall/any.hpp>
#include <nall/array.hpp>
#include <nall/detect.hpp>
#include <nall/dl.hpp>
#include <nall/endian.hpp>
#include <nall/file.hpp>
#include <nall/foreach.hpp>
#include <nall/function.hpp>
#include <nall/moduloarray.hpp>
#include <nall/platform.hpp>
#include <nall/priorityqueue.hpp>
#include <nall/property.hpp>
#include <nall/serializer.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/utility.hpp>
#include <nall/vector.hpp>
using namespace nall;

#ifdef DEBUGGER
  #define debugvirtual virtual
#else
  #define debugvirtual
#endif

namespace SNES {
  typedef int8_t   int8;
  typedef int16_t  int16;
  typedef int32_t  int32;
  typedef int64_t  int64;
  typedef uint8_t  uint8;
  typedef uint16_t uint16;
  typedef uint32_t uint32;
  typedef uint64_t uint64;

  struct Processor {
    cothread_t thread;
    unsigned frequency;
    int64 clock;

    inline void create(void (*entrypoint_)(), unsigned frequency_) {
      if(thread) co_delete(thread);
      thread = co_create(65536 * sizeof(void*), entrypoint_);
      frequency = frequency_;
      clock = 0;
    }

    inline void serialize(serializer &s) {
      s.integer(frequency);
      s.integer(clock);
    }

    inline Processor() : thread(0) {}
  };

  struct ChipDebugger {
    virtual bool property(unsigned id, string &name, string &value) = 0;
  };

  #include <memory/memory.hpp>
  #include <cpu/core/core.hpp>
  #include <smp/core/core.hpp>
  #include <ppu/counter/counter.hpp>

  #if defined(PROFILE_RESEARCH)
  #include "profile-research.hpp"
  #elif defined(PROFILE_BASELINE)
  #include "profile-baseline.hpp"
  #elif defined(PROFILE_PERFORMANCE)
  #include "profile-performance.hpp"
  #endif

  #include <system/system.hpp>
  #include <chip/chip.hpp>
  #include <cartridge/cartridge.hpp>
  #include <cheat/cheat.hpp>

  #include <memory/memory-inline.hpp>
  #include <ppu/counter/counter-inline.hpp>
  #include <cheat/cheat-inline.hpp>
}

namespace nall {
  template<> struct has_size<SNES::MappedRAM> { enum { value = true }; };
  template<> struct has_size<SNES::StaticRAM> { enum { value = true }; };
}

#undef debugvirtual
