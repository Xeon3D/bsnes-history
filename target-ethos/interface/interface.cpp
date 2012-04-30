#include "../ethos.hpp"
Interface *interface = nullptr;

uint32_t Interface::videoColor(unsigned source, uint16_t red, uint16_t green, uint16_t blue) {
  red >>= 8, green >>= 8, blue >>= 8;
  return red << 16 | green << 8 | blue << 0;
}

void Interface::videoRefresh(const uint32_t *data, unsigned pitch, unsigned width, unsigned height) {
  uint32_t *output;
  unsigned outputPitch;

  if(video.lock(output, outputPitch, width, height)) {
    pitch >>= 2, outputPitch >>= 2;

    for(unsigned y = 0; y < height; y++) {
      memcpy(output + y * outputPitch, data + y * pitch, 4 * width);
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
    utility->setStatusText({"FPS: ", frameCounter});
    frameCounter = 0;
  }
}

void Interface::audioSample(int16_t lsample, int16_t rsample) {
  audio.sample(lsample, rsample);
}

int16_t Interface::inputPoll(unsigned port, unsigned device, unsigned input) {
  unsigned guid = system().port[port].device[device].input[input].guid;
  return inputManager->poll(guid);
}
