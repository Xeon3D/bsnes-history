bool RasterSettingsWindow::message(uint id, uintptr_t param) {
ui::Control *control = (ui::Control*)param;
  if(id == ui::Message::Changed) {
    if(control == &contrast) {
      if(config::snes.contrast != contrast.get_position() - 96) {
        config::snes.contrast = contrast.get_position() - 96;
        lcontrast.set_text(string() << "Contrast: " << config::snes.contrast);
        snes.update_color_lookup_table();
      }
    } else if(control == &brightness) {
      if(config::snes.brightness != brightness.get_position() - 96) {
        config::snes.brightness = brightness.get_position() - 96;
        lbrightness.set_text(string() << "Brightness: " << config::snes.brightness);
        snes.update_color_lookup_table();
      }
    } else if(control == &gamma) {
      if(config::snes.gamma != gamma.get_position() + 10) {
        config::snes.gamma = gamma.get_position() + 10;
        lgamma.set_text(string() << "Gamma: " << config::snes.gamma); //TODO: print gamma as "%0.2f" / 100.0
        snes.update_color_lookup_table();
      }
    }
    return true;
  }

  if(id == ui::Message::Clicked) {
    if(control == &gamma_ramp) {
      if(config::snes.gamma_ramp != gamma_ramp.checked()) {
        config::snes.gamma_ramp = gamma_ramp.checked();
        snes.update_color_lookup_table();
      }
    } else if(control == &sepia) {
      if(config::snes.sepia != sepia.checked()) {
        config::snes.sepia = sepia.checked();
        snes.update_color_lookup_table();
      }
    } else if(control == &grayscale) {
      if(config::snes.grayscale != grayscale.checked()) {
        config::snes.grayscale = grayscale.checked();
        snes.update_color_lookup_table();
      }
    } else if(control == &invert) {
      if(config::snes.invert != invert.checked()) {
        config::snes.invert = invert.checked();
        snes.update_color_lookup_table();
      }
    } else if(control == &preset_optimal) {
      config::snes.contrast   = 0;
      config::snes.brightness = 0;
      config::snes.gamma      = 80;
      config::snes.gamma_ramp = true;
      config::snes.sepia      = false;
      config::snes.grayscale  = false;
      config::snes.invert     = false;
      sync_ui();
    } else if(control == &preset_standard) {
      config::snes.contrast   = 0;
      config::snes.brightness = 0;
      config::snes.gamma      = 100;
      config::snes.gamma_ramp = false;
      config::snes.sepia      = false;
      config::snes.grayscale  = false;
      config::snes.invert     = false;
      sync_ui();
    }
    return true;
  }

  return true;
}

//update all UI controls to match config file values ...
void RasterSettingsWindow::sync_ui() {
  contrast.set_position(config::snes.contrast + 96);
  lcontrast.set_text(string() << "Contrast: " << config::snes.contrast);
  brightness.set_position(config::snes.brightness + 96);
  lbrightness.set_text(string() << "Brightness: " << config::snes.brightness);
  gamma.set_position(config::snes.gamma - 10);
  lgamma.set_text(string() << "Gamma: " << config::snes.gamma); //TODO: print gamma as "%0.2f" / 100.0
  gamma_ramp.check(config::snes.gamma_ramp);
  sepia.check(config::snes.sepia);
  grayscale.check(config::snes.grayscale);
  invert.check(config::snes.invert);
  snes.update_color_lookup_table();
}

void RasterSettingsWindow::setup() {
  create(0, 475, 355);

int x = 0, y = 0;
  lcontrast.create(*this, 0, x,       y, 100, 25);
  contrast.create (*this, 0, x + 100, y, 375, 25, 192);
  y += 25;

  lbrightness.create(*this, 0, x,       y, 100, 25);
  brightness.create (*this, 0, x + 100, y, 375, 25, 192);
  y += 25;

  lgamma.create(*this, 0, x,       y, 100, 25);
  gamma.create (*this, 0, x + 100, y, 375, 25, 191);
  y += 25;

  gamma_ramp.create(*this, 0, x, y, 235, 20, "Gamma ramp");
  sepia.create(*this, 0, x + 240, y, 235, 20, "Sepia");
  y += 20;

  grayscale.create(*this, 0, x, y, 235, 20, "Grayscale");
  invert.create(*this, 0, x + 240, y, 235, 20, "Invert colors");
  y += 20;

  y += 5;
  preset_optimal.create (*this, 0, x,       y, 235, 30, "Optimal Preset");
  preset_standard.create(*this, 0, x + 240, y, 235, 30, "Standard Preset");

  sync_ui();
}
