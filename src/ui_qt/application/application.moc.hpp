struct FileReader : public library {
  string filterList;

  function<const char* ()> supported;
  function<bool (const char*, uint8_t**, unsigned*)> load;

  const char* direct_supported();
  bool direct_load(const char*, uint8_t**, unsigned*);

  FileReader();
} libsnesreader;

class Application : public QObject {
  Q_OBJECT

public:
  class App : public QApplication {
  public:
    #if defined(PLATFORM_WIN)
    bool winEventFilter(MSG *msg, long *result);
    #endif

    App(int &argc, char **argv) : QApplication(argc, argv) {}
  } *app;

  QTimer *timer;

  bool terminate;  //set to true to terminate main() loop and exit emulator
  bool power;
  bool pause;
  bool autopause;
  bool debug;      //debugger sets this to true when entered to suspend emulation
  bool debugrun;   //debugger sets this to true to run emulation to a debug event

  clock_t clockTime;
  clock_t autosaveTime;
  clock_t screensaverTime;

  string configFilename;
  string styleSheetFilename;

  int main(int &argc, char **argv);
  void locateFile(string &filename, bool createDataDirectory = false);
  void initPaths(const char *basename);
  void init();

  Application();
  ~Application();

public slots:
  void run();
} application;
