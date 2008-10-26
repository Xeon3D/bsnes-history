#define INTERFACE_MAIN

#include "../base.h"

#include "main.h"
#include "config.cpp"

void init_snes();
void term_snes();

/*****
 * hardware abstraction layer
 *****/

#include <ruby/ruby.hpp>
using namespace ruby;

#include <libfilter/libfilter.hpp>

/*****
 * platform abstraction layer
 *****/

#include <hiro/hiro.hpp>
using namespace libhiro;

/*****
 * core
 *****/

#include "ui.h"
#include "status.h"
#include "event.h"

#include "inputmanager.cpp"
#include "interface.cpp"

#include "ui.cpp"
#include "status.cpp"
#include "event.cpp"

void alert(const char *s, ...) {
  char str[4096];
  va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);

  status.enqueue(str);
}

void dprintf(const char *s, ...) {
  char str[4096];
  va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);

  fprintf(stdout, "%s\r\n", str);
}

void get_paths(const char *image) {
  char temp[PATH_MAX] = "";
  realpath(image, temp);

  if(strlen(temp) != 0) {
    for(int i = strlen(temp) - 1; i >= 0; i--) {
      if(temp[i] == '\\') temp[i] = '/';
    }
    //remove program name
    for(int i = strlen(temp) - 1; i >= 0; i--) {
      if(temp[i] == '/') {
        temp[i] = 0;
        break;
      }
    }
  }

  if(strend(temp, "/") == false) strcat(temp, "/");
  config::path.base = temp;

  userpath(temp);
  if(strend(temp, "/") == false) strcat(temp, "/");
  config::path.user = temp;
}

void set_config_filenames() {
  char filename[PATH_MAX];

  //locate bsnes.cfg
  strcpy(filename, config::path.base);
  strcat(filename, "bsnes.cfg");
  if(!file::exists(filename)) {
    strcpy(filename, config::path.user);
    strcat(filename, ".bsnes");
    mkdir(filename);
    strcat(filename, "/bsnes.cfg");
  }
  strcpy(config::bsnes_cfg, filename);

  //locate locale.cfg
  strcpy(filename, config::path.base);
  strcat(filename, "locale.cfg");
  if(!file::exists(filename)) {
    strcpy(filename, config::path.user);
    strcat(filename, ".bsnes");
    mkdir(filename);
    strcat(filename, "/locale.cfg");
  }
  strcpy(config::locale_cfg, filename);
}

void run() {
  while(hiro().pending()) hiro().run();
  status.update();
  input_manager.refresh();

  if(config::input.capture_mode == 2) {
    bool inactive = window_main.focused() == false;
    if(app.autopause == false && inactive == true) {
      app.autopause = true;
      audio.clear();
    } else if(app.autopause == true && inactive == false) {
      app.autopause = false;
    }
  } else {
    app.autopause = false;
  }

  if(cartridge.loaded() == false || app.pause == true || app.autopause == true) {
    usleep(20 * 1000);  //prevent bsnes from consuming 100% CPU resources when idle
  } else {
    snes.runtoframe();
  }
}

int hiromain(int argc, const char *const argv[]) {
  get_paths(argv[0]);
  set_config_filenames();

  config::config().load(config::bsnes_cfg);
  if(file::exists(config::bsnes_cfg) == false) {
    //in case program crashes on first run, save config file
    //settings, so that they can be modified by hand ...
    config::config().save(config::bsnes_cfg);
  }
  translate.import(config::locale_cfg);

  ui_init();
  if(app.term == false) {
    snes.init();
    if(argc >= 2) event::load_image(argv[1]);
    while(app.term == false) run();
    event::unload_cart();
  }

  config::config().save(config::bsnes_cfg);
  snes.term();
  ui_term();
  return 0;
}
