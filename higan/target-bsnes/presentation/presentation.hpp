struct AboutWindow : Window {
  AboutWindow();

  VerticalLayout layout{this};
    Canvas canvas{&layout, Size{400, 85}, 0};
    HorizontalLayout informationLayout{&layout, Size{~0, ~0}};
      Label informationLeft{&informationLayout, Size{~0, 0}, 3};
      Label informationRight{&informationLayout, Size{~0, 0}};
};

struct Presentation : Window {
  enum : uint { RecentGames = 9, QuickStates = 9 };
  enum : uint { StatusHeight = 24 };

  Presentation();
  auto updateStatusIcon() -> void;
  auto drawIcon(uint32_t* output, uint length, uint width, uint height) -> void;
  auto clearViewport() -> void;
  auto resizeViewport() -> void;
  auto resizeWindow() -> void;
  auto updateStatus() -> void;
  auto toggleFullscreenMode() -> void;
  auto clearRecentGames() -> void;
  auto updateRecentGames() -> void;
  auto addRecentGame(string location) -> void;
  auto updateShaders() -> void;

  MenuBar menuBar{this};
    Menu systemMenu{&menuBar};
      MenuItem loadGame{&systemMenu};
      Menu loadRecentGame{&systemMenu};
      MenuItem resetSystem{&systemMenu};
      MenuItem unloadGame{&systemMenu};
      MenuSeparator portSeparator{&systemMenu};
      Menu controllerPort1{&systemMenu};
      Menu controllerPort2{&systemMenu};
      Menu expansionPort{&systemMenu};
      MenuSeparator quitSeparator{&systemMenu};
      MenuItem quit{&systemMenu};
    Menu settingsMenu{&menuBar};
      Menu scaleMenu{&settingsMenu};
        MenuItem smallestScale{&scaleMenu};
        MenuItem smallScale{&scaleMenu};
        MenuItem mediumScale{&scaleMenu};
        MenuItem largeScale{&scaleMenu};
        MenuItem largestScale{&scaleMenu};
      Menu outputMenu{&settingsMenu};
        MenuCheckItem aspectCorrection{&outputMenu};
        MenuCheckItem overscanCropping{&outputMenu};
        MenuCheckItem integralScaling{&outputMenu};
        MenuCheckItem blurEmulation{&outputMenu};
      Menu shaderMenu{&settingsMenu};
      MenuSeparator settingsSeparatorA{&settingsMenu};
      MenuCheckItem synchronizeVideo{&settingsMenu};
      MenuCheckItem synchronizeAudio{&settingsMenu};
      MenuCheckItem muteAudio{&settingsMenu};
      MenuCheckItem showStatusBar{&settingsMenu};
      MenuSeparator settingsSeparatorB{&settingsMenu};
      MenuItem videoSettings{&settingsMenu};
      MenuItem audioSettings{&settingsMenu};
      MenuItem inputSettings{&settingsMenu};
      MenuItem hotkeySettings{&settingsMenu};
      MenuItem pathSettings{&settingsMenu};
      MenuItem advancedSettings{&settingsMenu};
    Menu toolsMenu{&menuBar};
      Menu saveState{&toolsMenu};
      Menu loadState{&toolsMenu};
      MenuSeparator toolsSeparatorA{&toolsMenu};
      Menu speedMenu{&toolsMenu};
        MenuRadioItem speedSlowest{&speedMenu};
        MenuRadioItem speedSlow{&speedMenu};
        MenuRadioItem speedNormal{&speedMenu};
        MenuRadioItem speedFast{&speedMenu};
        MenuRadioItem speedFastest{&speedMenu};
        Group speedGroup{&speedSlowest, &speedSlow, &speedNormal, &speedFast, &speedFastest};
      MenuCheckItem pauseEmulation{&toolsMenu};
      MenuSeparator toolsSeparatorB{&toolsMenu};
      MenuItem cheatEditor{&toolsMenu};
      MenuItem stateManager{&toolsMenu};
    Menu helpMenu{&menuBar};
      MenuItem documentation{&helpMenu};
      MenuSeparator helpSeparator{&helpMenu};
      MenuItem about{&helpMenu};

  VerticalLayout layout{this};
    HorizontalLayout viewportLayout{&layout, Size{~0, ~0}, 0};
      Viewport viewport{&viewportLayout, Size{~0, ~0}, 0};
    HorizontalLayout statusLayout{&layout, Size{~0, StatusHeight}, 0};
      Label spacerIcon{&statusLayout, Size{8, ~0}, 0};
      Canvas statusIcon{&statusLayout, Size{16, ~0}, 0};
      Label spacerLeft{&statusLayout, Size{4, ~0}, 0};
      Label statusLeft{&statusLayout, Size{~0, ~0}, 0};
      Label statusRight{&statusLayout, Size{80, ~0}, 0};
      Label spacerRight{&statusLayout, Size{8, ~0}, 0};
};

extern unique_pointer<AboutWindow> aboutWindow;
extern unique_pointer<Presentation> presentation;
