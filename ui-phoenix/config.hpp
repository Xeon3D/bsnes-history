struct Configuration : public configuration {
  struct Path {
    string base;
    string user;
    string current;
  } path;

  struct Video {
    string driver;
    bool synchronize;
    bool smooth;
    unsigned scale;
    bool aspectRatioCorrection;
    unsigned contrast;
    unsigned brightness;
    unsigned gamma;
    bool useGammaRamp;
  } video;

  struct Audio {
    string driver;
    bool synchronize;
    bool mute;
    unsigned volume;
    unsigned latency;
    unsigned inputFrequency;
    unsigned outputFrequency;
  } audio;

  struct Input {
    string driver;
  } input;

  struct Settings {
    unsigned focusPolicy;
    bool useNativeDialogs;
  } settings;

  void load();
  void save();
  void create();
};

extern Configuration config;
