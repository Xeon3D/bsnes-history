/*****
 * SNES Interface class
 *
 * Interfaces SNES core with platform-specific functionality
 * (video, audio, input, ...)
 *****/

SNESInterface snesinterface;

//video

static unsigned frameskip_counter = 0;
static unsigned frameskip_offset  = 0;

void SNESInterface::video_refresh(uint16_t *data, unsigned pitch, unsigned *line, unsigned width, unsigned height) {
  if(ppu.renderer_enabled() == true) {
    uint32_t *output;
    unsigned opitch;
    if(video.lock(output, opitch) == true) {
      unsigned owidth, oheight;
      libfilter::filter.render(output, opitch, owidth, oheight, data, pitch, line, width, height);
      video.unlock();
      video.refresh(owidth, oheight);
    }
  }

  if(config::video.frameskip == 0) {
    ppu.enable_renderer(true);
  } else {
    frameskip_counter++;
    frameskip_counter %= config::video.frameskip + 1;
    if(frameskip_counter == 0) {
      //randomize which frame of set will be rendered,
      //helps with two-frame animations (such as blinking)
      frameskip_offset = rand() % (config::video.frameskip + 1);
    }
    ppu.enable_renderer(frameskip_counter == frameskip_offset);
  }
}

//audio

void SNESInterface::audio_sample(uint16 l_sample, uint16 r_sample) {
  if(config::audio.mute == true) {
    l_sample = 0;
    r_sample = 0;
  }
  audio.sample(l_sample, r_sample);
}

//input

void SNESInterface::input_poll() {
  if(input_ready && input_ready() == false) {
    input_manager.clear();
  } else {
    input_manager.poll();
  }
}

int16_t SNESInterface::input_poll(unsigned deviceid, unsigned id) {
  return input_manager.get_status(deviceid, id);
}

//core

void SNESInterface::init() {
  input_manager.bind();
  libfilter::colortable.set_format(libfilter::Colortable::RGB888);
  libfilter::colortable.set_contrast(config::system.contrast);
  libfilter::colortable.set_brightness(config::system.brightness);
  libfilter::colortable.set_gamma(config::system.gamma);
  libfilter::colortable.enable_gamma_ramp(config::system.gamma_ramp);
  libfilter::colortable.enable_sepia(config::system.sepia);
  libfilter::colortable.enable_grayscale(config::system.grayscale);
  libfilter::colortable.enable_invert(config::system.invert);
  libfilter::colortable.update();
}

void SNESInterface::term() {
}
