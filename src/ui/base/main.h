class MainWindow : public Window {
public:
  MenuGroup menu_file;
    MenuItem menu_file_load;
    MenuGroup menu_file_load_special;
      MenuItem menu_file_load_bsx;
      MenuItem menu_file_load_bsc;
      MenuItem menu_file_load_st;
    MenuItem menu_file_unload;
    MenuSeparator menu_file_sep1;
    MenuItem menu_file_reset;
    MenuItem menu_file_power;
    MenuSeparator menu_file_sep2;
    MenuGroup menu_system_controller_port1;
      MenuRadioItem menu_system_controller_port1_none;
      MenuRadioItem menu_system_controller_port1_joypad;
      MenuRadioItem menu_system_controller_port1_multitap;
    MenuGroup menu_system_controller_port2;
      MenuRadioItem menu_system_controller_port2_none;
      MenuRadioItem menu_system_controller_port2_joypad;
      MenuRadioItem menu_system_controller_port2_multitap;
    MenuSeparator menu_file_sep3;
    MenuItem menu_file_exit;

  MenuGroup menu_settings;
    MenuGroup menu_settings_videomode;
      MenuRadioItem menu_settings_videomode_1x;
      MenuRadioItem menu_settings_videomode_2x;
      MenuRadioItem menu_settings_videomode_3x;
      MenuRadioItem menu_settings_videomode_4x;
      MenuRadioItem menu_settings_videomode_5x;
      MenuSeparator menu_settings_videomode_sep1;
      MenuCheckItem menu_settings_videomode_aspect_correction;
      MenuSeparator menu_settings_videomode_sep2;
      MenuRadioItem menu_settings_videomode_ntsc;
      MenuRadioItem menu_settings_videomode_pal;
    MenuGroup menu_settings_videofilter;
      MenuRadioItem menu_settings_videofilter_hwpoint;
      MenuRadioItem menu_settings_videofilter_hwlinear;
      MenuSeparator menu_settings_videofilter_sep1;
      MenuRadioItem menu_settings_videofilter_swnone;
      MenuRadioItem menu_settings_videofilter_swscanline;
      MenuRadioItem menu_settings_videofilter_swscale2x;
      MenuRadioItem menu_settings_videofilter_swhq2x;
      MenuRadioItem menu_settings_videofilter_swntsc;
    MenuGroup menu_settings_videoframeskip;
      MenuRadioItem menu_settings_videoframeskip_0;
      MenuRadioItem menu_settings_videoframeskip_1;
      MenuRadioItem menu_settings_videoframeskip_2;
      MenuRadioItem menu_settings_videoframeskip_3;
      MenuRadioItem menu_settings_videoframeskip_4;
      MenuRadioItem menu_settings_videoframeskip_5;
      MenuRadioItem menu_settings_videoframeskip_6;
      MenuRadioItem menu_settings_videoframeskip_7;
      MenuRadioItem menu_settings_videoframeskip_8;
      MenuRadioItem menu_settings_videoframeskip_9;
    MenuSeparator menu_settings_sep1;
    MenuCheckItem menu_settings_mute;
    MenuSeparator menu_settings_sep2;
    MenuGroup menu_settings_emuspeed;
      MenuRadioItem menu_settings_emuspeed_slowest;
      MenuRadioItem menu_settings_emuspeed_slow;
      MenuRadioItem menu_settings_emuspeed_normal;
      MenuRadioItem menu_settings_emuspeed_fast;
      MenuRadioItem menu_settings_emuspeed_fastest;
      MenuSeparator menu_settings_emuspeed_sep1;
      MenuCheckItem menu_settings_emuspeed_videosync;
      MenuCheckItem menu_settings_emuspeed_audiosync;
    MenuItem menu_settings_config;

  MenuGroup menu_misc;
    MenuCheckItem menu_misc_logaudio;
    MenuSeparator menu_misc_sep1;
    MenuItem menu_misc_about;

  Canvas view;

  bool input_ready();
  void setup();
  void sync();

  uintptr_t close(event_t);
  uintptr_t event(event_t);
  uintptr_t block(event_t);
} window_main;
