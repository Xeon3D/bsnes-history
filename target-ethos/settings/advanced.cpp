AdvancedSettings *advancedSettings = nullptr;

AdvancedSettings::AdvancedSettings() {
  driverTitle.setFont(application->titleFont);
  driverTitle.setText("Driver Selection");
  videoLabel.setText("Video:");
  audioLabel.setText("Audio:");
  inputLabel.setText("Input:");
  libraryTitle.setFont(application->titleFont);
  libraryTitle.setText("Game Library Path");
  libraryLabel.setText("Path:");
  libraryPath.setEditable(false);
  string path = string::read({configpath(), "higan/library.cfg"}).strip().transform("\\", "/");
  if(path.empty()) path = {userpath(), "Emulation/"};
  if(path.endswith("/") == false) path.append("/");
  libraryPath.setText(path);
  libraryBrowse.setText("Browse ...");
  infoLabel.setFont(application->boldFont);
  string profile;
  #if defined(PROFILE_ACCURACY)
  profile = "Accuracy";
  #elif defined(PROFILE_BALANCED)
  profile = "Balanced";
  #elif defined(PROFILE_PERFORMANCE)
  profile = "Performance";
  #endif
  infoLabel.setText({
    Emulator::Name, " v", Emulator::Version, "\n",
    "  ", profile, " Profile\n",
    "  Author: byuu\n",
    "  Website: http://byuu.org/\n",
    "  License: GPLv3"
  });

  lstring list;

  list.split(";", video.driver_list());
  for(unsigned n = 0; n < list.size(); n++) {
    videoDriver.append(list[n]);
    if(list[n] == config->video.driver) videoDriver.setSelection(n);
  }

  list.split(";", audio.driver_list());
  for(unsigned n = 0; n < list.size(); n++) {
    audioDriver.append(list[n]);
    if(list[n] == config->audio.driver) audioDriver.setSelection(n);
  }

  list.split(";", input.driver_list());
  for(unsigned n = 0; n < list.size(); n++) {
    inputDriver.append(list[n]);
    if(list[n] == config->input.driver) inputDriver.setSelection(n);
  }

  append(driverTitle, {~0, 0}, 5);
  append(driverLayout, {~0, 0}, 15);
    driverLayout.append(videoLabel, {0, 0}, 5);
    driverLayout.append(videoDriver, {~0, 0}, 5);
    driverLayout.append(audioLabel, {0, 0}, 5);
    driverLayout.append(audioDriver, {~0, 0}, 5);
    driverLayout.append(inputLabel, {0, 0}, 5);
    driverLayout.append(inputDriver, {~0, 0});
  append(libraryTitle, {~0, 0}, 5);
  append(libraryLayout, {~0, 0}, 15);
    libraryLayout.append(libraryLabel, {0, 0}, 5);
    libraryLayout.append(libraryPath, {~0, 0}, 5);
    libraryLayout.append(libraryBrowse, {80, 0});
  append(spacer, {~0, ~0});
  append(infoLabel, {~0, 0});

  videoDriver.onChange = [&] { config->video.driver = videoDriver.text(); };
  audioDriver.onChange = [&] { config->audio.driver = audioDriver.text(); };
  inputDriver.onChange = [&] { config->input.driver = inputDriver.text(); };

  libraryBrowse.onActivate = [&] {
    string path = DialogWindow::folderSelect(Window::none(), userpath());
    if(path.empty()) return;
    file::write({configpath(), "higan/library.cfg"}, path);
    libraryPath.setText(path);
  };
}
