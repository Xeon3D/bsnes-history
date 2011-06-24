#ifdef CONTROLLER_CPP

uint2 Justifier::data() {
  if(counter >= 32) return 1;

  if(counter == 0) {
    trigger1 = system.interface->input_poll(port, Input::Device::Justifier, 0, (unsigned)Input::JustifierID::Trigger);
    start1   = system.interface->input_poll(port, Input::Device::Justifier, 0, (unsigned)Input::JustifierID::Start);
    if(chained) {
      trigger2 = system.interface->input_poll(port, Input::Device::Justifiers, 1, (unsigned)Input::JustifierID::Trigger);
      start2   = system.interface->input_poll(port, Input::Device::Justifiers, 1, (unsigned)Input::JustifierID::Start);
    }
  }

  switch(counter++) {
  case  0: return 0;
  case  1: return 0;
  case  2: return 0;
  case  3: return 0;
  case  4: return 0;
  case  5: return 0;
  case  6: return 0;
  case  7: return 0;
  case  8: return 0;
  case  9: return 0;
  case 10: return 0;
  case 11: return 0;

  case 12: return 1;  //signature
  case 13: return 1;  // ||
  case 14: return 1;  // ||
  case 15: return 0;  // ||

  case 16: return 0;
  case 17: return 1;
  case 18: return 0;
  case 19: return 1;
  case 20: return 0;
  case 21: return 1;
  case 22: return 0;
  case 23: return 1;

  case 24: return trigger1;
  case 25: return trigger2;
  case 26: return start1;
  case 27: return start2;
  case 28: return active;

  case 29: return 0;
  case 30: return 0;
  case 31: return 0;
  }
}

void Justifier::latch(bool data) {
  if(latched == data) return;
  latched = data;
  counter = 0;
  if(chained) active = !active;  //toggle between both controllers when chained

  int nx1 = system.interface->input_poll(port, Input::Device::Justifier, 0, (unsigned)Input::JustifierID::X);
  int ny1 = system.interface->input_poll(port, Input::Device::Justifier, 0, (unsigned)Input::JustifierID::Y);
  nx1 += x1;
  ny1 += y1;
  x1 = max(-16, min(256 + 16, nx1));
  y1 = max(-16, min(240 + 16, ny1));

  if(chained == false) return;
  int nx2 = system.interface->input_poll(port, Input::Device::Justifiers, 1, (unsigned)Input::JustifierID::X);
  int ny2 = system.interface->input_poll(port, Input::Device::Justifiers, 1, (unsigned)Input::JustifierID::Y);
  nx2 += x2;
  ny2 += y2;
  x2 = max(-16, min(256 + 16, nx2));
  y2 = max(-16, min(240 + 16, ny2));
}

Justifier::Justifier(bool port, bool chained) : Controller(port), chained(chained) {
  latched = 0;
  counter = 0;
  active = 0;

  if(chained == false) {
    x1 = 256 / 2;
    y1 = 240 / 2;
    x2 = -1;
    y2 = -1;
  } else {
    x1 = 256 / 2 - 16;
    y1 = 240 / 2;
    x2 = 256 / 2 + 16;
    y2 = 240 / 2;
  }

  trigger1 = false;
  trigger2 = false;

  start1 = false;
  start2 = false;
}

#endif
