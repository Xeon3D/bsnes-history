struct FileBrowser : TopLevelWindow {
  VerticalLayout layout;
  HorizontalLayout pathLayout;
  LineEdit pathBox;
  Button browseButton;
  Button upButton;
  ListView contentsBox;

  enum class Mode : unsigned { Cartridge, Satellaview, SufamiTurbo, GameBoy } mode;
  void fileOpen(Mode mode, function<void (string)> callback);
  void create();

private:
  function<void (string)> callback;
  string folder;
  lstring filters;
  lstring contents;
  string folderPath;

  void folderBrowse();
  void folderUp();
  void fileActivate();
  void setFolder(const string &pathname);
  string cartridgeFolder(const string &pathname);
  void loadFile(const string &filename);
};

extern FileBrowser fileBrowser;
