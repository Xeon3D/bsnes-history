MainWindow::MainWindow() {
  setObjectName("main-window");
  setWindowTitle(utf8() << bsnesTitle << " v" << bsnesVersion);

  //menu bar
  #if defined(PLATFORM_OSX)
  menuBar = new QMenuBar(0);
  #else
  menuBar = new QMenuBar;
  #endif

  system = menuBar->addMenu("System");
    system_load = system->addAction("Load Cartridge ...");
    system->addSeparator();
    system_power = system->addMenu("Power");
      system_power_on = system_power->addAction("On");
      system_power_on->setCheckable(true);
      system_power_off = system_power->addAction("Off");
      system_power_off->setCheckable(true);
    system_reset = system->addAction("Reset");
    system->addSeparator();
    system_port1 = system->addMenu("Controller Port 1");
      system_port1_none = system_port1->addAction("None");
      system_port1_none->setCheckable(true);
      system_port1_joypad = system_port1->addAction("Joypad");
      system_port1_joypad->setCheckable(true);
      system_port1_multitap = system_port1->addAction("Multitap");
      system_port1_multitap->setCheckable(true);
      system_port1_mouse = system_port1->addAction("Mouse");
      system_port1_mouse->setCheckable(true);
    system_port2 = system->addMenu("Controller Port 2");
      system_port2_none = system_port2->addAction("None");
      system_port2_none->setCheckable(true);
      system_port2_joypad = system_port2->addAction("Joypad");
      system_port2_joypad->setCheckable(true);
      system_port2_multitap = system_port2->addAction("Multitap");
      system_port2_multitap->setCheckable(true);
      system_port2_mouse = system_port2->addAction("Mouse");
      system_port2_mouse->setCheckable(true);
      system_port2_superscope = system_port2->addAction("Super Scope");
      system_port2_superscope->setCheckable(true);
      system_port2_justifier = system_port2->addAction("Justifier");
      system_port2_justifier->setCheckable(true);
      system_port2_justifiers = system_port2->addAction("Two Justifiers");
      system_port2_justifiers->setCheckable(true);
    #if !defined(PLATFORM_OSX)
    system->addSeparator();
    #endif
    system_exit = system->addAction("Exit");
    system_exit->setMenuRole(QAction::QuitRole);

  settings = menuBar->addMenu("Settings");
    settings_videoMode = settings->addMenu("Video Mode");
      settings_videoMode_1x = settings_videoMode->addAction("Scale 1x");
      settings_videoMode_1x->setCheckable(true);
      settings_videoMode_2x = settings_videoMode->addAction("Scale 2x");
      settings_videoMode_2x->setCheckable(true);
      settings_videoMode_3x = settings_videoMode->addAction("Scale 3x");
      settings_videoMode_3x->setCheckable(true);
      settings_videoMode_4x = settings_videoMode->addAction("Scale 4x");
      settings_videoMode_4x->setCheckable(true);
      settings_videoMode_max = settings_videoMode->addAction("Scale Max");
      settings_videoMode_max->setCheckable(true);
      settings_videoMode->addSeparator();
      settings_videoMode_correctAspectRatio = settings_videoMode->addAction("Correct Aspect Ratio");
      settings_videoMode_correctAspectRatio->setCheckable(true);
      settings_videoMode_fullscreen = settings_videoMode->addAction("Fullscreen");
      settings_videoMode_fullscreen->setCheckable(true);
      settings_videoMode->addSeparator();
      settings_videoMode_ntsc = settings_videoMode->addAction("NTSC");
      settings_videoMode_ntsc->setCheckable(true);
      settings_videoMode_pal = settings_videoMode->addAction("PAL");
      settings_videoMode_pal->setCheckable(true);
    settings_videoFilter = settings->addMenu("Video Filter");
      settings_videoFilter_point = settings_videoFilter->addAction("Point");
      settings_videoFilter_point->setCheckable(true);
      settings_videoFilter_linear = settings_videoFilter->addAction("Linear");
      settings_videoFilter_linear->setCheckable(true);
      settings_videoFilter->addSeparator();
      settings_videoFilter_none = settings_videoFilter->addAction("None");
      settings_videoFilter_none->setCheckable(true);
      settings_videoFilter_scanline = settings_videoFilter->addAction("Scanline");
      settings_videoFilter_scanline->setCheckable(true);
      settings_videoFilter_scale2x = settings_videoFilter->addAction("Scale2x");
      settings_videoFilter_scale2x->setCheckable(true);
      settings_videoFilter_lq2x = settings_videoFilter->addAction("LQ2x");
      settings_videoFilter_lq2x->setCheckable(true);
      settings_videoFilter_hq2x = settings_videoFilter->addAction("HQ2x");
      settings_videoFilter_hq2x->setCheckable(true);
      settings_videoFilter_ntsc = settings_videoFilter->addAction("NTSC");
      settings_videoFilter_ntsc->setCheckable(true);
    settings->addSeparator();
    settings_muteAudio = settings->addAction("Mute Audio Output");
    settings_muteAudio->setCheckable(true);
    settings->addSeparator();
    settings_emulationSpeed = settings->addMenu("Emulation Speed");
      settings_emulationSpeed_slowest = settings_emulationSpeed->addAction("50%");
      settings_emulationSpeed_slowest->setCheckable(true);
      settings_emulationSpeed_slow = settings_emulationSpeed->addAction("75%");
      settings_emulationSpeed_slow->setCheckable(true);
      settings_emulationSpeed_normal = settings_emulationSpeed->addAction("100%");
      settings_emulationSpeed_normal->setCheckable(true);
      settings_emulationSpeed_fast = settings_emulationSpeed->addAction("150%");
      settings_emulationSpeed_fast->setCheckable(true);
      settings_emulationSpeed_fastest = settings_emulationSpeed->addAction("200%");
      settings_emulationSpeed_fastest->setCheckable(true);
      settings_emulationSpeed->addSeparator();
      settings_emulationSpeed_syncVideo = settings_emulationSpeed->addAction("Sync Video");
      settings_emulationSpeed_syncVideo->setCheckable(true);
      settings_emulationSpeed_syncAudio = settings_emulationSpeed->addAction("Sync Audio");
      settings_emulationSpeed_syncAudio->setCheckable(true);
    settings_configuration = settings->addAction("Configuration ...");
    settings_configuration->setMenuRole(QAction::PreferencesRole);

  tools = menuBar->addMenu("Tools");
    tools_cheatEditor = tools->addAction("Cheat Editor ...");
    tools_stateManager = tools->addAction("State Manager ...");
    #if defined(DEBUGGER)
    tools->addSeparator();
    #endif
    tools_debugger = tools->addAction("Debugger ...");
    #if !defined(DEBUGGER)
    tools_debugger->setVisible(false);
    #endif

  help = menuBar->addMenu("Help");
    help_documentation = help->addAction("Documentation ...");
    help_license = help->addAction("License ...");
    #if !defined(PLATFORM_OSX)
    help->addSeparator();
    #endif
    help_about = help->addAction("About ...");
    help_about->setMenuRole(QAction::AboutRole);

  //canvas
  canvasContainer = new CanvasObject;
  canvasContainer->setAcceptDrops(true); {
    canvasContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    canvasContainer->setObjectName("backdrop");

    canvasLayout = new QVBoxLayout; {
      canvasLayout->setMargin(0);
      canvasLayout->setAlignment(Qt::AlignCenter);

      canvas = new CanvasWidget;
      canvas->setAcceptDrops(true);
      canvas->setFocusPolicy(Qt::StrongFocus);
      canvas->setAttribute(Qt::WA_PaintOnScreen, true);  //disable Qt painting on focus / resize

      QPalette palette;
      palette.setColor(QPalette::Window, QColor(0, 0, 0));
      canvas->setPalette(palette);
      canvas->setAutoFillBackground(true);
    }
    canvasLayout->addWidget(canvas);
  }
  canvasContainer->setLayout(canvasLayout);

  //status bar
  statusBar = new QStatusBar;
  statusBar->showMessage("");
  systemState = new QLabel;
  statusBar->addPermanentWidget(systemState);

  //layout
  layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);
  #if !defined(PLATFORM_OSX)
  layout->addWidget(menuBar);
  #endif
  layout->addWidget(canvasContainer);
  layout->addWidget(statusBar);
  setLayout(layout);

  //slots
  connect(system_load, SIGNAL(triggered()), this, SLOT(loadCartridge()));
  connect(system_power_on, SIGNAL(triggered()), this, SLOT(powerOn()));
  connect(system_power_off, SIGNAL(triggered()), this, SLOT(powerOff()));
  connect(system_reset, SIGNAL(triggered()), this, SLOT(reset()));
  connect(system_port1_none, SIGNAL(triggered()), this, SLOT(setPort1None()));
  connect(system_port1_joypad, SIGNAL(triggered()), this, SLOT(setPort1Joypad()));
  connect(system_port1_multitap, SIGNAL(triggered()), this, SLOT(setPort1Multitap()));
  connect(system_port1_mouse, SIGNAL(triggered()), this, SLOT(setPort1Mouse()));
  connect(system_port2_none, SIGNAL(triggered()), this, SLOT(setPort2None()));
  connect(system_port2_joypad, SIGNAL(triggered()), this, SLOT(setPort2Joypad()));
  connect(system_port2_multitap, SIGNAL(triggered()), this, SLOT(setPort2Multitap()));
  connect(system_port2_mouse, SIGNAL(triggered()), this, SLOT(setPort2Mouse()));
  connect(system_port2_superscope, SIGNAL(triggered()), this, SLOT(setPort2SuperScope()));
  connect(system_port2_justifier, SIGNAL(triggered()), this, SLOT(setPort2Justifier()));
  connect(system_port2_justifiers, SIGNAL(triggered()), this, SLOT(setPort2Justifiers()));
  connect(system_exit, SIGNAL(triggered()), this, SLOT(quit()));
  connect(settings_videoMode_1x, SIGNAL(triggered()), this, SLOT(setVideoMode1x()));
  connect(settings_videoMode_2x, SIGNAL(triggered()), this, SLOT(setVideoMode2x()));
  connect(settings_videoMode_3x, SIGNAL(triggered()), this, SLOT(setVideoMode3x()));
  connect(settings_videoMode_4x, SIGNAL(triggered()), this, SLOT(setVideoMode4x()));
  connect(settings_videoMode_max, SIGNAL(triggered()), this, SLOT(setVideoModeMax()));
  connect(settings_videoMode_correctAspectRatio, SIGNAL(triggered()), this, SLOT(toggleAspectCorrection()));
  connect(settings_videoMode_fullscreen, SIGNAL(triggered()), this, SLOT(toggleFullscreen()));
  connect(settings_videoMode_ntsc, SIGNAL(triggered()), this, SLOT(setVideoNtsc()));
  connect(settings_videoMode_pal, SIGNAL(triggered()), this, SLOT(setVideoPal()));
  connect(settings_videoFilter_point, SIGNAL(triggered()), this, SLOT(setPointFilter()));
  connect(settings_videoFilter_linear, SIGNAL(triggered()), this, SLOT(setLinearFilter()));
  connect(settings_videoFilter_none, SIGNAL(triggered()), this, SLOT(setNoFilter()));
  connect(settings_videoFilter_scanline, SIGNAL(triggered()), this, SLOT(setScanlineFilter()));
  connect(settings_videoFilter_scale2x, SIGNAL(triggered()), this, SLOT(setScale2xFilter()));
  connect(settings_videoFilter_lq2x, SIGNAL(triggered()), this, SLOT(setLq2xFilter()));
  connect(settings_videoFilter_hq2x, SIGNAL(triggered()), this, SLOT(setHq2xFilter()));
  connect(settings_videoFilter_ntsc, SIGNAL(triggered()), this, SLOT(setNtscFilter()));
  connect(settings_muteAudio, SIGNAL(triggered()), this, SLOT(muteAudio()));
  connect(settings_emulationSpeed_slowest, SIGNAL(triggered()), this, SLOT(setSpeedSlowest()));
  connect(settings_emulationSpeed_slow, SIGNAL(triggered()), this, SLOT(setSpeedSlow()));
  connect(settings_emulationSpeed_normal, SIGNAL(triggered()), this, SLOT(setSpeedNormal()));
  connect(settings_emulationSpeed_fast, SIGNAL(triggered()), this, SLOT(setSpeedFast()));
  connect(settings_emulationSpeed_fastest, SIGNAL(triggered()), this, SLOT(setSpeedFastest()));
  connect(settings_emulationSpeed_syncVideo, SIGNAL(triggered()), this, SLOT(syncVideo()));
  connect(settings_emulationSpeed_syncAudio, SIGNAL(triggered()), this, SLOT(syncAudio()));
  connect(settings_configuration, SIGNAL(triggered()), this, SLOT(showConfigWindow()));
  connect(tools_cheatEditor, SIGNAL(triggered()), this, SLOT(showCheatEditor()));
  connect(tools_stateManager, SIGNAL(triggered()), this, SLOT(showStateManager()));
  connect(tools_debugger, SIGNAL(triggered()), this, SLOT(showDebugger()));
  connect(help_documentation, SIGNAL(triggered()), this, SLOT(showDocumentation()));
  connect(help_license, SIGNAL(triggered()), this, SLOT(showLicense()));
  connect(help_about, SIGNAL(triggered()), this, SLOT(showAbout()));

  syncUi();
}

void MainWindow::syncUi() {
  system_power->setEnabled(SNES::cartridge.loaded());
  system_power_on->setChecked (application.power == true);
  system_power_off->setChecked(application.power == false);
  system_reset->setEnabled(SNES::cartridge.loaded() && application.power);

  system_port1_none->setChecked      (SNES::config.controller_port1 == SNES::Input::DeviceNone);
  system_port1_joypad->setChecked    (SNES::config.controller_port1 == SNES::Input::DeviceJoypad);
  system_port1_multitap->setChecked  (SNES::config.controller_port1 == SNES::Input::DeviceMultitap);
  system_port1_mouse->setChecked     (SNES::config.controller_port1 == SNES::Input::DeviceMouse);
  system_port2_none->setChecked      (SNES::config.controller_port2 == SNES::Input::DeviceNone);
  system_port2_joypad->setChecked    (SNES::config.controller_port2 == SNES::Input::DeviceJoypad);
  system_port2_multitap->setChecked  (SNES::config.controller_port2 == SNES::Input::DeviceMultitap);
  system_port2_mouse->setChecked     (SNES::config.controller_port2 == SNES::Input::DeviceMouse);
  system_port2_superscope->setChecked(SNES::config.controller_port2 == SNES::Input::DeviceSuperScope);
  system_port2_justifier->setChecked (SNES::config.controller_port2 == SNES::Input::DeviceJustifier);
  system_port2_justifiers->setChecked(SNES::config.controller_port2 == SNES::Input::DeviceJustifiers);

  settings_videoMode_1x->setChecked (config.video.context->multiplier == 1);
  settings_videoMode_2x->setChecked (config.video.context->multiplier == 2);
  settings_videoMode_3x->setChecked (config.video.context->multiplier == 3);
  settings_videoMode_4x->setChecked (config.video.context->multiplier == 4);
  settings_videoMode_max->setChecked(config.video.context->multiplier >= 5);

  settings_videoMode_correctAspectRatio->setChecked(config.video.context->correctAspectRatio);
  settings_videoMode_fullscreen->setChecked(config.video.isFullscreen);
  settings_videoMode_ntsc->setChecked(config.video.context->region == 0);
  settings_videoMode_pal->setChecked (config.video.context->region == 1);

  settings_videoFilter_point->setChecked   (config.video.context->hwFilter == 0);
  settings_videoFilter_linear->setChecked  (config.video.context->hwFilter == 1);
  settings_videoFilter_none->setChecked    (config.video.context->swFilter == 0);
  settings_videoFilter_scanline->setChecked(config.video.context->swFilter == 1);
  settings_videoFilter_scale2x->setChecked (config.video.context->swFilter == 2);
  settings_videoFilter_lq2x->setChecked    (config.video.context->swFilter == 3);
  settings_videoFilter_hq2x->setChecked    (config.video.context->swFilter == 4);
  settings_videoFilter_ntsc->setChecked    (config.video.context->swFilter == 5);

  settings_muteAudio->setChecked(config.audio.mute);

  settings_emulationSpeed_slowest->setChecked(config.system.speed == 0);
  settings_emulationSpeed_slow->setChecked   (config.system.speed == 1);
  settings_emulationSpeed_normal->setChecked (config.system.speed == 2);
  settings_emulationSpeed_fast->setChecked   (config.system.speed == 3);
  settings_emulationSpeed_fastest->setChecked(config.system.speed == 4);

  settings_emulationSpeed_syncVideo->setChecked(config.video.synchronize);
  settings_emulationSpeed_syncAudio->setChecked(config.audio.synchronize);
}

bool MainWindow::isActive() {
  return isActiveWindow() && !isMinimized();
}

void MainWindow::loadCartridge() {
  diskBrowser->loadAnyCartridge();
}

void MainWindow::powerOn()  { utility.modifySystemState(Utility::PowerOn); }
void MainWindow::powerOff() { utility.modifySystemState(Utility::PowerOff); }
void MainWindow::reset()    { utility.modifySystemState(Utility::Reset); }

void MainWindow::setPort1None()       { SNES::config.controller_port1 = SNES::Input::DeviceNone;       utility.updateControllers(); syncUi(); }
void MainWindow::setPort1Joypad()     { SNES::config.controller_port1 = SNES::Input::DeviceJoypad;     utility.updateControllers(); syncUi(); }
void MainWindow::setPort1Multitap()   { SNES::config.controller_port1 = SNES::Input::DeviceMultitap;   utility.updateControllers(); syncUi(); }
void MainWindow::setPort1Mouse()      { SNES::config.controller_port1 = SNES::Input::DeviceMouse;      utility.updateControllers(); syncUi(); }
void MainWindow::setPort2None()       { SNES::config.controller_port2 = SNES::Input::DeviceNone;       utility.updateControllers(); syncUi(); }
void MainWindow::setPort2Joypad()     { SNES::config.controller_port2 = SNES::Input::DeviceJoypad;     utility.updateControllers(); syncUi(); }
void MainWindow::setPort2Multitap()   { SNES::config.controller_port2 = SNES::Input::DeviceMultitap;   utility.updateControllers(); syncUi(); }
void MainWindow::setPort2Mouse()      { SNES::config.controller_port2 = SNES::Input::DeviceMouse;      utility.updateControllers(); syncUi(); }
void MainWindow::setPort2SuperScope() { SNES::config.controller_port2 = SNES::Input::DeviceSuperScope; utility.updateControllers(); syncUi(); }
void MainWindow::setPort2Justifier()  { SNES::config.controller_port2 = SNES::Input::DeviceJustifier;  utility.updateControllers(); syncUi(); }
void MainWindow::setPort2Justifiers() { SNES::config.controller_port2 = SNES::Input::DeviceJustifiers; utility.updateControllers(); syncUi(); }

void MainWindow::quit() {
  application.terminate = true;
}

void MainWindow::setVideoMode1x()  { config.video.context->multiplier = 1; utility.resizeMainWindow(); syncUi(); }
void MainWindow::setVideoMode2x()  { config.video.context->multiplier = 2; utility.resizeMainWindow(); syncUi(); }
void MainWindow::setVideoMode3x()  { config.video.context->multiplier = 3; utility.resizeMainWindow(); syncUi(); }
void MainWindow::setVideoMode4x()  { config.video.context->multiplier = 4; utility.resizeMainWindow(); syncUi(); }
void MainWindow::setVideoModeMax() { config.video.context->multiplier = 9; utility.resizeMainWindow(); syncUi(); }

void MainWindow::toggleAspectCorrection() {
  config.video.context->correctAspectRatio = settings_videoMode_correctAspectRatio->isChecked();
  utility.resizeMainWindow();
}

void MainWindow::toggleFullscreen() {
  config.video.isFullscreen = settings_videoMode_fullscreen->isChecked();
  utility.updateFullscreenState();
  utility.resizeMainWindow();
  syncUi();
}

void MainWindow::setVideoNtsc() { config.video.context->region = 0; utility.updateVideoMode(); utility.resizeMainWindow(); syncUi(); }
void MainWindow::setVideoPal()  { config.video.context->region = 1; utility.updateVideoMode(); utility.resizeMainWindow(); syncUi(); }

void MainWindow::setPointFilter()    { config.video.context->hwFilter = 0; utility.updateHardwareFilter(); syncUi(); }
void MainWindow::setLinearFilter()   { config.video.context->hwFilter = 1; utility.updateHardwareFilter(); syncUi(); }
void MainWindow::setNoFilter()       { config.video.context->swFilter = 0; utility.updateSoftwareFilter(); syncUi(); }
void MainWindow::setScanlineFilter() { config.video.context->swFilter = 1; utility.updateSoftwareFilter(); syncUi(); }
void MainWindow::setScale2xFilter()  { config.video.context->swFilter = 2; utility.updateSoftwareFilter(); syncUi(); }
void MainWindow::setLq2xFilter()     { config.video.context->swFilter = 3; utility.updateSoftwareFilter(); syncUi(); }
void MainWindow::setHq2xFilter()     { config.video.context->swFilter = 4; utility.updateSoftwareFilter(); syncUi(); }
void MainWindow::setNtscFilter()     { config.video.context->swFilter = 5; utility.updateSoftwareFilter(); syncUi(); }

void MainWindow::muteAudio() { config.audio.mute = settings_muteAudio->isChecked(); }

void MainWindow::setSpeedSlowest() { config.system.speed = 0; utility.updateEmulationSpeed(); syncUi(); }
void MainWindow::setSpeedSlow()    { config.system.speed = 1; utility.updateEmulationSpeed(); syncUi(); }
void MainWindow::setSpeedNormal()  { config.system.speed = 2; utility.updateEmulationSpeed(); syncUi(); }
void MainWindow::setSpeedFast()    { config.system.speed = 3; utility.updateEmulationSpeed(); syncUi(); }
void MainWindow::setSpeedFastest() { config.system.speed = 4; utility.updateEmulationSpeed(); syncUi(); }

void MainWindow::syncVideo() { config.video.synchronize = settings_emulationSpeed_syncVideo->isChecked(); utility.updateAvSync(); }
void MainWindow::syncAudio() { config.audio.synchronize = settings_emulationSpeed_syncAudio->isChecked(); utility.updateAvSync(); }

void MainWindow::showConfigWindow() {
  settingsWindow->showAt(-1.0, 1.0);
  settingsWindow->setFocus();
}

void MainWindow::showCheatEditor()  { toolsWindow->showCheatEditor();  }
void MainWindow::showStateManager() { toolsWindow->showStateManager(); }
void MainWindow::showDebugger()     { debugger->show(); }

void MainWindow::showDocumentation()  {
  QFile file(":/documentation.html");
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    htmlViewerWindow->show("Usage Documentation", file.readAll().constData());
    file.close();
  }
}

void MainWindow::showLicense() {
  QFile file(":/license.html");
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    htmlViewerWindow->show("License Agreement", file.readAll().constData());
    file.close();
  }
}
void MainWindow::showAbout() {
  aboutWindow->showAt(0.0, 0.0);
  aboutWindow->setFocus();
}

void MainWindow::closeEvent(QCloseEvent*) {
  quit();
}

//============
//CanvasObject
//============

//implement drag-and-drop support:
//drag cartridge image onto main window canvas area to load

void CanvasObject::dragEnterEvent(QDragEnterEvent *event) {
  if(event->mimeData()->hasUrls()) {
    //do not accept multiple files at once
    if(event->mimeData()->urls().count() == 1) event->acceptProposedAction();
  }
}

void CanvasObject::dropEvent(QDropEvent *event) {
  if(event->mimeData()->hasUrls()) {
    QList<QUrl> list = event->mimeData()->urls();
    if(list.count() == 1) utility.loadCartridge(list.at(0).toLocalFile().toUtf8().constData());
  }
}

//accept all key events for this widget to prevent system chime from playing on OS X
//key events are actually handled by Input class

void CanvasObject::keyPressEvent(QKeyEvent *event) {
}

void CanvasObject::keyReleaseEvent(QKeyEvent *event) {
}

//===========
//CanvasWidget
//============

//custom video render and mouse capture functionality

QPaintEngine* CanvasWidget::paintEngine() const {
  if(SNES::cartridge.loaded()) {
    video.refresh();
    return 0;
  }
  return QWidget::paintEngine();
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event) {
  //acquire exclusive mode access to mouse when video output widget is clicked
  //(will only acquire if cart is loaded, and mouse / lightgun is in use.)
  utility.acquireMouse();
}

void CanvasWidget::paintEvent(QPaintEvent *event) {
  event->ignore();
}
