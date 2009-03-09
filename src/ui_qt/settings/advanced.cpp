void AdvancedSettingsWindow::setup() {
  panel = new QWidget;

  layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);

  title = new QLabel("Advanced Configuration Settings");
  title->setProperty("class", "title");
  layout->addWidget(title);

  driverLayout = new QGridLayout;
  driverLayout->setVerticalSpacing(0);
  driverLayout->setHorizontalSpacing(Style::WidgetSpacing); {
    videoLabel = new QLabel("Video driver:");
    driverLayout->addWidget(videoLabel, 0, 0);

    audioLabel = new QLabel("Audio driver:");
    driverLayout->addWidget(audioLabel, 0, 1);

    inputLabel = new QLabel("Input driver:");
    driverLayout->addWidget(inputLabel, 0, 2);

    videoDriver = new QComboBox;
    driverLayout->addWidget(videoDriver, 1, 0);

    audioDriver = new QComboBox;
    driverLayout->addWidget(audioDriver, 1, 1);

    inputDriver = new QComboBox;
    driverLayout->addWidget(inputDriver, 1, 2);

    driverInfo = new QLabel("<i>Note: driver changes require restart to take effect.</i>");
    driverLayout->addWidget(driverInfo, 2, 0, 1, 3);
  }
  layout->addLayout(driverLayout);
  layout->addSpacing(Style::WidgetSpacing);

  regionTitle = new QLabel("Hardware region:");
  layout->addWidget(regionTitle);

  regionLayout = new QHBoxLayout;
  regionLayout->setSpacing(Style::WidgetSpacing); {
    regionGroup = new QButtonGroup(panel);

    regionAuto = new QRadioButton("Auto-detect");
    regionAuto->setToolTip("Automatically select hardware region on cartridge load");
    regionGroup->addButton(regionAuto);
    regionLayout->addWidget(regionAuto);

    regionNTSC = new QRadioButton("NTSC");
    regionNTSC->setToolTip("Force NTSC region (Japan, Korea, US)");
    regionGroup->addButton(regionNTSC);
    regionLayout->addWidget(regionNTSC);

    regionPAL = new QRadioButton("PAL");
    regionPAL->setToolTip("Force PAL region (Europe, ...)");
    regionGroup->addButton(regionPAL);
    regionLayout->addWidget(regionPAL);
  }
  layout->addLayout(regionLayout);
  layout->addSpacing(Style::WidgetSpacing);

  portTitle = new QLabel("Expansion port device:");
  layout->addWidget(portTitle);

  portLayout = new QHBoxLayout;
  portLayout->setSpacing(Style::WidgetSpacing); {
    portGroup = new QButtonGroup(panel);

    portSatellaview = new QRadioButton("Satellaview");
    portGroup->addButton(portSatellaview);
    portLayout->addWidget(portSatellaview);

    portNone = new QRadioButton("None");
    portGroup->addButton(portNone);
    portLayout->addWidget(portNone);

    portSpacer = new QWidget;
    portLayout->addWidget(portSpacer);
  }
  layout->addLayout(portLayout);
  layout->addSpacing(Style::WidgetSpacing);

  focusTitle = new QLabel("When main window does not have focus:");
  layout->addWidget(focusTitle);

  focusLayout = new QHBoxLayout;
  focusLayout->setSpacing(Style::WidgetSpacing); {
    focusButtonGroup = new QButtonGroup(panel);

    focusPause = new QRadioButton("Pause emulation");
    focusPause->setToolTip("Ideal for prolonged multi-tasking");
    focusButtonGroup->addButton(focusPause);
    focusLayout->addWidget(focusPause);

    focusIgnore = new QRadioButton("Ignore input");
    focusIgnore->setToolTip("Ideal for light multi-tasking when using keyboard");
    focusButtonGroup->addButton(focusIgnore);
    focusLayout->addWidget(focusIgnore);

    focusAllow = new QRadioButton("Allow input");
    focusAllow->setToolTip("Ideal for light multi-tasking when using joypad(s)");
    focusButtonGroup->addButton(focusAllow);
    focusLayout->addWidget(focusAllow);
  }
  layout->addLayout(focusLayout);

  spacer = new QWidget;
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(spacer);

  panel->setLayout(layout);
  initializeUi();

  connect(videoDriver, SIGNAL(currentIndexChanged(int)), this, SLOT(videoDriverChange(int)));
  connect(audioDriver, SIGNAL(currentIndexChanged(int)), this, SLOT(audioDriverChange(int)));
  connect(inputDriver, SIGNAL(currentIndexChanged(int)), this, SLOT(inputDriverChange(int)));
  connect(regionAuto, SIGNAL(pressed()), this, SLOT(setRegionAuto()));
  connect(regionNTSC, SIGNAL(pressed()), this, SLOT(setRegionNTSC()));
  connect(regionPAL, SIGNAL(pressed()), this, SLOT(setRegionPAL()));
  connect(portSatellaview, SIGNAL(pressed()), this, SLOT(setPortSatellaview()));
  connect(portNone, SIGNAL(pressed()), this, SLOT(setPortNone()));
  connect(focusPause, SIGNAL(pressed()), this, SLOT(pauseWithoutFocus()));
  connect(focusIgnore, SIGNAL(pressed()), this, SLOT(ignoreInputWithoutFocus()));
  connect(focusAllow, SIGNAL(pressed()), this, SLOT(allowInputWithoutFocus()));
}

void AdvancedSettingsWindow::initializeUi() {
  lstring part;

  part.split(";", video.driver_list());
  for(unsigned i = 0; i < part.size(); i++) {
    videoDriver->addItem(utf8() << part[i]);
    if(part[i] == config.system.video) videoDriver->setCurrentIndex(i);
  }

  part.split(";", audio.driver_list());
  for(unsigned i = 0; i < part.size(); i++) {
    audioDriver->addItem(utf8() << part[i]);
    if(part[i] == config.system.audio) audioDriver->setCurrentIndex(i);
  }

  part.split(";", input.driver_list());
  for(unsigned i = 0; i < part.size(); i++) {
    inputDriver->addItem(utf8() << part[i]);
    if(part[i] == config.system.input) inputDriver->setCurrentIndex(i);
  }

  regionAuto->setChecked(snes.config.region == SNES::Autodetect);
  regionNTSC->setChecked(snes.config.region == SNES::NTSC);
  regionPAL->setChecked (snes.config.region == SNES::PAL);

  portSatellaview->setChecked(snes.config.expansion_port == SNES::ExpansionBSX);
  portNone->setChecked       (snes.config.expansion_port == SNES::ExpansionNone);

  focusPause->setChecked (config.input.focusPolicy == Configuration::Input::FocusPolicyPauseEmulation);
  focusIgnore->setChecked(config.input.focusPolicy == Configuration::Input::FocusPolicyIgnoreInput);
  focusAllow->setChecked (config.input.focusPolicy == Configuration::Input::FocusPolicyAllowInput);
}

void AdvancedSettingsWindow::videoDriverChange(int index) {
  if(index >= 0) config.system.video = videoDriver->itemText(index).toUtf8().data();
}

void AdvancedSettingsWindow::audioDriverChange(int index) {
  if(index >= 0) config.system.audio = audioDriver->itemText(index).toUtf8().data();
}

void AdvancedSettingsWindow::inputDriverChange(int index) {
  if(index >= 0) config.system.input = inputDriver->itemText(index).toUtf8().data();
}

void AdvancedSettingsWindow::setRegionAuto() { snes.config.region = SNES::Autodetect; }
void AdvancedSettingsWindow::setRegionNTSC() { snes.config.region = SNES::NTSC; }
void AdvancedSettingsWindow::setRegionPAL()  { snes.config.region = SNES::PAL; }

void AdvancedSettingsWindow::setPortSatellaview() { snes.config.expansion_port = SNES::ExpansionBSX; }
void AdvancedSettingsWindow::setPortNone()        { snes.config.expansion_port = SNES::ExpansionNone; }

void AdvancedSettingsWindow::pauseWithoutFocus()       { config.input.focusPolicy = Configuration::Input::FocusPolicyPauseEmulation; }
void AdvancedSettingsWindow::ignoreInputWithoutFocus() { config.input.focusPolicy = Configuration::Input::FocusPolicyIgnoreInput; }
void AdvancedSettingsWindow::allowInputWithoutFocus()  { config.input.focusPolicy = Configuration::Input::FocusPolicyAllowInput; }
