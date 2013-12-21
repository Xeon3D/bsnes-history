struct AbstractInput {
  string name;
  string mapping;
  bool logic;  //0 = OR, 1 = AND
  bool state;

  struct Input {
    enum class Type : unsigned { Button, MouseButton, MouseAxis, HatUp, HatDown, HatLeft, HatRight, Axis, AxisLo, AxisHi, Rumble } type;
    unsigned scancode;
  };
  vector<Input> inputList;

  void bind();
  bool append(string mapping);
  virtual bool bind(unsigned scancode, int16_t value) = 0;
  virtual int16_t poll() = 0;
  virtual void rumble(bool enable) {}
  AbstractInput();
};

struct DigitalInput : AbstractInput {
  using AbstractInput::bind;
  bool bind(unsigned scancode, int16_t value);
  int16_t poll();
};

struct RelativeInput : AbstractInput {
  using AbstractInput::bind;
  bool bind(unsigned scancode, int16_t value);
  int16_t poll();
};

struct AbsoluteInput : AbstractInput {
  using AbstractInput::bind;
  bool bind(unsigned scancode, int16_t value);
  int16_t poll();
};

struct RumbleInput : AbstractInput {
  using AbstractInput::bind;
  bool bind(unsigned scancode, int16_t value);
  int16_t poll();
  void rumble(bool enable);
};

struct HotkeyInput : DigitalInput {
  function<void ()> press;
  function<void ()> release;
  HotkeyInput();
};

struct InputManager {
  vector<AbstractInput*> inputMap;
  vector<HotkeyInput*> hotkeyMap;
  int16_t* scancode[2];
  bool activeScancode;

  void bind();
  void poll();
  int16_t poll(unsigned scancode);
  void saveConfiguration();
  void bootstrap();
  InputManager();
  ~InputManager();

  //hotkeys.cpp
  void appendHotkeys();
  void pollHotkeys();

private:
  Configuration::Document config;
};

extern InputManager* inputManager;
