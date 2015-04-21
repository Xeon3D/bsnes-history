#include "../tomoko.hpp"
Presentation* presentation = nullptr;

Presentation::Presentation() {
  presentation = this;

  libraryMenu.setText("Library");
  for(auto& emulator : program->emulators) {
    for(auto& media : emulator->media) {
      if(media.bootable == false) continue;
      auto item = new MenuItem{&libraryMenu};
      item->setText({media.name, " ..."}).onActivate([=] {
        libraryManager->show(media.type);
      });
      loadBootableMedia.append(item);
    }
  }

  systemMenu.setText("System").setVisible(false);
  powerSystem.setText("Power").onActivate([&] { program->powerCycle(); });
  resetSystem.setText("Reset").onActivate([&] { program->softReset(); });
  unloadSystem.setText("Unload").onActivate([&] { program->unloadMedia(); drawSplashScreen(); });

  settingsMenu.setText("Settings");
  videoScaleMenu.setText("Video Scale");
  MenuRadioItem::group({videoScaleSmall, videoScaleNormal, videoScaleLarge});
  if(config().video.scale == "Small") videoScaleSmall.setChecked();
  if(config().video.scale == "Normal") videoScaleNormal.setChecked();
  if(config().video.scale == "Large") videoScaleLarge.setChecked();
  videoScaleSmall.setText("Small").onActivate([&] {
    config().video.scale = "Small";
    resizeViewport();
  });
  videoScaleNormal.setText("Normal").onActivate([&] {
    config().video.scale = "Normal";
    resizeViewport();
  });
  videoScaleLarge.setText("Large").onActivate([&] {
    config().video.scale = "Large";
    resizeViewport();
  });
  aspectCorrection.setText("Aspect Correction").setChecked(config().video.aspectCorrection).onToggle([&] {
    config().video.aspectCorrection = aspectCorrection.checked();
    resizeViewport();
  });
  videoFilterMenu.setText("Video Filter");
  MenuRadioItem::group({videoFilterNone, videoFilterBlur});
  if(config().video.filter == "None") videoFilterNone.setChecked();
  if(config().video.filter == "Blur") videoFilterBlur.setChecked();
  videoFilterNone.setText("None").onActivate([&] { config().video.filter = "None"; program->updateVideoFilter(); });
  videoFilterBlur.setText("Blur").onActivate([&] { config().video.filter = "Blur"; program->updateVideoFilter(); });
  colorEmulation.setText("Color Emulation").setChecked(config().video.colorEmulation).onToggle([&] {
    config().video.colorEmulation = colorEmulation.checked();
    program->updateVideoPalette();
  });
  maskOverscan.setText("Mask Overscan").setChecked(config().video.overscan.mask).onToggle([&] {
    config().video.overscan.mask = maskOverscan.checked();
  });
  synchronizeVideo.setText("Synchronize Video").setChecked(config().video.synchronize).onToggle([&] {
    config().video.synchronize = synchronizeVideo.checked();
    video.set(Video::Synchronize, config().video.synchronize);
  });
  synchronizeAudio.setText("Synchronize Audio").setChecked(config().audio.synchronize).onToggle([&] {
    config().audio.synchronize = synchronizeAudio.checked();
    audio.set(Audio::Synchronize, config().audio.synchronize);
  });
  muteAudio.setText("Mute Audio").setChecked(config().audio.mute).onToggle([&] {
    config().audio.mute = muteAudio.checked();
    program->dsp.setVolume(config().audio.mute ? 0.0 : 1.0);
  });
  showStatusBar.setText("Show Status Bar").setChecked(config().userInterface.showStatusBar).onToggle([&] {
    config().userInterface.showStatusBar = showStatusBar.checked();
    statusBar.setVisible(config().userInterface.showStatusBar);
    if(visible()) resizeViewport();
  });
  showConfiguration.setText("Configuration ...").onActivate([&] { settingsManager->show(0); });

  toolsMenu.setText("Tools").setVisible(false);
  saveStateMenu.setText("Save State");
  saveSlot1.setText("Slot 1").onActivate([&] { program->saveState(1); });
  saveSlot2.setText("Slot 2").onActivate([&] { program->saveState(2); });
  saveSlot3.setText("Slot 3").onActivate([&] { program->saveState(3); });
  saveSlot4.setText("Slot 4").onActivate([&] { program->saveState(4); });
  saveSlot5.setText("Slot 5").onActivate([&] { program->saveState(5); });
  loadStateMenu.setText("Load State");
  loadSlot1.setText("Slot 1").onActivate([&] { program->loadState(1); });
  loadSlot2.setText("Slot 2").onActivate([&] { program->loadState(2); });
  loadSlot3.setText("Slot 3").onActivate([&] { program->loadState(3); });
  loadSlot4.setText("Slot 4").onActivate([&] { program->loadState(4); });
  loadSlot5.setText("Slot 5").onActivate([&] { program->loadState(5); });
  cheatEditor.setText("Cheat Editor").onActivate([&] { toolsManager->show(0); });
  stateManager.setText("State Manager").onActivate([&] { toolsManager->show(1); });

  statusBar.setFont(Font::sans(8, "Bold"));
  statusBar.setVisible(config().userInterface.showStatusBar);

  onClose([&] { program->quit(); });

  setTitle({"tomoko v", Emulator::Version});
  setResizable(false);
  setBackgroundColor({16, 16, 16});
  resizeViewport();
}

auto Presentation::resizeViewport() -> void {
  signed width  = 256;
  signed height = 240;

  if(emulator) {
    width  = emulator->information.width;
    height = emulator->information.height;
  }

  if(fullScreen() == false) {
    bool arc = config().video.aspectCorrection && emulator && emulator->information.aspectRatio != 1.0;

    if(config().video.scale == "Small" ) width *= 1, height *= 1;
    if(config().video.scale == "Normal") width *= 2, height *= 2;
    if(config().video.scale == "Large" ) width *= 4, height *= 4;
    if(arc) width = width * 8 / 7;

    setSize({width, height});
    viewport.setGeometry({0, 0, width, height});
    setPlacement(0.5, 0.5);
  } else {
    auto desktop = Desktop::size();

    //aspect ratio correction is always enabled in fullscreen mode
    //note that below algorithm yields 7:6 ratio on 2560x(1440,1600) monitors
    //this is extremely close to the optimum 8:7 ratio
    //it is used so that linear interpolation isn't required
    //todo: we should handle other resolutions nicely as well
    unsigned multiplier = desktop.height() / height;
    width *= 1 + multiplier;
    height *= multiplier;

    signed x  = (desktop.width() - width) / 2;
    signed y  = (desktop.height() - height) / 2;

    if(x < 0) x = 0;
    if(y < 0) y = 0;
    if(width > desktop.width()) width = desktop.width();
    if(height > desktop.height()) height = desktop.height();

    viewport.setGeometry({x, y, width, height});
  }

  if(!emulator) drawSplashScreen();
}

auto Presentation::toggleFullScreen() -> void {
  if(fullScreen() == false) {
    menuBar.setVisible(false);
    statusBar.setVisible(false);
    setResizable(true);
    setFullScreen(true);
  } else {
    setFullScreen(false);
    setResizable(false);
    menuBar.setVisible(true);
    statusBar.setVisible(config().userInterface.showStatusBar);
  }

  Application::processEvents();
  resizeViewport();
}

auto Presentation::drawSplashScreen() -> void {
  uint32* output;
  unsigned length;
  if(video.lock(output, length, 256, 240)) {
    for(auto y : range(240)) {
      uint32* dp = output + y * (length >> 2);
      for(auto x : range(256)) *dp++ = 0xff000000;
    }
    video.unlock();
    video.refresh();
  }
}
