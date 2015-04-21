#include "../tomoko.hpp"
#include "input.cpp"
#include "hotkeys.cpp"
#include "advanced.cpp"
SettingsManager* settingsManager = nullptr;

SettingsManager::SettingsManager() {
  settingsManager = this;

  layout.setMargin(5);
  statusBar.setFont(Font::sans(8, "Bold"));

  setTitle("Configuration Settings");
  setSize({600, 400});
  setPlacement(0.0, 1.0);

  onSize([&] {
    input.mappingList.resizeColumns();
    hotkeys.mappingList.resizeColumns();
  });
}

auto SettingsManager::show(unsigned setting) -> void {
  panel.item(setting)->setSelected();
  setVisible();
  setFocused();
  doSize();
}
