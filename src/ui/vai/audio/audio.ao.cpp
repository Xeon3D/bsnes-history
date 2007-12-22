#include "audio.ao.h"

#include <ao/ao.h>

class pAudioAO {
public:
  AudioAO &self;
  int driver_id;
  ao_sample_format driver_format;
  ao_device *audio_device;

  struct {
    uint frequency;
  } settings;

  bool cap(Audio::Setting setting) {
    if(setting == Audio::Frequency) return true;
    return false;
  }

  uintptr_t get(Audio::Setting setting) {
    if(setting == Audio::Frequency) return settings.frequency;
    return false;
  }

  bool set(Audio::Setting setting, uintptr_t param) {
    if(setting == Audio::Frequency) {
      settings.frequency = param;
      if(audio_device) {
        term();
        init();
      }
      return true;
    }
    return false;
  }

  void sample(uint16 l_sample, uint16 r_sample) {
  uint32 samp = (l_sample << 0) + (r_sample << 16);
    ao_play(audio_device, (char*)&samp, 4); //This may need to be byte swapped for Big Endian
  }

  void init() {
    driver_id = ao_default_driver_id(); //ao_driver_id((const char*)driver)
    if(driver_id < 0) driver_id = ao_default_driver_id(); //fallback on default if driver doesn't exist
    driver_format.bits = 16;
    driver_format.channels = 2;
    driver_format.rate = settings.frequency;
    driver_format.byte_format = AO_FMT_LITTLE;

    audio_device = ao_open_live(driver_id, &driver_format, 0);
    if(audio_device) {
      ao_info *di = ao_driver_info(driver_id);
    } else {
      printf("libao: failed to open audio device.\n");
    }
  }

  void term() {
    if(audio_device) {
      ao_close(audio_device);
      audio_device = 0;
    }
  }

  pAudioAO(AudioAO &self_) : self(self_) {
    settings.frequency = 22050;
    audio_device = 0;
    ao_initialize();
  }

  ~pAudioAO() {
    term();
  //ao_shutdown(); //FIXME: this is causing a segfault for some reason when called ...
  }
};

bool AudioAO::cap(Setting setting) { return p.cap(setting); }
uintptr_t AudioAO::get(Setting setting) { return p.get(setting); }
bool AudioAO::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
void AudioAO::sample(uint16 l_sample, uint16 r_sample) { p.sample(l_sample, r_sample); }
void AudioAO::init() { p.init(); }
void AudioAO::term() { p.term(); }
AudioAO::AudioAO() : p(*new pAudioAO(*this)) {}
AudioAO::~AudioAO() { delete &p; }
