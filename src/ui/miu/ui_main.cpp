bool MainWindow::input_ready() {
  return focused() == true; //only allow SNES to recognize input when main window is focused
}

uintptr_t MainWindow::event(Event e) {
  if(e.type == Event::Close) {
    _term_ = true;
    hide();
    return false;
  }

  if(e.type == Event::Tick) {
    if(e.widget == &menu_file_load) {
      event::load_rom();
    }

    if(e.widget == &menu_file_load_bsx) {
      window_bsxloader.mode = BSXLoaderWindow::ModeBSX;
      window_bsxloader.set_text("Load BS-X Cartridge");
      window_bsxloader.tbase.set_text(config::path.bsx);
      window_bsxloader.focus();
    }

    if(e.widget == &menu_file_load_bsc) {
      window_bsxloader.mode = BSXLoaderWindow::ModeBSC;
      window_bsxloader.set_text("Load BS-X Slotted Cartridge");
      window_bsxloader.tbase.set_text("");
      window_bsxloader.focus();
    }

    if(e.widget == &menu_file_load_st) {
      window_stloader.tbase.set_text(config::path.st);
      window_stloader.focus();
    }

    if(e.widget == &menu_file_unload) {
      event::unload_rom();
    }

    if(e.widget == &menu_file_reset) {
      event::reset();
    }

    if(e.widget == &menu_file_power) {
      event::power();
    }

    if(e.widget == &menu_file_exit) {
      event(Event(Event::Close));
    }

    if(locked == false) {
    //set locked to true to update below menu item check statuses without triggering events
      if(e.widget == &menu_settings_videomode_1x) { event::update_multiplier(1); }
      if(e.widget == &menu_settings_videomode_2x) { event::update_multiplier(2); }
      if(e.widget == &menu_settings_videomode_3x) { event::update_multiplier(3); }
      if(e.widget == &menu_settings_videomode_4x) { event::update_multiplier(4); }
      if(e.widget == &menu_settings_videomode_5x) { event::update_multiplier(5); }

      if(e.widget == &menu_settings_videomode_aspect_correction) {
        event::update_aspect_correction(menu_settings_videomode_aspect_correction.checked());
      }

      if(e.widget == &menu_settings_videomode_ntsc) { event::update_region(0); }
      if(e.widget == &menu_settings_videomode_pal)  { event::update_region(1); }

      if(e.widget == &menu_settings_videofilter_hwpoint)  { event::update_hardware_filter(0); }
      if(e.widget == &menu_settings_videofilter_hwlinear) { event::update_hardware_filter(1); }

      if(e.widget == &menu_settings_videofilter_swnone)    { event::update_software_filter(0); }
      if(e.widget == &menu_settings_videofilter_swntsc)    { event::update_software_filter(1); }
      if(e.widget == &menu_settings_videofilter_swhq2x)    { event::update_software_filter(2); }
      if(e.widget == &menu_settings_videofilter_swscale2x) { event::update_software_filter(3); }

      if(e.widget == &menu_settings_videoframeskip_0) { config::video.frameskip = 0; }
      if(e.widget == &menu_settings_videoframeskip_1) { config::video.frameskip = 1; }
      if(e.widget == &menu_settings_videoframeskip_2) { config::video.frameskip = 2; }
      if(e.widget == &menu_settings_videoframeskip_3) { config::video.frameskip = 3; }
      if(e.widget == &menu_settings_videoframeskip_4) { config::video.frameskip = 4; }
      if(e.widget == &menu_settings_videoframeskip_5) { config::video.frameskip = 5; }
      if(e.widget == &menu_settings_videoframeskip_6) { config::video.frameskip = 6; }
      if(e.widget == &menu_settings_videoframeskip_7) { config::video.frameskip = 7; }
      if(e.widget == &menu_settings_videoframeskip_8) { config::video.frameskip = 8; }
      if(e.widget == &menu_settings_videoframeskip_9) { config::video.frameskip = 9; }

      if(e.widget == &menu_settings_mute) {
        config::snes.mute = menu_settings_mute.checked();
      }

      if(e.widget == &menu_settings_speedreg_enable) {
        config::system.regulate_speed = menu_settings_speedreg_enable.checked();
        if(uiAudio) uiAudio->set(Audio::Synchronize, config::system.regulate_speed);
      }

      if(e.widget == &menu_settings_speedreg_slowest) { event::update_speed_regulation(1); }
      if(e.widget == &menu_settings_speedreg_slow)    { event::update_speed_regulation(2); }
      if(e.widget == &menu_settings_speedreg_normal)  { event::update_speed_regulation(3); }
      if(e.widget == &menu_settings_speedreg_fast)    { event::update_speed_regulation(4); }
      if(e.widget == &menu_settings_speedreg_fastest) { event::update_speed_regulation(5); }
    }

    if(e.widget == &menu_settings_config) { window_settings.show(); }

    if(e.widget == &menu_misc_logaudio) {
      (menu_misc_logaudio.checked() == true) ? snes.log_audio_enable() : snes.log_audio_disable();
    }

    if(e.widget == &menu_misc_showfps) {
      config::misc.show_frame_counter = menu_misc_showfps.checked();
      if(config::misc.show_frame_counter == false) {
        set_text(BSNES_TITLE);
      }
    }

    if(e.widget == &menu_misc_about) {
      window_about.focus();
    }
  }

  return true;
}

uintptr_t MainWindow::block(Event) {
  if(uiAudio) uiAudio->clear_audio();
  return true;
}

uintptr_t MainWindow::keydown(Event e) {
  if(e.param == keymap::esc) { event::toggle_menu(); }
  if(e.param == keymap::f11) { event::toggle_fullscreen(); }
}

void MainWindow::setup() {
  snesinterface.input_ready = bind(&MainWindow::input_ready, this);
  locked = true;

  create(Window::AutoCenter, 256, 224, BSNES_TITLE);
  set_background_color(0, 0, 0);

MenuRadioItemGroup group;
  attach(menu_file.create("File"));
    menu_file.attach(menu_file_load.create("Load Cartridge ..."));
    menu_file.attach(menu_file_load_special.create("Load Special"));
      menu_file_load_special.attach(menu_file_load_bsx.create("Load BS-X Cartridge ..."));
      menu_file_load_special.attach(menu_file_load_bsc.create("Load BS-X Slotted Cartridge ..."));
      menu_file_load_special.attach(menu_file_load_st.create("Load ST Cartridge ..."));
    menu_file.attach(menu_file_unload.create("Unload Cartridge"));
    menu_file.attach(menu_file_sep1.create());
    menu_file.attach(menu_file_reset.create("Reset System"));
    menu_file.attach(menu_file_power.create("Power Cycle System"));
    menu_file.attach(menu_file_sep2.create());
    menu_file.attach(menu_file_exit.create("Exit"));

  attach(menu_settings.create("Settings"));
    menu_settings.attach(menu_settings_videomode.create("Video Mode"));
      group.add(&menu_settings_videomode_1x);
      group.add(&menu_settings_videomode_2x);
      group.add(&menu_settings_videomode_3x);
      group.add(&menu_settings_videomode_4x);
      group.add(&menu_settings_videomode_5x);
      menu_settings_videomode.attach(menu_settings_videomode_1x.create(group, "Scale 1x"));
      menu_settings_videomode.attach(menu_settings_videomode_2x.create(group, "Scale 2x"));
      menu_settings_videomode.attach(menu_settings_videomode_3x.create(group, "Scale 3x"));
      menu_settings_videomode.attach(menu_settings_videomode_4x.create(group, "Scale 4x"));
      menu_settings_videomode.attach(menu_settings_videomode_5x.create(group, "Scale 5x"));
      group.reset();
      menu_settings_videomode.attach(menu_settings_videomode_sep1.create());
      menu_settings_videomode.attach(menu_settings_videomode_aspect_correction.create("Correct Aspect Ratio"));
      menu_settings_videomode.attach(menu_settings_videomode_sep2.create());
      group.add(&menu_settings_videomode_ntsc);
      group.add(&menu_settings_videomode_pal);
      menu_settings_videomode.attach(menu_settings_videomode_ntsc.create(group, "NTSC"));
      menu_settings_videomode.attach(menu_settings_videomode_pal.create(group, "PAL"));
      group.reset();

    menu_settings.attach(menu_settings_videofilter.create("Video Filter"));
      group.add(&menu_settings_videofilter_hwpoint);
      group.add(&menu_settings_videofilter_hwlinear);
      menu_settings_videofilter.attach(menu_settings_videofilter_hwpoint.create(group, "Point"));
      menu_settings_videofilter.attach(menu_settings_videofilter_hwlinear.create(group, "Linear"));
      group.reset();
      menu_settings_videofilter.attach(menu_settings_videofilter_sep1.create());
      group.add(&menu_settings_videofilter_swnone);
      group.add(&menu_settings_videofilter_swntsc);
      group.add(&menu_settings_videofilter_swhq2x);
      group.add(&menu_settings_videofilter_swscale2x);
      menu_settings_videofilter.attach(menu_settings_videofilter_swnone.create(group, "None"));
      menu_settings_videofilter.attach(menu_settings_videofilter_swntsc.create(group, "NTSC"));
      menu_settings_videofilter.attach(menu_settings_videofilter_swhq2x.create(group, "HQ2x"));
      menu_settings_videofilter.attach(menu_settings_videofilter_swscale2x.create(group, "Scale2x"));
      group.reset();

    menu_settings.attach(menu_settings_videoframeskip.create("Video Frameskip"));
      group.add(&menu_settings_videoframeskip_0);
      group.add(&menu_settings_videoframeskip_1);
      group.add(&menu_settings_videoframeskip_2);
      group.add(&menu_settings_videoframeskip_3);
      group.add(&menu_settings_videoframeskip_4);
      group.add(&menu_settings_videoframeskip_5);
      group.add(&menu_settings_videoframeskip_6);
      group.add(&menu_settings_videoframeskip_7);
      group.add(&menu_settings_videoframeskip_8);
      group.add(&menu_settings_videoframeskip_9);
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_0.create(group, "0"));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_sep1.create());
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_1.create(group, "1"));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_2.create(group, "2"));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_3.create(group, "3"));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_4.create(group, "4"));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_5.create(group, "5"));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_6.create(group, "6"));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_7.create(group, "7"));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_8.create(group, "8"));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_9.create(group, "9"));
      group.reset();

    menu_settings.attach(menu_settings_sep1.create());
    menu_settings.attach(menu_settings_mute.create("Mute Sound Output"));
    menu_settings.attach(menu_settings_sep2.create());

    menu_settings.attach(menu_settings_speedreg.create("Speed Regulation"));
      menu_settings_speedreg.attach(menu_settings_speedreg_enable.create("Enable"));
      menu_settings_speedreg.attach(menu_settings_speedreg_sep1.create());
      group.add(&menu_settings_speedreg_slowest);
      group.add(&menu_settings_speedreg_slow);
      group.add(&menu_settings_speedreg_normal);
      group.add(&menu_settings_speedreg_fast);
      group.add(&menu_settings_speedreg_fastest);
      menu_settings_speedreg.attach(menu_settings_speedreg_slowest.create(group, "Slowest"));
      menu_settings_speedreg.attach(menu_settings_speedreg_slow.create(group, "Slow"));
      menu_settings_speedreg.attach(menu_settings_speedreg_normal.create(group, "Normal"));
      menu_settings_speedreg.attach(menu_settings_speedreg_fast.create(group, "Fast"));
      menu_settings_speedreg.attach(menu_settings_speedreg_fastest.create(group, "Fastest"));
      group.reset();

    menu_settings.attach(menu_settings_sep3.create());
    menu_settings.attach(menu_settings_config.create("Configuration ..."));

  attach(menu_misc.create("Misc"));
    menu_misc.attach(menu_misc_logaudio.create("Log Audio Data"));
    menu_misc.attach(menu_misc_showfps.create("Show FPS"));
    menu_misc.attach(menu_misc_sep1.create());
    menu_misc.attach(menu_misc_about.create("About ..."));

  view.create(0, 256, 224);
  attach(view, 0, 0);

  on_close = bind(&MainWindow::event, this);
  on_block = bind(&MainWindow::block, this);
  on_keydown = bind(&MainWindow::keydown, this);

  menu_file_load.on_tick =
  menu_file_load_bsx.on_tick =
  menu_file_load_bsc.on_tick =
  menu_file_load_st.on_tick =
  menu_file_unload.on_tick =
  menu_file_reset.on_tick =
  menu_file_power.on_tick =
  menu_file_exit.on_tick =

  menu_settings_videomode_1x.on_tick =
  menu_settings_videomode_2x.on_tick =
  menu_settings_videomode_3x.on_tick =
  menu_settings_videomode_4x.on_tick =
  menu_settings_videomode_5x.on_tick =
  menu_settings_videomode_aspect_correction.on_tick =
  menu_settings_videomode_ntsc.on_tick =
  menu_settings_videomode_pal.on_tick =

  menu_settings_videofilter_hwpoint.on_tick =
  menu_settings_videofilter_hwlinear.on_tick =
  menu_settings_videofilter_swnone.on_tick =
  menu_settings_videofilter_swntsc.on_tick =
  menu_settings_videofilter_swhq2x.on_tick =
  menu_settings_videofilter_swscale2x.on_tick =

  menu_settings_videoframeskip_0.on_tick =
  menu_settings_videoframeskip_1.on_tick =
  menu_settings_videoframeskip_2.on_tick =
  menu_settings_videoframeskip_3.on_tick =
  menu_settings_videoframeskip_4.on_tick =
  menu_settings_videoframeskip_5.on_tick =
  menu_settings_videoframeskip_6.on_tick =
  menu_settings_videoframeskip_7.on_tick =
  menu_settings_videoframeskip_8.on_tick =
  menu_settings_videoframeskip_9.on_tick =

  menu_settings_mute.on_tick =
  menu_settings_speedreg_enable.on_tick =
  menu_settings_speedreg_slowest.on_tick =
  menu_settings_speedreg_slow.on_tick =
  menu_settings_speedreg_normal.on_tick =
  menu_settings_speedreg_fast.on_tick =
  menu_settings_speedreg_fastest.on_tick =
  menu_settings_config.on_tick =

  menu_misc_logaudio.on_tick =
  menu_misc_showfps.on_tick =
  menu_misc_about.on_tick =

  bind(&MainWindow::event, this);
}

void MainWindow::update_menu_settings() {
  locked = true;
  event::load_video_settings();

  switch(event::video_settings.multiplier) { default:
    case 1: menu_settings_videomode_1x.check(); break;
    case 2: menu_settings_videomode_2x.check(); break;
    case 3: menu_settings_videomode_3x.check(); break;
    case 4: menu_settings_videomode_4x.check(); break;
    case 5: menu_settings_videomode_5x.check(); break;
  }

  menu_settings_videomode_aspect_correction.check(event::video_settings.aspect_correction);

  switch(event::video_settings.region) { default:
    case 0: menu_settings_videomode_ntsc.check(); break;
    case 1: menu_settings_videomode_pal.check(); break;
  }

  switch(event::video_settings.hardware_filter) { default:
    case 0: menu_settings_videofilter_hwpoint.check(); break;
    case 1: menu_settings_videofilter_hwlinear.check(); break;
  }

  switch(event::video_settings.software_filter) { default:
    case 0: menu_settings_videofilter_swnone.check(); break;
    case 1: menu_settings_videofilter_swntsc.check(); break;
    case 2: menu_settings_videofilter_swhq2x.check(); break;
    case 3: menu_settings_videofilter_swscale2x.check(); break;
  }

  menu_settings_mute.check(config::snes.mute);

  menu_settings_speedreg_enable.check(config::system.regulate_speed);
  menu_settings_speedreg_normal.check();

  menu_misc_showfps.check(config::misc.show_frame_counter);

  locked = false;
}
