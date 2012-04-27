#include "../base.hpp"
#include "core.cpp"
#include "nes/nes.cpp"
#include "snes/snes.cpp"
#include "gb/gb.cpp"
#include "gba/gba.cpp"
Interface *interface = nullptr;

Filter filter;

void Filter::render(const uint32_t *input, unsigned inputPitch, unsigned inputWidth, unsigned inputHeight) {
  width = inputWidth, height = inputHeight;
  dl_size(width, height);
  dl_render(data, pitch, input, inputPitch, inputWidth, inputHeight);
}

Filter::Filter() {
  data = new uint32_t[2048 * 2048];
  pitch = 2048 * sizeof(uint32_t);
}

Filter::~Filter() {
  delete[] data;
}

string CartridgePath::filename(const string &folderName, const string &fileName) const {
  if(name.empty()) return "";
  if(folder) return { name, folderName };
  return { name, fileName };
}

string CartridgePath::title() const {
  if(name.empty()) return "";
  if(folder) {
    string title = name;
    title.rtrim<1>("/");
    title = notdir(nall::basename(title));
    return title;
  }
  return notdir(name);
}

void Interface::bindControllers() {
  switch(mode()) {
  case Mode::NES:
    nes.setController(0, config->nes.controllerPort1Device);
    nes.setController(1, config->nes.controllerPort2Device);
    break;

  case Mode::SNES:
    snes.setController(0, config->snes.controllerPort1Device);
    snes.setController(1, config->snes.controllerPort2Device);
    break;
  }
}

void Interface::setController(unsigned port, unsigned device) {
  switch(mode()) {
  case Mode::NES: return nes.setController(port, device);
  case Mode::SNES: return snes.setController(port, device);
  }
}

void Interface::updateDSP() {
  audio.set(Audio::Frequency, config->audio.frequency);
  audio.set(Audio::Latency, config->audio.latency);

  if(config->audio.resampler == "linear" ) dspaudio.setResampler(DSP::ResampleEngine::Linear);
  if(config->audio.resampler == "hermite") dspaudio.setResampler(DSP::ResampleEngine::Hermite);
  if(config->audio.resampler == "sinc"   ) dspaudio.setResampler(DSP::ResampleEngine::Sinc);
  dspaudio.setResamplerFrequency(config->audio.frequency);
  dspaudio.setVolume(config->audio.mute == false ? (double)config->audio.volume / 100.0 : 0.0);

  switch(mode()) {
  case Mode::NES:  return dspaudio.setFrequency(config->audio.frequencyNES);
  case Mode::SNES: return dspaudio.setFrequency(config->audio.frequencySNES);
  case Mode::GB:   return dspaudio.setFrequency(config->audio.frequencyGB);
  case Mode::GBA:  return dspaudio.setFrequency(config->audio.frequencyGBA);
  }
}

string Interface::markup() {
  if(core) return core->markup();
  return "";
}

bool Interface::cartridgeLoaded() {
  if(core) return core->cartridgeLoaded();
  return false;
}

void Interface::loadCartridge(Mode mode) {
  switch(mode) {
  case Mode::NES:  core = &nes; break;
  case Mode::SNES: core = &snes; break;
  case Mode::GB:   core = &gb; break;
  case Mode::GBA:  core = &gba; break;
  default:         core = nullptr; break;
  }
  utility->setMode(this->mode = mode);

  bindControllers();
  cheatEditor->load(game.filename("cheats.xml", ".cht"));
  stateManager->load(game.filename("states.bsa", ".bsa"), 0u);
  dipSwitches->load();
  utility->showMessage({ "Loaded ", cartridgeTitle });
}

bool Interface::loadCartridge(string filename) {
  filename.trim<1>("\"");
  filename.transform("\\", "/");
  bool result = false;
  if(filename.endswith(".nes") || filename.endswith(".nes/")) result = nes.loadCartridge(filename);
  if(filename.endswith(".sfc") || filename.endswith(".sfc/")) result = snes.loadCartridge(filename);
  if(filename.endswith(".gb" ) || filename.endswith(".gb/" )) result = gb.loadCartridge(GB::System::Revision::GameBoy, filename);
  if(filename.endswith(".gbc") || filename.endswith(".gbc/")) result = gb.loadCartridge(GB::System::Revision::GameBoyColor, filename);
  if(filename.endswith(".gba") || filename.endswith(".gba/")) result = gba.loadCartridge(filename);
  return result;
}

void Interface::unloadCartridge() {
  if(cartridgeLoaded() == false) return;
  cheatDatabase->setVisible(false);
  cheatEditor->save(game.filename("cheats.xml", ".cht"));
  stateManager->save(game.filename("states.bsa", ".bsa"), 0u);
  setCheatCodes();

  if(core) core->unloadCartridge();
  core = nullptr;
  cartridgeTitle = "";
  utility->setMode(mode = Mode::None);
}

void Interface::power() {
  if(core == nullptr) return;
  core->power();
  utility->showMessage("System power was cycled");
}

void Interface::reset() {
  if(core == nullptr) return;
  core->reset();
  utility->showMessage("System was reset");
}

void Interface::run() {
  if(core == nullptr) return;
  core->run();
}

serializer Interface::serialize() {
  if(core == nullptr) return serializer();
  return core->serialize();
}

bool Interface::unserialize(serializer &s) {
  if(core == nullptr) return false;
  return core->unserialize(s);
}

bool Interface::saveState(unsigned slot) {
  string filename = game.filename({ "state-", slot, ".bst" }, { "-", slot, ".bst" });
  serializer s = serialize();
  bool result = file::write(filename, s.data(), s.size());
  utility->showMessage(result == true ? string{ "Saved state ", slot } : "Failed to save state");
  return result;
}

bool Interface::loadState(unsigned slot) {
  string filename = game.filename({ "state-", slot, ".bst" }, { "-", slot, ".bst" });
  auto memory = file::read(filename);
  if(memory.empty()) {
    utility->showMessage(string{ "Slot ", slot, " save file not found" });
    return false;
  }
  serializer s(memory.data(), memory.size());
  bool result = unserialize(s);
  utility->showMessage(result == true ? string{ "Loaded state ", slot } : "Failed to load state");
  return result;
}

void Interface::setCheatCodes(const lstring &list) {
  if(core) return core->setCheats(list);
}

void Interface::updatePalette() {
  switch(mode()) {
  case Mode::NES:  return NES::video.generate_palette();
  case Mode::SNES: return SNES::video.generate_palette();
  case Mode::GB:   return GB::video.generate_palette();
  case Mode::GBA:  return GBA::video.generate_palette();
  }
}

string Interface::sha256() {
  switch(mode()) {
  case Mode::NES:  return NES::cartridge.sha256();
  case Mode::SNES: return SNES::cartridge.sha256();
  case Mode::GB:   return GB::cartridge.sha256();
  case Mode::GBA:  return GBA::cartridge.sha256();
  }
  return "{None}";
}

Interface::Interface() : core(nullptr) {
  mode = Mode::None;
  nes.initialize();
  snes.initialize();
  gb.initialize();
  gba.initialize();
}

//internal

bool Interface::applyPatch(CartridgePath &filepath, vector<uint8_t> &memory) {
  string patchname = filepath.filename("patch.bps", ".bps");
  if(file::exists(patchname) == false) return false;

  bpspatch bps;
  bps.modify(patchname);
  bps.source(memory.data(), memory.size());
  unsigned targetSize = bps.size();
  uint8_t *targetData = new uint8_t[targetSize];
  bps.target(targetData, targetSize);
  if(bps.apply() != bpspatch::result::success) {
    delete[] targetData;
    return false;
  }

  memory.resize(targetSize);
  memcpy(memory.data(), targetData, targetSize);
  delete[] targetData;
  return true;
}

void Interface::videoRefresh(const uint32_t *input, unsigned inputPitch, unsigned width, unsigned height) {
  uint32_t *output;
  unsigned outputPitch;

  if(application->depth == 30 && filter.opened()) {
    filter.render(input, inputPitch, width, height);
    input = filter.data;
    inputPitch = filter.pitch;
    width = filter.width;
    height = filter.height;
  }

  if(video.lock(output, outputPitch, width, height)) {
    inputPitch >>= 2, outputPitch >>= 2;

    for(unsigned y = 0; y < height; y++) {
      memcpy(output + y * outputPitch, input + y * inputPitch, width * sizeof(uint32_t));
    }

    if(config->video.maskOverscan && (mode() == Mode::NES || mode() == Mode::SNES)) {
      unsigned h = config->video.maskOverscanHorizontal;
      unsigned v = config->video.maskOverscanVertical;

      if(h) for(unsigned y = 0; y < height; y++) {
        memset(output + y * outputPitch, 0, h * sizeof(uint32_t));
        memset(output + y * outputPitch + (width - h), 0, h * sizeof(uint32_t));
      }

      if(v) for(unsigned y = 0; y < v; y++) {
        memset(output + y * outputPitch, 0, width * sizeof(uint32_t));
        memset(output + (height - 1 - y) * outputPitch, 0, width * sizeof(uint32_t));
      }
    }

    video.unlock();
    video.refresh();
  }

  static unsigned frameCounter = 0;
  static time_t previous, current;
  frameCounter++;

  time(&current);
  if(current != previous) {
    previous = current;
    utility->setStatusText({ "FPS: ", frameCounter });
    frameCounter = 0;
  }
}
