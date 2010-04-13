#include "libsnes.hpp"
#include <snes/snes.hpp>

#include <nall/info/snes.hpp>
using namespace nall;

struct Interface : public SNES::Interface {
  snes_video_refresh_t pvideo_refresh;
  snes_audio_sample_t paudio_sample;
  snes_input_poll_t pinput_poll;
  snes_input_state_t pinput_state;

  void video_refresh(uint16_t *data, unsigned pitch, unsigned *line, unsigned width, unsigned height) {
    return pvideo_refresh(data, pitch, line, width, height);
  }

  void audio_sample(uint16_t left, uint16_t right) {
    return paudio_sample(left, right);
  }

  void input_poll() {
    return pinput_poll();
  }

  int16_t input_poll(bool port, SNES::Input::Device device, unsigned index, unsigned id) {
    return pinput_state(port, (unsigned)device, index, id);
  }

  Interface() : pvideo_refresh(0), paudio_sample(0), pinput_poll(0), pinput_state(0) {
  }
};

static Interface interface;

unsigned snes_library_revision() {
  return 1;
}

void snes_set_video_refresh(snes_video_refresh_t video_refresh) {
  interface.pvideo_refresh = video_refresh;
}

void snes_set_audio_sample(snes_audio_sample_t audio_sample) {
  interface.paudio_sample = audio_sample;
}

void snes_set_input_poll(snes_input_poll_t input_poll) {
  interface.pinput_poll = input_poll;
}

void snes_set_input_state(snes_input_state_t input_state) {
  interface.pinput_state = input_state;
}

void snes_set_controller_port_device(bool port, unsigned device) {
  SNES::input.port_set_device(port, (SNES::Input::Device)device);
}

void snes_init() {
  SNES::system.init(&interface);
  SNES::input.port_set_device(0, SNES::Input::Device::Joypad);
  SNES::input.port_set_device(1, SNES::Input::Device::Joypad);
}

void snes_term() {
  SNES::system.term();
}

void snes_unload() {
  SNES::cartridge.unload();
}

void snes_run() {
  SNES::system.run();
}

unsigned snes_serialize_size() {
  return SNES::system.serialize_size();
}

bool snes_serialize(uint8_t *data, unsigned size) {
  SNES::system.runtosave();
  serializer s = SNES::system.serialize();
  if(s.size() > size) return false;
  memcpy(data, s.data(), s.size());
  return true;
}

bool snes_unserialize(const uint8_t *data, unsigned size) {
  serializer s(data, size);
  return SNES::system.unserialize(s);
}

void snes_cheat_reset() {
  SNES::cheat.reset();
  SNES::cheat.synchronize();
}

void snes_cheat_set(unsigned index, bool enabled, const char *code) {
  SNES::cheat[index] = code;
  SNES::cheat[index].enabled = enabled;
  SNES::cheat.synchronize();
}

void snes_load_cartridge_normal(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size
) {
  SNES::cheat.reset();
  SNES::memory::cartrom.copy(rom_data, rom_size);
  string xmlrom = rom_xml ? string(rom_xml) : snes_information(rom_data, rom_size).xml_memory_map;
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, { xmlrom });
  SNES::system.power();
}

void snes_load_cartridge_bsx_slotted(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, const uint8_t *bsx_data, unsigned bsx_size
) {
  SNES::cheat.reset();
  SNES::memory::cartrom.copy(rom_data, rom_size);
  string xmlrom = rom_xml ? string(rom_xml) : snes_information(rom_data, rom_size).xml_memory_map;
  SNES::memory::bsxflash.copy(bsx_data, bsx_size);
  string xmlbsx = bsx_xml ? string(bsx_xml) : snes_information(bsx_data, bsx_size).xml_memory_map;
  SNES::cartridge.load(SNES::Cartridge::Mode::BsxSlotted, { xmlrom, xmlbsx });
  SNES::system.power();
}

void snes_load_cartridge_bsx(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, const uint8_t *bsx_data, unsigned bsx_size
) {
  SNES::cheat.reset();
  SNES::memory::cartrom.copy(rom_data, rom_size);
  string xmlrom = rom_xml ? string(rom_xml) : snes_information(rom_data, rom_size).xml_memory_map;
  SNES::memory::bsxflash.copy(bsx_data, bsx_size);
  string xmlbsx = bsx_xml ? string(bsx_xml) : snes_information(bsx_data, bsx_size).xml_memory_map;
  SNES::cartridge.load(SNES::Cartridge::Mode::Bsx, { xmlrom, xmlbsx });
  SNES::system.power();
}

void snes_load_cartridge_sufami_turbo(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *sta_xml, const uint8_t *sta_data, unsigned sta_size,
  const char *stb_xml, const uint8_t *stb_data, unsigned stb_size
) {
  SNES::cheat.reset();
  SNES::memory::cartrom.copy(rom_data, rom_size);
  string xmlrom = rom_xml ? string(rom_xml) : snes_information(rom_data, rom_size).xml_memory_map;
  SNES::memory::stArom.copy(sta_data, sta_size);
  string xmlsta = sta_xml ? string(sta_xml) : snes_information(sta_data, sta_size).xml_memory_map;
  SNES::memory::stBrom.copy(stb_data, stb_size);
  string xmlstb = stb_xml ? string(stb_xml) : snes_information(stb_data, stb_size).xml_memory_map;
  SNES::cartridge.load(SNES::Cartridge::Mode::SufamiTurbo, { xmlrom, xmlsta, xmlstb });
  SNES::system.power();
}

void snes_load_cartridge_super_game_boy(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *dmg_xml, const uint8_t *dmg_data, unsigned dmg_size
) {
  SNES::cheat.reset();
  SNES::memory::cartrom.copy(rom_data, rom_size);
  string xmlrom = rom_xml ? string(rom_xml) : snes_information(rom_data, rom_size).xml_memory_map;
  SNES::memory::gbrom.copy(dmg_data, dmg_size);
  string xmldmg = dmg_xml ? string(dmg_xml) : snes_information(dmg_data, dmg_size).xml_memory_map;
  SNES::cartridge.load(SNES::Cartridge::Mode::SuperGameBoy, { xmlrom, xmldmg });
  SNES::system.power();
}

#define SNES_MEMORY_CARTRIDGE_RAM       0
#define SNES_MEMORY_CARTRIDGE_RTC       1
#define SNES_MEMORY_BSX_RAM             2
#define SNES_MEMORY_BSX_PRAM            3
#define SNES_MEMORY_SUFAMI_TURBO_A_RAM  4
#define SNES_MEMORY_SUFAMI_TURBO_B_RAM  5
#define SNES_MEMORY_GAME_BOY_RAM        6
#define SNES_MEMORY_GAME_BOY_RTC        7

uint8_t* snes_get_memory_data(unsigned id) {
  switch(id) {
    case SNES_MEMORY_CARTRIDGE_RAM: return SNES::memory::cartram.data();
    case SNES_MEMORY_CARTRIDGE_RTC: return SNES::memory::cartrtc.data();
    case SNES_MEMORY_BSX_RAM: return SNES::memory::bsxram.data();
    case SNES_MEMORY_BSX_PRAM: return SNES::memory::bsxpram.data();
    case SNES_MEMORY_SUFAMI_TURBO_A_RAM: return SNES::memory::stAram.data();
    case SNES_MEMORY_SUFAMI_TURBO_B_RAM: return SNES::memory::stBram.data();
    case SNES_MEMORY_GAME_BOY_RAM: return SNES::memory::gbram.data();
    case SNES_MEMORY_GAME_BOY_RTC: return SNES::memory::gbrtc.data();
  }
  return 0;
}

unsigned snes_get_memory_size(unsigned id) {
  switch(id) {
    case SNES_MEMORY_CARTRIDGE_RAM: return SNES::memory::cartram.size();
    case SNES_MEMORY_CARTRIDGE_RTC: return SNES::memory::cartrtc.size();
    case SNES_MEMORY_BSX_RAM: return SNES::memory::bsxram.size();
    case SNES_MEMORY_BSX_PRAM: return SNES::memory::bsxpram.size();
    case SNES_MEMORY_SUFAMI_TURBO_A_RAM: return SNES::memory::stAram.size();
    case SNES_MEMORY_SUFAMI_TURBO_B_RAM: return SNES::memory::stBram.size();
    case SNES_MEMORY_GAME_BOY_RAM: return SNES::memory::gbram.size();
    case SNES_MEMORY_GAME_BOY_RTC: return SNES::memory::gbrtc.size();
  }
  return 0;
}
