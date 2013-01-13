Browser *browser = nullptr;

Browser::Browser() {
  bootstrap();
  setGeometry({128, 128, 640, 400});
  windowManager->append(this, "Browser");

  layout.setMargin(5);
  homeButton.setImage({resource::home, sizeof resource::home});
  upButton.setImage({resource::up, sizeof resource::up});
  openButton.setText("Open");

  append(layout);
  layout.append(pathLayout, {~0, 0}, 5);
    pathLayout.append(pathEdit, {~0, 0}, 5);
    pathLayout.append(homeButton, {28, 28}, 5);
    pathLayout.append(upButton, {28, 28});
  layout.append(fileList, {~0, ~0}, 5);
  layout.append(controlLayout, {~0, 0});
    controlLayout.append(filterLabel, {~0, 0}, 5);
    controlLayout.append(openButton, {80, 0});

  pathEdit.onActivate = [&] {
    string path = pathEdit.text();
    path.transform("\\", "/");
    if(path.endswith("/") == false) path.append("/");
    setPath(path);
  };

  homeButton.onActivate = [&] {
    setPath({userpath(), "Emulation/"});
  };

  upButton.onActivate = [&] {
    setPath(parentdir(path));
  };

  fileList.onChange = {&Browser::synchronize, this};
  fileList.onActivate = openButton.onActivate = {&Browser::fileListActivate, this};
  onClose = [&] { dialogActive = false; };

  synchronize();
}

void Browser::synchronize() {
  openButton.setEnabled(fileList.selected());
  if(fileList.selected()) {
    for(auto &folder : folderList) {
      if(folder.extension == extension) {
        folder.selection = fileList.selection();
      }
    }
  }
}

void Browser::saveConfiguration() {
  config.save(application->path("paths.cfg"));
}

void Browser::bootstrap() {
  for(auto &emulator : application->emulator) {
    for(auto &media : emulator->media) {
      bool found = false;
      for(auto &folder : folderList) {
        if(folder.extension == media.type) {
          found = true;
          break;
        }
      }
      if(found == true) continue;

      Folder folder;
      folder.extension = media.type;
      folder.path = {userpath(), "Emulation/", media.name, "/"};
      folder.selection = 0;
      folderList.append(folder);
    }
  }

  for(auto &folder : folderList) {
    config.append(folder.path, folder.extension);
    config.append(folder.selection, string{folder.extension, "::selection"});
  }

  config.load(application->path("paths.cfg"));
  config.save(application->path("paths.cfg"));
}

string Browser::select(const string &title, const string &extension) {
  this->extension = extension;

  string path;
  unsigned selection = 0;
  for(auto &folder : folderList) {
    if(folder.extension == extension) {
      path = folder.path;
      selection = folder.selection;
      break;
    }
  }
  if(path.empty()) path = application->basepath;
  setPath(path, selection);

  filterLabel.setText({"Filter: *.", extension});

  audio.clear();
  setTitle(title);
  setModal(true);
  setVisible(true);
  fileList.setFocused();
  outputFilename = "";

  dialogActive = true;
  bool backspace = false;
  using phoenix::Keyboard;

  while(dialogActive) {
    OS::processEvents();
    if(Keyboard::pressed(Keyboard::Scancode::Escape)) onClose();
    if(Keyboard::pressed(Keyboard::Scancode::Backspace)) {
      if(backspace == false) {
        backspace = true;
        if(fileList.focused()) upButton.onActivate();
      }
    } else {
      backspace = false;
    }
    usleep(20 * 1000);
  }

  setModal(false);
  setVisible(false);
  return outputFilename;
}

void Browser::setPath(const string &path, unsigned selection) {
  //save path for next browser selection
  for(auto &folder : folderList) {
    if(folder.extension == extension) folder.path = path;
  }

  this->path = path;
  pathEdit.setText(path);

  fileList.reset();
  filenameList.reset();

  lstring contents = directory::ifolders(path);

  for(auto &filename : contents) {
    string suffix = {".", this->extension, "/"};
    if(filename.endswith("/") && !filename.endswith(suffix)) {
      string name = filename;
      name.rtrim<1>("/");
      fileList.append(name);
      fileList.setImage(filenameList.size(), 0, {resource::folder, sizeof resource::folder});
      filenameList.append(filename);
    }
  }

  for(auto &filename : contents) {
    string suffix = {".", this->extension, "/"};
    if(filename.endswith(suffix)) {
      string name = filename;
      name.rtrim<1>(suffix);
      fileList.append(name);
      fileList.setImage(filenameList.size(), 0, {resource::game, sizeof resource::game});
      filenameList.append(filename);
    }
  }

  fileList.setSelection(selection);
  fileList.setFocused();
  synchronize();
}

void Browser::fileListActivate() {
  unsigned selection = fileList.selection();
  string filename = filenameList[selection];
  if(string{filename}.rtrim<1>("/").endswith(this->extension) == false) return setPath({path, filename});

  dialogActive = false;
  outputFilename = {path, filename};
}
