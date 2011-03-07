#ifdef MSU1_CPP

void MSU1::serialize(serializer &s) {
  Processor::serialize(s);

  s.integer(mmio.data_offset);
  s.integer(mmio.audio_offset);
  s.integer(mmio.audio_loop_offset);

  s.integer(mmio.audio_track);
  s.integer(mmio.audio_volume);

  s.integer(mmio.data_busy);
  s.integer(mmio.audio_busy);
  s.integer(mmio.audio_repeat);
  s.integer(mmio.audio_play);

  if(datafile.open()) datafile.close();
  if(datafile.open(system.interface->path(Cartridge::Slot::MSU1, ".msu"), file::mode::read)) {
    datafile.seek(mmio.data_offset);
  }

  if(audiofile.open()) audiofile.close();
  if(audiofile.open(system.interface->path(Cartridge::Slot::MSU1, { "-", (unsigned)mmio.audio_track, ".pcm" }), file::mode::read)) {
    audiofile.seek(mmio.audio_offset);
  }
}

#endif
