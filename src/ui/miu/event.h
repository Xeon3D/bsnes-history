namespace event {

struct VideoSettings {
  uint mode;
  bool synchronize;
  bool aspect_correction;
  uint region;
  uint multiplier;
  uint hardware_filter;
  uint software_filter;
} video_settings;
void load_video_settings();

void update_aspect_correction(bool);
void update_multiplier(uint);
void update_region(uint);
void update_hardware_filter(uint);
void update_software_filter(uint);

void update_speed_regulation(uint);

void update_frame_counter();
void update_video_settings();
void toggle_menu();
void toggle_fullscreen();

bool load_rom(char*);
void load_rom();
void load_cart_normal(const char*);
void load_cart_bsx(const char*, const char*);
void load_cart_bsc(const char*, const char*);
void load_cart_st(const char*, const char*, const char*);
void unload_rom();
void reset();
void power();

};
