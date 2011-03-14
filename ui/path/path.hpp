struct Path : public configuration {
  string user;
  string base;

  string sfc;
  string bs;
  string st;
  string gb;

  string satellaviewBios;
  string sufamiTurboBios;
  string superGameBoyBios;

  string necdsp;
  string msu1;
  string serial;

  string srm;
  string rtc;
  string bss;
  string bsp;
  string sts;
  string sav;

  string bsa;
  string bst;
  string cht;
  string log;

  string home(const string &filename);
  string load(const string &path);
  void save(const string &path, const string &value);
  string load(SNES::Cartridge::Path path, const string &hint);

  void load();
  void save();

  Path();
};

extern Path path;
