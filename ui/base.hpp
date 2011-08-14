#include <snes/snes.hpp>

#include <nall/base64.hpp>
#include <nall/bmp.hpp>
#include <nall/compositor.hpp>
#include <nall/config.hpp>
#include <nall/directory.hpp>
#include <nall/dsp.hpp>
#include <nall/filemap.hpp>
#include <nall/input.hpp>
#include <nall/resource.hpp>
#include <nall/ups.hpp>
#include <nall/snes/cartridge.hpp>
#include <nall/gameboy/cartridge.hpp>
using namespace nall;

#include <ruby/ruby.hpp>
using namespace ruby;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

struct TopLevelWindow : Window {
  string name;
  string position;
};

#include "interface.hpp"
#include "config.hpp"
#include "general/general.hpp"
#include "settings/settings.hpp"
#include "tools/tools.hpp"
#include "input/input.hpp"
#include "utility/utility.hpp"
#include "path/path.hpp"
#include "cartridge/cartridge.hpp"

#if defined(DEBUGGER)
  #include "debugger/debugger.hpp"
#endif

struct Application {
  Font proportionalFont;
  Font proportionalFontBold;
  Font monospaceFont;
  Font titleFont;
  bool compositorActive;

  bool pause;
  bool quit;
  void main(int argc, char **argv);

  void addWindow(TopLevelWindow *window, const string &name, const string &position);
  Application();

private:
  array<TopLevelWindow*> windows;
  configuration geometryConfig;
  void loadGeometry();
  void saveGeometry();
};

extern nall::dsp dspaudio;
extern Application application;
