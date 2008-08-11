class InputConfigWindow : public Window {
public:
  Label    capture_mode;
  Radiobox capture_always;
  Radiobox capture_focus;
  Radiobox capture_pause;
  Combobox config_type;
  Combobox config_subtype;
  Listbox  list;
  Button   setkey;
  Button   clrkey;

  void setup();
  void refresh_subtype();
  void refresh_list();

  enum InputType {
    TypeUnknown,

    Port1_Joypad,
    Port1_Multitap1,
    Port1_Multitap2,
    Port1_Multitap3,
    Port1_Multitap4,

    Port2_Joypad,
    Port2_Multitap1,
    Port2_Multitap2,
    Port2_Multitap3,
    Port2_Multitap4,

    UI_General,
  };

  InputType get_input_type(unsigned &length);

  uintptr_t capture_change(Event);
  uintptr_t type_change(Event);
  uintptr_t subtype_change(Event);
  uintptr_t list_change(Event);
  uintptr_t set_tick(Event);
  uintptr_t clr_tick(Event);

  string_setting& acquire(unsigned index, const char *&name);
  uint get_value(uint index);
  void set_value(uint index, uint16 value);
} window_input_config;

class InputCaptureWindow : public Window {
public:
  Label label;
  Canvas canvas;

  bool waiting;
  bool locked;
  uint index;

  void assign(uint16_t key);
  void show();
  void setup();

  uintptr_t close(Event);

  InputCaptureWindow();
} window_input_capture;
