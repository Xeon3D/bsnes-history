enum : uint { OBJ = 0, BG0 = 1, BG1 = 2, BG2 = 3, BG3 = 4, SFX = 5 };
enum : uint { In0 = 0, In1 = 1, Obj = 2, Out = 3 };

struct Registers {
  struct Control {
    uint3 bgmode;
    uint1 cgbmode;
    uint1 frame;
    uint1 hblank;
    uint1 objmapping;
    uint1 forceblank;
    uint1 enable[5];
    uint1 enablewindow[3];

    operator uint16_t() const;
    auto operator=(uint16 source) -> uint16;
    auto operator=(const Control&) -> Control& = delete;
  } control;

  uint1 greenswap;

  struct Status {
    uint1 vblank;
    uint1 hblank;
    uint1 vcoincidence;
    uint1 irqvblank;
    uint1 irqhblank;
    uint1 irqvcoincidence;
    uint8 vcompare;

    operator uint16_t() const;
    auto operator=(uint16 source) -> uint16;
    auto operator=(const Status&) -> Status& = delete;
  } status;

  uint16 vcounter;

  struct BackgroundControl {
    uint2 priority;
    uint2 characterbaseblock;
    uint2 unused;
    uint1 mosaic;
    uint1 colormode;
    uint5 screenbaseblock;
    uint1 affinewrap;  //BG2,3 only
    uint2 screensize;

    operator uint16_t() const;
    auto operator=(uint16 source) -> uint16;
    auto operator=(const BackgroundControl&) -> BackgroundControl& = delete;
  };

  struct Background {
    BackgroundControl control;
    uint9 hoffset;
    uint9 voffset;

    //BG2,3 only
    int16 pa, pb, pc, pd;
    int28 x, y;

    //internal
    int28 lx, ly;
    uint vmosaic;
    uint hmosaic;
    uint id;
  } bg[4];

  struct WindowFlags {
    uint1 enable[6];

    operator uint8_t() const;
    auto operator=(uint8 source) -> uint8;
    auto operator=(const WindowFlags&) -> WindowFlags& = delete;
  };

  struct Window {
    uint8 x1, x2;
    uint8 y1, y2;
  } window[2];

  WindowFlags windowflags[4];

  struct Mosaic {
    uint4 bghsize;
    uint4 bgvsize;
    uint4 objhsize;
    uint4 objvsize;
  } mosaic;

  struct BlendControl {
    uint1 above[6];
    uint1 below[6];
    uint2 mode;

    operator uint16_t() const;
    auto operator=(uint16 source) -> uint16;
    auto operator=(const BlendControl&) -> BlendControl& = delete;
  };

  struct Blend {
    BlendControl control;
    uint5 eva;
    uint5 evb;
    uint5 evy;
  } blend;
} regs;
