#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "paths.cpp"
#include "advanced.cpp"
#include "utility/inputcapture.cpp"

SettingsWindow::SettingsWindow() {
  setObjectName("settings-window");
  setWindowTitle("Configuration Settings");
  setMinimumSize(625, 360);

  list = new QListWidget;
  list->addItem(video = new QListWidgetItem("Video"));
  list->addItem(audio = new QListWidgetItem("Audio"));
  list->addItem(input = new QListWidgetItem("Input"));
  list->addItem(paths = new QListWidgetItem("Paths"));
  list->addItem(advanced = new QListWidgetItem("Advanced"));
  list->setCurrentItem(input);  //select most frequently used panel by default
  list->setFixedWidth(135);

  panel = new QWidget;
  panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  layout = new QHBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  layout->addWidget(list);
  layout->addWidget(panel);
  setLayout(layout);

  winVideoSettings    = new VideoSettingsWindow;
  winAudioSettings    = new AudioSettingsWindow;
  winInputSettings    = new InputSettingsWindow;
  winPathSettings     = new PathSettingsWindow;
  winAdvancedSettings = new AdvancedSettingsWindow;
  winInputCapture     = new InputCaptureWindow;

  panelLayout = new QStackedLayout(panel);
  panelLayout->addWidget(winVideoSettings->panel);
  panelLayout->addWidget(winAudioSettings->panel);
  panelLayout->addWidget(winInputSettings->panel);
  panelLayout->addWidget(winPathSettings->panel);
  panelLayout->addWidget(winAdvancedSettings->panel);
  panel->setLayout(panelLayout);

  connect(list, SIGNAL(currentRowChanged(int)), this, SLOT(listChanged()));

  listChanged();
}

void SettingsWindow::listChanged() {
  QListWidgetItem *item = list->currentItem();

  if(item == video)    panelLayout->setCurrentWidget(winVideoSettings->panel);
  if(item == audio)    panelLayout->setCurrentWidget(winAudioSettings->panel);
  if(item == input)    panelLayout->setCurrentWidget(winInputSettings->panel);
  if(item == paths)    panelLayout->setCurrentWidget(winPathSettings->panel);
  if(item == advanced) panelLayout->setCurrentWidget(winAdvancedSettings->panel);
}
