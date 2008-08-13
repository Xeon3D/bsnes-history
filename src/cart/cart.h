class Cartridge {
public:
  enum CartridgeType {
    CartridgeNormal,
    CartridgeBSX,
    CartridgeBSC,
    CartridgeSufamiTurbo,
  };

  enum HeaderField {
    CART_NAME = 0x00,
    MAPPER    = 0x15,
    ROM_TYPE  = 0x16,
    ROM_SIZE  = 0x17,
    RAM_SIZE  = 0x18,
    REGION    = 0x19,
    COMPANY   = 0x1a,
    VERSION   = 0x1b,
    ICKSUM    = 0x1c,
    CKSUM     = 0x1e,
    RESETV    = 0x3c,
  };

  enum Region {
    NTSC,
    PAL,
  };

  enum MemoryMapper {
    LoROM,
    HiROM,
    ExLoROM,
    ExHiROM,
    SPC7110ROM,
    BSXROM,
    BSCLoROM,
    BSCHiROM,
    STROM,
  };

  enum DSP1MemoryMapper {
    DSP1Unmapped,
    DSP1LoROM1MB,
    DSP1LoROM2MB,
    DSP1HiROM,
  };

  struct {
    bool loaded;
    char fn[PATH_MAX];
    uint8 *rom, *ram, *rtc;
    uint rom_size, ram_size, rtc_size;
  } cart;

  struct {
    char fn[PATH_MAX];
    uint8 *ram;
    uint ram_size;
  } bs;

  struct {
    char fn[PATH_MAX];
    uint8 *rom, *ram;
    uint rom_size, ram_size;
  } stA, stB;

  struct {
    CartridgeType type;

    uint32 crc32;
    char filename[PATH_MAX * 4];
    bool patched;

    Region region;
    MemoryMapper mapper;
    uint rom_size;
    uint ram_size;

    bool bsxbase;
    bool bsxcart;
    bool bsxflash;
    bool st;
    bool superfx;
    bool sa1;
    bool srtc;
    bool sdd1;
    bool spc7110;
    bool spc7110rtc;
    bool cx4;
    bool dsp1;
    bool dsp2;
    bool dsp3;
    bool dsp4;
    bool obc1;
    bool st010;
    bool st011;
    bool st018;

    DSP1MemoryMapper dsp1_mapper;

    uint header_index;
  } info;

  MemoryMapper mapper();
  Region region();

  void load_cart_normal(const char*);
  void load_cart_bsx(const char*, const char*);
  void load_cart_bsc(const char*, const char*);
  void load_cart_st(const char*, const char*, const char*);

  void unload_cart_normal();
  void unload_cart_bsx();
  void unload_cart_bsc();
  void unload_cart_st();

  bool loaded();
  void load_begin(CartridgeType);
  void load_end();
  bool unload();

  unsigned score_header(unsigned);
  void find_header();
  void read_header();
  void read_extended_header();

  enum CompressionMode {
    CompressionNone,    //always load without compression
    CompressionInspect, //use file header inspection
    CompressionAuto,    //use file extension or file header inspection (configured by user)
  };
  bool load_file(const char *fn, uint8 *&data, uint &size, CompressionMode compression = CompressionNone);
  bool save_file(const char *fn, uint8 *data, uint size);
  bool apply_patch(const uint8_t *pdata, unsigned psize, uint8_t *&data, unsigned &size);
  char* modify_extension(char *filename, const char *extension);
  char* get_base_filename(char *filename);
  char* get_path_filename(char *filename, const char *path, const char *source, const char *extension);
  char* get_patch_filename(const char *source, const char *extension);
  char* get_save_filename(const char *source, const char *extension);
  char* get_cheat_filename(const char *source, const char *extension);

  Cartridge();
  ~Cartridge();

private:
  char patchfn[PATH_MAX];
  char savefn[PATH_MAX];
  char rtcfn[PATH_MAX];
  char cheatfn[PATH_MAX];
};

namespace memory {
  extern MappedRAM cartrom, cartram, cartrtc;
  extern MappedRAM bscram;
  extern MappedRAM stArom, stAram;
  extern MappedRAM stBrom, stBram;
};

extern Cartridge cartridge;
