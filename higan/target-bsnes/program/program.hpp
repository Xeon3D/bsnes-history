struct Program : Emulator::Platform {
  Application::Namespace tr{"Program"};

  //program.cpp
  Program(string_vector arguments);
  auto main() -> void;
  auto quit() -> void;

  //platform.cpp
  auto open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file override;
  auto load(uint id, string name, string type, string_vector options = {}) -> Emulator::Platform::Load override;
  auto videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioSample(const double* samples, uint channels) -> void override;
  auto inputPoll(uint port, uint device, uint input) -> int16 override;
  auto inputRumble(uint port, uint device, uint input, bool enable) -> void override;

  //game.cpp
  auto load() -> void;
  auto loadFile(string location) -> vector<uint8_t>;
  auto loadSuperFamicom(string location) -> bool;
  auto loadGameBoy(string location) -> bool;
  auto loadBSMemory(string location) -> bool;
  auto loadSufamiTurboA(string location) -> bool;
  auto loadSufamiTurboB(string location) -> bool;
  auto save() -> void;
  auto reset() -> void;
  auto unload() -> void;
  auto verified() const -> bool;

  //game-pak.cpp
  auto openPakSuperFamicom(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openPakGameBoy(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openPakBSMemory(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openPakSufamiTurboA(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openPakSufamiTurboB(string name, vfs::file::mode mode) -> vfs::shared::file;

  //game-rom.cpp
  auto openRomSuperFamicom(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openRomGameBoy(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openRomBSMemory(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openRomSufamiTurboA(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openRomSufamiTurboB(string name, vfs::file::mode mode) -> vfs::shared::file;

  //paths.cpp
  auto path(string type, string location, string extension = "") -> string;
  auto gamePath() -> string;
  auto cheatPath() -> string;
  auto statePath() -> string;
  auto screenshotPath() -> string;

  //states.cpp
  auto managedStates() -> string_vector;
  auto loadState(string filename) -> bool;
  auto saveState(string filename) -> bool;
  auto saveUndoState() -> bool;
  auto saveRedoState() -> bool;
  auto removeState(string filename) -> bool;
  auto renameState(string from, string to) -> bool;

  //video.cpp
  auto updateVideoDriver() -> void;
  auto updateVideoBlocking() -> void;
  auto updateVideoShader() -> void;
  auto updateVideoPalette() -> void;

  //audio.cpp
  auto updateAudioDriver() -> void;
  auto updateAudioExclusive() -> void;
  auto updateAudioDevice() -> void;
  auto updateAudioBlocking() -> void;
  auto updateAudioFrequency() -> void;
  auto updateAudioLatency() -> void;
  auto updateAudioEffects() -> void;

  //input.cpp
  auto updateInputDriver() -> void;

  //utility.cpp
  auto showMessage(string text) -> void;
  auto showFrameRate(string text) -> void;
  auto updateStatus() -> void;
  auto captureScreenshot() -> bool;
  auto paused() -> bool;
  auto focused() -> bool;

  //patch.cpp
  auto appliedPatch() const -> bool;
  auto applyPatchIPS(vector<uint8_t>& data, string location) -> bool;
  auto applyPatchBPS(vector<uint8_t>& data, string location) -> bool;

  //hacks.cpp
  auto hackCompatibility() -> void;
  auto hackPatchMemory(vector<uint8_t>& data) -> void;
  auto hackOverclockSuperFX() -> void;

public:
  struct Game {
    explicit operator bool() const { return (bool)location; }

    string location;
    string manifest;
    Markup::Node document;
    boolean patched;
    boolean verified;
  };

  struct SuperFamicom : Game {
    string label;
    vector<uint8_t> program;
    vector<uint8_t> data;
    vector<uint8_t> expansion;
    vector<uint8_t> firmware;
  } superFamicom;

  struct GameBoy : Game {
    vector<uint8_t> program;
  } gameBoy;

  struct BSMemory : Game {
    vector<uint8_t> program;
  } bsMemory;

  struct SufamiTurbo : Game {
    vector<uint8_t> program;
  } sufamiTurboA, sufamiTurboB;

  string_vector gameQueue;

  struct Screenshot {
    const uint32* data = nullptr;
    uint pitch = 0;
    uint width = 0;
    uint height = 0;
  } screenshot;

  bool frameAdvance = false;

  uint64 autoSaveTime;

  uint64 statusTime;
  string statusMessage;
  string statusFrameRate;
};

extern unique_pointer<Program> program;
