Palette palette;
Interface interface;

const uint8_t Palette::gammaRamp[32] = {
  0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
  0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
  0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
  0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff,
};

uint8_t Palette::contrastAdjust(uint8_t input) {
  signed contrast = Palette::contrast - 100;
  signed result = input - contrast + (2 * contrast * input + 127) / 255;
  return max(0, min(255, result));
}

uint8_t Palette::brightnessAdjust(uint8_t input) {
  signed brightness = Palette::brightness - 100;
  signed result = input + brightness;
  return max(0, min(255, result));
}

uint8_t Palette::gammaAdjust(uint8_t input) {
  signed result = (signed)(pow(((double)input / 255.0), (double)gamma / 100.0) * 255.0 + 0.5);
  return max(0, min(255, result));
}

void Palette::update() {
  for(unsigned i = 0; i < 32768; i++) {
    unsigned r = (i >> 10) & 31;
    unsigned g = (i >>  5) & 31;
    unsigned b = (i >>  0) & 31;

    r = (r << 3) | (r >> 2);
    g = (g << 3) | (g >> 2);
    b = (b << 3) | (b >> 2);

    if(useGammaRamp) {
      r = gammaRamp[r >> 3];
      g = gammaRamp[g >> 3];
      b = gammaRamp[b >> 3];
    }

    if(contrast != 100) {
      r = contrastAdjust(r);
      g = contrastAdjust(g);
      b = contrastAdjust(b);
    }

    if(brightness != 100) {
      r = brightnessAdjust(r);
      g = brightnessAdjust(g);
      b = brightnessAdjust(b);
    }

    if(gamma != 100) {
      r = gammaAdjust(r);
      g = gammaAdjust(g);
      b = gammaAdjust(b);
    }

    color[i] = (r << 16) | (g << 8) | (b << 0);
  }
}

Palette::Palette() {
  contrast = 100;
  brightness = 100;
  gamma = 100;
  useGammaRamp = true;
  update();
}

void Interface::video_refresh(const uint16_t *data, unsigned width, unsigned height) {
  bool interlace = (height >= 240);
  bool overscan = (height == 239 || height == 478);
  unsigned inpitch = interlace ? 1024 : 2048;

  uint32_t *buffer;
  unsigned outpitch;

  if(video.lock(buffer, outpitch, width, height)) {
    for(unsigned y = 0; y < height; y++) {
      uint32_t *output = buffer + y * (outpitch >> 2);
      const uint16_t *input = data + y * (inpitch >> 1);
      for(unsigned x = 0; x < width; x++) *output++ = palette.color[*input++];
    }
    video.unlock();
    video.refresh();
  }

  static signed frameCounter = 0;
  static time_t previous, current;
  frameCounter++;

  time(&current);
  if(current != previous) {
    mainWindow.setStatusText(string("FPS: ", frameCounter));
    frameCounter = 0;
    previous = current;
  }
}

void Interface::audio_sample(uint16_t left, uint16_t right) {
  audio.sample(left, right);
}

void Interface::input_poll() {
  input.poll(state);
}

int16_t Interface::input_poll(bool port, SNES::Input::Device device, unsigned index, unsigned id) {
  if(port == 0) {
    if(device == SNES::Input::Device::Joypad) {
      switch(id) {
        case SNES::Input::JoypadID::Up:     return state[keyboard(0)[Keyboard::Up]];
        case SNES::Input::JoypadID::Down:   return state[keyboard(0)[Keyboard::Down]];
        case SNES::Input::JoypadID::Left:   return state[keyboard(0)[Keyboard::Left]];
        case SNES::Input::JoypadID::Right:  return state[keyboard(0)[Keyboard::Right]];
        case SNES::Input::JoypadID::B:      return state[keyboard(0)[Keyboard::Z]];
        case SNES::Input::JoypadID::A:      return state[keyboard(0)[Keyboard::X]];
        case SNES::Input::JoypadID::Y:      return state[keyboard(0)[Keyboard::A]];
        case SNES::Input::JoypadID::X:      return state[keyboard(0)[Keyboard::S]];
        case SNES::Input::JoypadID::L:      return state[keyboard(0)[Keyboard::D]];
        case SNES::Input::JoypadID::R:      return state[keyboard(0)[Keyboard::C]];
        case SNES::Input::JoypadID::Select: return state[keyboard(0)[Keyboard::Apostrophe]];
        case SNES::Input::JoypadID::Start:  return state[keyboard(0)[Keyboard::Return]];
      }
    }
  }
  return 0;
}
