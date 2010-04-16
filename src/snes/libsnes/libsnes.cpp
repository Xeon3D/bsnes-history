#include "libsnes.hpp"
#include <snes/snes.hpp>

#include <nall/snes/info.hpp>
using namespace nall;

struct Interface : public SNES::Interface {
  snes_video_refresh_t pvideo_refresh;
  snes_audio_sample_t paudio_sample;
  snes_input_poll_t pinput_poll;
  snes_input_state_t pinput_state;

  void video_refresh(const uint16_t *data, unsigned width, unsigned height) {
    if(pvideo_refresh) return pvideo_refresh(data, width, height);
  }

  void audio_sample(uint16_t left, uint16_t right) {
    if(paudio_sample) return paudio_sample(left, right);
  }

  void input_poll() {
    if(pinput_poll) return pinput_poll();
  }

  int16_t input_poll(bool port, SNES::Input::Device device, unsigned index, unsigned id) {
    if(pinput_state) return pinput_state(port, (unsigned)device, index, id);
    return 0;
  }

  Interface() : pvideo_refresh(0), paudio_sample(0), pinput_poll(0), pinput_state(0) {
  }
};

static Interface interface;

unsigned snes_library_revision(void) {
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

void snes_init(void) {
  SNES::system.init(&interface);
  SNES::input.port_set_device(0, SNES::Input::Device::Joypad);
  SNES::input.port_set_device(1, SNES::Input::Device::Joypad);
}

void snes_term(void) {
  SNES::system.term();
}

void snes_power(void) {
  SNES::system.power();
}

void snes_reset(void) {
  SNES::system.reset();
}

void snes_run(void) {
  SNES::system.run();
}

void snes_unload(void) {
  SNES::cartridge.unload();
}

unsigned snes_serialize_size(void) {
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

void snes_cheat_reset(void) {
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
  snes_cheat_reset();
  SNES::memory::cartrom.copy(rom_data, rom_size);
  string xmlrom = rom_xml ? string(rom_xml) : snes_information(rom_data, rom_size).xml_memory_map;
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, { xmlrom });
  SNES::system.power();
}

void snes_load_cartridge_bsx_slotted(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, const uint8_t *bsx_data, unsigned bsx_size
) {
  snes_cheat_reset();
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
  snes_cheat_reset();
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
  snes_cheat_reset();
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
  snes_cheat_reset();
  SNES::memory::cartrom.copy(rom_data, rom_size);
  string xmlrom = rom_xml ? string(rom_xml) : snes_information(rom_data, rom_size).xml_memory_map;
  SNES::memory::gbrom.copy(dmg_data, dmg_size);
  string xmldmg = dmg_xml ? string(dmg_xml) : snes_information(dmg_data, dmg_size).xml_memory_map;
  SNES::cartridge.load(SNES::Cartridge::Mode::SuperGameBoy, { xmlrom, xmldmg });
  SNES::system.power();
}

bool snes_get_region(void) {
  return SNES::system.region() == SNES::System::Region::NTSC ? 0 : 1;
}

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

static uint32_t colortable[32768];

void snes_blit_set_colortable(const uint32_t *new_colortable) {
  memcpy(colortable, new_colortable, 32768 * sizeof(uint32_t));
}

void snes_blit(
  uint32_t *output, unsigned output_pitch, unsigned output_width, unsigned output_height,
  const uint16_t *input, unsigned input_pitch, unsigned input_width, unsigned input_height
) {
  output_pitch >>= 2;
  input_pitch >>= 1;

  double step_x = (double)input_width / (double)output_width;
  double step_y = (double)input_height / (double)output_height;

  double pos_y = 0;
  for(unsigned y = 0; y < output_height; y++) {
    uint32_t *o = output + y * output_pitch;
    const uint16_t *i = input + (unsigned)(pos_y + 0.5) * input_pitch;
    double pos_x = 0;
    for(unsigned x = 0; x < output_width; x++) {
      unsigned pixel = i[(unsigned)(pos_x + 0.5)];
      *o++ = colortable[pixel];
      pos_x += step_x;
    }
    pos_y += step_y;
  }
}
