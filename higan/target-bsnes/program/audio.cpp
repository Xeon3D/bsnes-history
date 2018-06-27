auto Program::updateAudioDriver() -> void {
  audio = Audio::create(settings["Audio/Driver"].text());
  audio->setContext(presentation->viewport.handle());
  audio->setChannels(2);
  updateAudioExclusive();
  updateAudioDevice();
  updateAudioBlocking();

  settingsWindow->advanced.updateAudioDriver();

  if(!audio->ready()) {
    MessageDialog({
      "Error: failed to initialize [", settings["Audio/Driver"].text(), "] audio driver."
    }).error();
    settings["Audio/Driver"].setValue("None");
    return updateAudioDriver();
  }
}

auto Program::updateAudioExclusive() -> void {
  if(!audio) return;
  audio->clear();
  audio->setExclusive(settings["Audio/Exclusive"].boolean());
  updateAudioFrequency();
  updateAudioLatency();
}

auto Program::updateAudioDevice() -> void {
  if(!audio) return;
  audio->clear();
  if(!audio->availableDevices().find(settings["Audio/Device"].text())) {
    settings["Audio/Device"].setValue(audio->availableDevices()(0));
  }
  audio->setDevice(settings["Audio/Device"].text());
  updateAudioFrequency();
  updateAudioLatency();
  settingsWindow->audio.updateDevice();
}

auto Program::updateAudioBlocking() -> void {
  audio->setBlocking(settings["Audio/Blocking"].boolean());
}

auto Program::updateAudioFrequency() -> void {
  if(!audio) return;
  audio->clear();
  if(!audio->availableFrequencies().find(settings["Audio/Frequency"].real())) {
    settings["Audio/Frequency"].setValue(audio->availableFrequencies()(0));
  }
  audio->setFrequency(settings["Audio/Frequency"].real());
  Emulator::audio.setFrequency(settings["Audio/Frequency"].real() + settings["Audio/Skew"].integer());
  settingsWindow->audio.updateFrequency();
}

auto Program::updateAudioLatency() -> void {
  if(!audio) return;
  audio->clear();
  if(!audio->availableLatencies().find(settings["Audio/Latency"].natural())) {
    settings["Audio/Latency"].setValue(audio->availableLatencies()(0));
  }
  audio->setLatency(settings["Audio/Latency"].natural());
  settingsWindow->audio.updateLatency();
}

auto Program::updateAudioEffects() -> void {
  double volume = settings["Audio/Mute"].boolean() ? 0.0 : settings["Audio/Volume"].natural() * 0.01;
  Emulator::audio.setVolume(volume);

  double balance = max(-1.0, min(+1.0, (settings["Audio/Balance"].integer() - 50) / 50.0));
  Emulator::audio.setBalance(balance);

  bool reverb = settings["Audio/Reverb"].boolean();
  Emulator::audio.setReverb(reverb);
}
