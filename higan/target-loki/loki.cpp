#include "loki.hpp"
unique_pointer<Video> video;
unique_pointer<Audio> audio;
unique_pointer<Input> input;
Emulator::Interface* emulator = nullptr;

auto locate(string name) -> string {
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  location = {Path::config(), "loki/", name};
  if(inode::exists(location)) return location;

  directory::create({Path::local(), "loki/"});
  return {Path::local(), "loki/", name};
}

#include <nall/main.hpp>
auto nall::main(lstring args) -> void {
  Application::setName("loki");
  new Program(args);
  Application::run();
}
