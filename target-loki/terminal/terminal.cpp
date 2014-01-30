#include "../loki.hpp"
Terminal* terminal = nullptr;

Terminal::Terminal() {
  terminal = this;

  setTitle({"loki v", Emulator::Version});
  setWindowGeometry({0, 480 + frameMargin().height, 800, 480});

  console.setFont(Font::monospace(8));
  console.setPrompt("$ ");
  echo("loki v", Emulator::Version, "\n\n");

  layout.append(console, {~0, ~0});
  append(layout);

  onClose = &Application::quit;
  console.onActivate = {&Terminal::command, this};
}

void Terminal::command(string t) {
  lstring args = t.qsplit(" ");
  string s = args.takeFirst();
  unsigned argc = args.size();

  if(s.empty()) return;

  if(s.beginsWith("settings.")) return settings->command(s, args);

  if(s == "break") {
    debugger->mode = Debugger::Mode::Break;
    return;
  }

  if(s == "clear") {
    reset();
    return;
  }

  if(s == "hex" && argc >= 1 && argc <= 2) {
    debugger->echoHex(hex(args[0]), argc == 2 ? integer(args[1]) : 256);
    return;
  }

  if(s == "quit") {
    Application::quit();
    return;
  }

  if(s == "run") {
    debugger->mode = Debugger::Mode::Run;
    return;
  }

  if(s == "step" && argc <= 1) {
    if(debugger->mode != Debugger::Mode::Break) {
      echo("Error: must break before stepping\n");
      return;
    }
    debugger->mode = Debugger::Mode::Step;
    debugger->stepDuration = (argc == 1 ? decimal(args[0]) : 1);
    return;
  }

  echo("Error: unrecognized command: ", s, "\n");
}

void Terminal::reset() {
  console.reset();
}

void Terminal::print(const string& text) {
  console.print(text);
}
