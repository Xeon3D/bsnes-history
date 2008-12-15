class PathSettingsWindow : public Window {
public:
  Label   lrompath;
  Editbox rompath;
  Button  romselect;
  Button  romdefault;

  Label   lsavepath;
  Editbox savepath;
  Button  saveselect;
  Button  savedefault;

  Label   lpatchpath;
  Editbox patchpath;
  Button  patchselect;
  Button  patchdefault;

  Label   lcheatpath;
  Editbox cheatpath;
  Button  cheatselect;
  Button  cheatdefault;

  Label   lexportpath;
  Editbox exportpath;
  Button  exportselect;
  Button  exportdefault;

  uintptr_t selectpath_rom(event_t);
  uintptr_t defaultpath_rom(event_t);

  uintptr_t selectpath_save(event_t);
  uintptr_t defaultpath_save(event_t);

  uintptr_t selectpath_patch(event_t);
  uintptr_t defaultpath_patch(event_t);

  uintptr_t selectpath_cheat(event_t);
  uintptr_t defaultpath_cheat(event_t);

  uintptr_t selectpath_export(event_t);
  uintptr_t defaultpath_export(event_t);

  void update_paths();
  void setup();
} window_path_settings;
