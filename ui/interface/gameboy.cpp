bool InterfaceGameBoy::loadCartridge(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) return false;

  interface->baseName = nall::basename(filename);
  GameBoyCartridge info(data, size);
  GameBoy::Interface::loadCartridge(info.xml, data, size);

  delete[] data;
  return true;
}

void InterfaceGameBoy::unloadCartridge() {
  GameBoy::Interface::unloadCartridge();
  interface->baseName = "";
}

bool InterfaceGameBoy::saveState(const string &filename) {
  serializer s = serialize();
  return file::write(filename, s.data(), s.size());
}

bool InterfaceGameBoy::loadState(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) return false;
  serializer s(data, size);
  delete[] data;
  return unserialize(s);
}

//

void InterfaceGameBoy::videoRefresh(const uint8_t *data) {
  interface->videoRefresh();

  uint32_t *output;
  unsigned outpitch;
  if(video.lock(output, outpitch, 160, 144)) {
    for(unsigned y = 0; y < 144; y++) {
      const uint8_t *sp = data + y * 160;
      uint32_t *dp = output + y * (outpitch >> 2);
      for(unsigned x = 0; x < 160; x++) {
        uint32_t color = *sp++;
        *dp++ = (color << 16) | (color << 8) | (color << 0);
      }
    }

    video.unlock();
    video.refresh();
  }
}

void InterfaceGameBoy::audioSample(int16_t csample, int16_t lsample, int16_t rsample) {
  signed samples[] = { lsample, rsample };
  dspaudio.sample(samples);
  while(dspaudio.pending()) {
    dspaudio.read(samples);
    audio.sample(samples[0], samples[1]);
  }
}

bool InterfaceGameBoy::inputPoll(unsigned id) {
  return inputManager->gameBoy.device.controller.poll(id);
}
