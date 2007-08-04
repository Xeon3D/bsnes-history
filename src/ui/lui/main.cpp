void run();

#if defined(PLATFORM_WIN)
  #include "../../lib/libui_win.h"
#elif defined(PLATFORM_X)
  #include "../../lib/libui_gtk.h"
#else
  #error "unsupported platform"
#endif

namespace ui = libui;

bool _term_ = false;

#include "ui.h"
#include "event.h"

#include "ui.cpp"
#include "event.cpp"

bool allow_input() {
#if defined(PLATFORM_X)
//TODO: window_main.focused() does not work at all on X
  return true;
#endif
//only allow input capture when main window is active
  return window_main.focused() == true;
}

void alert(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(stdout, "%s\r\n", str);
}

void dprintf(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(stdout, "%s\r\n", str);
}

void dprintf(uint source, char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(stdout, "[%d]: %s\r\n", source, str);
}

void set_config_filename() {
  userpath(config::filename);
  strcat(config::filename, "/.bsnes");
  mkdir(config::filename); //always make directory in case it does not exist, fail silently if it does
  strcat(config::filename, "/bsnes.cfg");
}

void run() {
  while(ui::events_pending() == true) { ui::run(); }
  if(cartridge.loaded() == true) {
    snes.runtoframe();
    event::update_frame_counter();
  }
#if defined(_MSC_VER)
//prevent bsnes from consuming 100% CPU resources when idle
  else { Sleep(1); }
#endif
}

#if defined(PLATFORM_WIN)
int __stdcall WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
int argc = __argc;
char **argv = __argv;
#else
int main(int argc, char *argv[]) {
#endif

//int main(int argc, char *argv[]) {
  set_config_filename();

  ui::init();
  config_file.load(config::filename);
  config_file.save(config::filename); //in case program crashes on first run, config file settings can be modified
  init_snes();
  ui_init();

  if(argc >= 2) {
    cartridge.load_begin(Cartridge::CART_NORMAL);
    cartridge.load(argv[1]);
    cartridge.load_end();
    snes.power();
  }

  while(_term_ == false) {
    run();
  }

  event::unload_rom();

  config_file.save(config::filename);
  term_snes();
  ui_term();
  ui::term();
  return 0;
}
