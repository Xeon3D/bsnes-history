bool InterfaceSNES::loadCartridge(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) return false;

  interface->baseName = nall::basename(filename);
  string xml = SNESCartridge(data, size).xmlMemoryMap;
  SNES::Interface::loadCartridge(xml, data, size);

  delete[] data;
  return true;
}

void InterfaceSNES::unloadCartridge() {
  SNES::Interface::unloadCartridge();
  interface->baseName = "";
}

bool InterfaceSNES::saveState(const string &filename) {
  serializer s = serialize();
  return file::write(filename, s.data(), s.size());
}

bool InterfaceSNES::loadState(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) return false;
  serializer s(data, size);
  delete[] data;
  return unserialize(s);
}

//

void InterfaceSNES::videoRefresh(const uint16_t *data, bool hires, bool interlace, bool overscan) {
  interface->video_refresh();

  unsigned width = hires ? 512 : 256;
  unsigned height = 0 ? 224 : 239;
  if(interlace) height <<= 1;
  unsigned inpitch = interlace ? 1024 : 2048;

  if(0) {  //NTSC
    if(overscan == false) data +=  9 * 1024;  // 0 + 224 +  0
    if(overscan == true ) data += 16 * 1024;  //-7 + 224 + -7
  }

  if(1) {  //PAL
    if(overscan == false) data +=  1 * 1024;  // 8 + 224 +  7
    if(overscan == true ) data +=  9 * 1024;  // 0 + 239 +  0
  }

  uint32_t *output;
  unsigned outpitch;
  if(video.lock(output, outpitch, width, height)) {
    for(unsigned y = 0; y < height; y++) {
      const uint16_t *sp = data + y * (inpitch >> 1);
      uint32_t *dp = output + y * (outpitch >> 2);
      for(unsigned x = 0; x < width; x++) {
        uint32_t color = *sp++;
        color = ((color & 0x7c00) << 9) | ((color & 0x03e0) << 6) | ((color & 0x001f) << 3);
        *dp++ = color | ((color >> 3) & 0x070707);
      }
    }

    video.unlock();
    video.refresh();
  }
}

void InterfaceSNES::audioSample(int16_t lsample, int16_t rsample) {
  signed samples[] = { lsample, rsample };
  dspaudio.sample(samples);
  while(dspaudio.pending()) {
    dspaudio.read(samples);
    audio.sample(samples[0], samples[1]);
  }
}

int16_t InterfaceSNES::inputPoll(bool port, SNES::Input::Device device, unsigned index, unsigned id) {
  if(port == 0 && device == SNES::Input::Device::Joypad) return inputManager->snes.port1.gamepad.poll(id);
  return 0;
}

string InterfaceSNES::path(SNES::Cartridge::Slot slot, const string &hint) {
  return dir(interface->baseName);
}
