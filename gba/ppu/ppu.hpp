struct PPU : Thread, MMIO {
  StaticMemory vram;
  StaticMemory pram;
  #include "registers.hpp"
  #include "state.hpp"
  uint16 *output;

  static void Enter();
  void enter();
  void step(unsigned clocks);

  void power();
  void scanline();
  void frame();

  uint8 read(uint32 addr);
  void write(uint32 addr, uint8 byte);

  void render_backgrounds();
  void render_background_linear(unsigned bgnumber);
  void render_background_affine(unsigned bgnumber);
  void render_background_bitmap(unsigned bgnumber);

  void render_objects();
  void render_object_linear(Object&);
  void render_object_affine(Object&);
  uint32 oam_read(uint32 addr, uint32 size);
  void oam_write(uint32 addr, uint32 size, uint32 word);
  uint8 oam_read(uint32 addr);
  void oam_write(uint32 addr, uint8 byte);

  uint15 palette(uint9 index);
  void render_forceblank();
  void render_screen();

  PPU();
  ~PPU();
};

extern PPU ppu;
