struct Utility {
  void setInterface(Emulator::Interface *emulator);
  void loadMedia(Emulator::Interface *emulator, Emulator::Interface::Media &media);
  void loadMedia(Emulator::Interface *emulator, Emulator::Interface::Media &media, const string &pathname);
  void loadMedia(unsigned id, const string &path);
  void loadMedia(unsigned id, const string &name, const string &type, const string &path);
  void loadMemory();
  void saveMemory();

  void connect(unsigned port, unsigned device);
  void power();
  void reset();
  void load();
  void unload();

  void saveState(unsigned slot);
  void loadState(unsigned slot);

  void synchronizeDSP();
  void synchronizeRuby();
  void updateShader();
  void resize(bool resizeWindow = false);
  void toggleFullScreen();

  void updateStatus();
  void setStatusText(const string &text);
  void showMessage(const string &message);

  Utility();

  lstring path;
  lstring pathname;

private:
  string statusText;
  string statusMessage;
  time_t statusTime;
};

extern Utility *utility;
