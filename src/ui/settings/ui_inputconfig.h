class InputConfigWindow : public Window {
public:
  Label    capture_mode;
  Radiobox capture_always;
  Radiobox capture_focus;
  Radiobox capture_pause;
  Listbox  list;
  Button   setkey;
  Button   clrkey;

  void setup();
  void refresh_list();

  uintptr_t capture_change(Event);
  uintptr_t list_change(Event);
  uintptr_t set_tick(Event);
  uintptr_t clr_tick(Event);

  static const int list_size;
  static const char list_index[][64];
  uint get_value(uint index);
  void set_value(uint index, uint16 value);
} window_input_config;

class InputCaptureWindow : public Window {
public:
  Label label;

  bool waiting;
  bool locked;
  uint index;

  void assign(uint16_t key);
  void show();
  void setup();

  uintptr_t close(Event);

  InputCaptureWindow();
} window_input_capture;
