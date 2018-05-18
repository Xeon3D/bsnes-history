#include "bsnes.hpp"
unique_pointer<Video> video;
unique_pointer<Audio> audio;
unique_pointer<Input> input;
Emulator::Interface* interface = nullptr;

auto locate(string name) -> string {
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  location = {Path::config(), "bsnes/", name};
  if(inode::exists(location)) return location;

  directory::create({Path::local(), "bsnes/"});
  return {Path::local(), "bsnes/", name};
}

#include <nall/main.hpp>
auto nall::main(string_vector args) -> void {
  Application::setName("bsnes");
  new Program(args);
  Application::run();
}
