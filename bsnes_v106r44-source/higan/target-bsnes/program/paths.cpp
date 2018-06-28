auto Program::path(string type, string location, string extension) -> string {
  auto pathname = Location::path(location);
  auto filename = Location::file(location);
  auto prefix = Location::prefix(filename);
  auto suffix = extension;

  if(type == "Games") {
    if(auto path = settings["Path/Games"].text()) {
      pathname = path;
    }
  }

  if(type == "Patches") {
    if(auto path = settings["Path/Patches"].text()) {
      pathname = path;
    }
  }

  if(type == "Saves") {
    if(auto path = settings["Path/Saves"].text()) {
      pathname = path;
    }
  }

  if(type == "Cheats") {
    if(auto path = settings["Path/Cheats"].text()) {
      pathname = path;
    }
  }

  if(type == "States") {
    if(auto path = settings["Path/States"].text()) {
      pathname = path;
    }
  }

  return {pathname, prefix, suffix};
}

auto Program::gamePath() -> string {
  if(!emulator->loaded()) return "";
  if(gameBoy.location) return gameBoy.location;
  return superFamicom.location;
}

auto Program::cheatPath() -> string {
  if(!emulator->loaded()) return "";
  auto location = gamePath();
  if(location.endsWith("/")) {
    return {location, "cheats.bml"};
  } else {
    return path("Cheats", location, ".cht");
  }
}

auto Program::statePath() -> string {
  if(!emulator->loaded()) return "";
  auto location = gamePath();
  if(location.endsWith("/")) {
    return {location, "bsnes/states/"};
  } else {
    return path("States", location, ".bsz");
  }
}
