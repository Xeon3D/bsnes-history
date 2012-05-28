#include "../ethos.hpp"
#include "hotkeys.cpp"
InputManager *inputManager = nullptr;

void AbstractInput::bind() {
  inputList.reset();
  lstring list = mapping.split(",");

  for(auto &mapping : list) {
    Input::Type type;
         if(mapping.endswith(".Up")) type = Input::Type::HatUp;
    else if(mapping.endswith(".Down")) type = Input::Type::HatDown;
    else if(mapping.endswith(".Left")) type = Input::Type::HatLeft;
    else if(mapping.endswith(".Right")) type = Input::Type::HatRight;
    else if(mapping.endswith(".Lo")) type = Input::Type::AxisLo;
    else if(mapping.endswith(".Hi")) type = Input::Type::AxisHi;
    else if(mapping.beginswith("JP") && mapping.position("Axis")) type = Input::Type::Axis;
    else if(mapping.beginswith("MS") && mapping.endswith("axis")) type = Input::Type::MouseAxis;
    else if(mapping.beginswith("MS")) type = Input::Type::MouseButton;
    else type = Input::Type::Button;

    string decode = mapping;
    if(auto position = decode.position(".")) decode[position()] = 0;
    unsigned scancode = Scancode::decode(decode);

    inputList.append({type, scancode});
  }
}

bool AbstractInput::append(const string &encode) {
  if(mapping.position(encode)) return true;  //mapping already bound
  if(mapping.empty() || mapping == "None") mapping = encode;  //remove "None"
  else mapping.append(",", encode);  //add to existing mapping list
  bind();
  return true;
}

AbstractInput::AbstractInput() : state(false) {
}

//

bool DigitalInput::bind(unsigned scancode, int16_t value) {
  using nall::Keyboard;
  using nall::Mouse;

  if(scancode == Scancode::None || scancode == keyboard(0)[Keyboard::Escape]) {
    inputList.reset();
    mapping = "None";
    return true;
  }

  string encode = Scancode::encode(scancode);

  if(Keyboard::isAnyKey(scancode) || Keyboard::isAnyModifier(scancode) || Joypad::isAnyButton(scancode)) {
    if(value == 0) return false;
    return append(encode);
  }

  if(Mouse::isAnyButton(scancode)) {
    if(value == 0) return false;
    return append(encode);
  }

  if(Joypad::isAnyHat(scancode)) {
    if(value & Joypad::HatUp   ) { encode.append(".Up"   ); return append(encode); }
    if(value & Joypad::HatDown ) { encode.append(".Down" ); return append(encode); }
    if(value & Joypad::HatLeft ) { encode.append(".Left" ); return append(encode); }
    if(value & Joypad::HatRight) { encode.append(".Right"); return append(encode); }
  }

  if(Joypad::isAnyAxis(scancode)) {
    if(value < -12288) { encode.append(".Lo"); return append(encode); }
    if(value > +24576) { encode.append(".Hi"); return append(encode); }
  }

  return false;
}

int16_t DigitalInput::poll() {
  bool result = logic;

  for(auto &item : inputList) {
    int16_t value = inputManager->poll(item.scancode);
    bool output = logic;
    switch(item.type) {
    case Input::Type::Button:      output = value;                    break;
    case Input::Type::MouseButton: output = value & input.acquired(); break;
    case Input::Type::HatUp:       output = value & Joypad::HatUp;    break;
    case Input::Type::HatDown:     output = value & Joypad::HatDown;  break;
    case Input::Type::HatLeft:     output = value & Joypad::HatLeft;  break;
    case Input::Type::HatRight:    output = value & Joypad::HatRight; break;
    case Input::Type::AxisLo:      output = value < -16384;           break;
    case Input::Type::AxisHi:      output = value > +16384;           break;
    }
    if(logic == 0) result |= output;
    if(logic == 1) result &= output;
  }

  return result;
}

//

bool RelativeInput::bind(unsigned scancode, int16_t value) {
  using nall::Keyboard;
  using nall::Mouse;

  if(scancode == Scancode::None || scancode == keyboard(0)[Keyboard::Escape]) {
    inputList.reset();
    mapping = "None";
    return true;
  }

  string encode = Scancode::encode(scancode);

  if(Mouse::isAnyAxis(scancode)) return append(encode);
  if(Joypad::isAnyAxis(scancode)) return append(encode);

  return false;
}

int16_t RelativeInput::poll() {
  int16_t result = 0;

  for(auto &item : inputList) {
    int16_t value = inputManager->poll(item.scancode);
    switch(item.type) {
    case Input::Type::MouseAxis: value = input.acquired() ? value : 0; break;
    case Input::Type::Axis:      value = value;                        break;
    }
    result += value;
  }

  return result;
}

//

bool AbsoluteInput::bind(unsigned scancode, int16_t value) {
  using nall::Keyboard;
  using nall::Mouse;

  if(scancode == Scancode::None || scancode == keyboard(0)[Keyboard::Escape]) {
    inputList.reset();
    mapping = "None";
    return true;
  }

  string encode = Scancode::encode(scancode);

  if(Mouse::isAnyAxis(scancode)) {
    //only one input can be assigned for absolute positioning
    inputList.reset();
    mapping = encode;
    return true;
  }

  return false;
}

int16_t AbsoluteInput::poll() {
  int16_t result = -32768;  //offscreen value

  using nall::Mouse;

  Position position = phoenix::Mouse::position();
  Geometry geometry = presentation->geometry();

  if(position.x < geometry.x
  || position.y < geometry.y
  || position.x >= geometry.x + geometry.width
  || position.y >= geometry.y + geometry.height) {
    //cursor is offscreen
    position.x = -32768;
    position.y = -32768;
  } else {
    //convert from screen to viewport coordinates
    double x = position.x - geometry.x;
    double y = position.y - geometry.y;

    //scale coordinate range to -0.5 to +0.5 (0.0 = center)
    x = x * 1.0 / geometry.width  - 0.5;
    y = y * 1.0 / geometry.height - 0.5;

    //scale coordinates to -32767 to +32767
    signed px = (signed)(x * 65535.0);
    signed py = (signed)(y * 65535.0);

    //clamp to valid range
    position.x = max(-32767, min(+32767, px));
    position.y = max(-32767, min(+32767, py));
  }

  for(auto &item : inputList) {
    if(item.scancode == mouse(0)[Mouse::Xaxis]) {
      result = position.x;
    }

    if(item.scancode == mouse(0)[Mouse::Yaxis]) {
      result = position.y;
    }
  }

  return result;
}

//

HotkeyInput::HotkeyInput() {
  logic = 1;  //AND
  inputManager->hotkeyMap.append(this);
}

//

void InputManager::bind() {
  for(auto &input : inputMap) input->bind();
  for(auto &input : hotkeyMap) input->bind();
}

void InputManager::poll() {
  using nall::Keyboard;

  activeScancode = !activeScancode;
  input.poll(scancode[activeScancode]);

  for(unsigned n = 0; n < Scancode::Limit; n++) {
    if(scancode[0][n] != scancode[1][n]) {
      if(browser->focused()) {
        browser->inputEvent(n, scancode[activeScancode][n]);
      }
      if(settings->focused()) {
        inputSettings->inputEvent(n, scancode[activeScancode][n]);
        hotkeySettings->inputEvent(n, scancode[activeScancode][n]);
      }
    }
  }

  if(presentation->focused()) pollHotkeys();
}

int16_t InputManager::poll(unsigned scancode) {
  return this->scancode[activeScancode][scancode];
}

void InputManager::saveConfiguration() {
  config.save(application->path("input.cfg"));
}

InputManager::InputManager() {
  inputManager = this;
  activeScancode = 0;
  bootstrap();
}

void InputManager::bootstrap() {
  unsigned guid = 0;
  for(auto &emulator : application->emulator) {
    for(auto &port : emulator->port) {
      for(auto &device : port.device) {
        for(auto &number : device.order) {
          auto &input = device.input[number];

          AbstractInput *abstract = nullptr;
          if(input.type == 0) abstract = new DigitalInput;
          if(input.type == 1) abstract = new RelativeInput;
          if(input.type == 2) abstract = new AbsoluteInput;
          if(input.type >= 3) continue;

          abstract->name = {emulator->information.name, "::", port.name, "::", device.name, "::", input.name};
          abstract->name.replace(" ", "");
          abstract->mapping = "None";
          abstract->logic = 0;  //OR

          input.guid = guid++;
          inputMap.append(abstract);
        }
      }
    }
  }

  for(auto &input : inputMap) {
    config.append(input->mapping, input->name);
  }

  appendHotkeys();

  config.load(application->path("input.cfg"));
  config.save(application->path("input.cfg"));

  bind();
}
