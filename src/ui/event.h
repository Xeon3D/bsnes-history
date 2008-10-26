namespace event {

void input_event(uint16_t);

void acquire();
void unacquire();

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

void update_frameskip(int);
void update_emulation_speed(int);

enum system_state_t { LoadCart, UnloadCart, PowerOn, PowerOff, PowerCycle, Reset };
void modify_system_state(system_state_t);

void update_controller_port1(int);
void update_controller_port2(int);

void update_video_settings();
void update_opacity();

void toggle_fullscreen();
void toggle_menubar();
void toggle_statusbar();

bool load_cart(char*);
void load_cart();
void load_image(const char*);
void load_cart_normal(const char*);
void load_cart_bsc(const char*, const char*);
void load_cart_bsx(const char*, const char*);
void load_cart_st(const char*, const char*, const char*);
void unload_cart();

void quit();

} //namespace event
