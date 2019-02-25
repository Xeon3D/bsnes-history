//Texas Instruments TMS9918A (derivative)

struct VDP : Thread {
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto refresh() -> void;

  auto vlines() -> uint;
  auto vblank() -> bool;

  auto power() -> void;

  //io.cpp
  auto vcounter() -> uint8;
  auto hcounter() -> uint8;
  auto data() -> uint8;
  auto status() -> uint8;

  auto data(uint8) -> void;
  auto control(uint8) -> void;
  auto registerWrite(uint4 addr, uint8 data) -> void;

  //background.cpp
  struct Background {
    auto run(uint8 hoffset, uint9 voffset) -> void;
    auto graphics1(uint8 hoffset, uint9 voffset) -> void;
    auto graphics2(uint8 hoffset, uint9 voffset) -> void;
    auto graphics3(uint8 hoffset, uint9 voffset, uint vlines) -> void;

    auto power() -> void;

    //serialization.cpp
    auto serialize(serializer&) -> void;

    struct Output {
      uint4 color;
      uint1 palette;
      uint1 priority;
    } output;
  } background;

  //sprite.cpp
  struct Sprite {
    auto setup(uint9 voffset) -> void;
    auto run(uint8 hoffset, uint9 voffset) -> void;
    auto graphics1(uint8 hoffset, uint9 voffset) -> void;
    auto graphics2(uint8 hoffset, uint9 voffset) -> void;
    auto graphics3(uint8 hoffset, uint9 voffset, uint vlines) -> void;

    auto power() -> void;

    //serialization.cpp
    auto serialize(serializer&) -> void;

    struct Object {
      uint8 x;
      uint8 y;
      uint8 pattern;
      uint4 color;
    };

    struct Output {
      uint4 color;
    } output;

    array<Object[8]> objects;
    uint objectsValid;
  } sprite;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  auto palette(uint5 index) -> uint12;

  uint32 buffer[256 * 264];
  uint8 vram[0x4000];
  uint8 cram[0x40];  //SG + MS = 0x20, GG = 0x40

  struct IO {
    uint vcounter = 0;  //vertical counter
    uint hcounter = 0;  //horizontal counter
    uint lcounter = 0;  //line counter

    //interrupt flags
    bool intLine = 0;
    bool intFrame = 0;

    //status flags
    bool spriteOverflow = 0;
    bool spriteCollision = 0;
    uint5 fifthSprite;

    //latches
    bool controlLatch = 0;
    uint16 controlData;
    uint2 code;
    uint14 address;

    uint8 vramLatch;

    //$00  mode control 1
    bool externalSync = 0;
    bool spriteShift = 0;
    bool lineInterrupts = 0;
    bool leftClip = 0;
    bool horizontalScrollLock = 0;
    bool verticalScrollLock = 0;

    //$01  mode control 2
    bool spriteDouble = 0;
    bool spriteTile = 0;
    bool frameInterrupts = 0;
    bool displayEnable = 0;

    //$00 + $01
    uint4 mode;

    //$02  name table base address
    uint4 nameTableAddress;

    //$03  color table base address
    uint8 colorTableAddress;

    //$04  pattern table base address
    uint3 patternTableAddress;

    //$05  sprite attribute table base address
    uint7 spriteAttributeTableAddress;

    //$06  sprite pattern table base address
    uint3 spritePatternTableAddress;

    //$07  backdrop color
    uint4 backdropColor;

    //$08  horizontal scroll offset
    uint8 hscroll;

    //$09  vertical scroll offset
    uint8 vscroll;

    //$0a  line counter
    uint8 lineCounter;
  } io;
};

extern VDP vdp;
