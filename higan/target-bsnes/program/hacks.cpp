auto Program::hackCompatibility() -> void {
  bool fastPPU = settingsWindow->advanced.fastPPUOption.checked();
  bool fastPPUNoSpriteLimit = settingsWindow->advanced.noSpriteLimit.checked();
  bool fastPPUHiresMode7 = settingsWindow->advanced.hiresMode7.checked();
  bool fastDSP = settingsWindow->advanced.fastDSPOption.checked();

  auto label = superFamicom.label;
  if(label == "AIR STRIKE PATROL" || label == "DESERT FIGHTER") fastPPU = false;
  if(label == "KOUSHIEN_2") fastDSP = false;
  if(label == "RENDERING RANGER R2") fastDSP = false;

  emulator->set("Fast PPU", fastPPU);
  emulator->set("Fast PPU/No Sprite Limit", fastPPUNoSpriteLimit);
  emulator->set("Fast PPU/Hires Mode 7", fastPPUHiresMode7);
  emulator->set("Fast DSP", fastDSP);
}

auto Program::hackPatchMemory(vector<uint8_t>& data) -> void {
  auto label = superFamicom.label;

  if(label == "Satellaview BS-X" && data.size() >= 0x100000) {
    //BS-X: Sore wa Namae o Nusumareta Machi no Monogatari (JPN) (1.1)
    //disable limited play check for BS Memory flash cartridges
    //benefit: allow locked out BS Memory flash games to play without manual header patching
    //detriment: BS Memory ROM cartridges will cause the game to hang in the load menu
    if(data[0x4a9b] == 0x10) data[0x4a9b] = 0x80;
    if(data[0x4d6d] == 0x10) data[0x4d6d] = 0x80;
    if(data[0x4ded] == 0x10) data[0x4ded] = 0x80;
    if(data[0x4e9a] == 0x10) data[0x4e9a] = 0x80;
  }
}

auto Program::hackOverclockSuperFX() -> void {
  //todo: implement a better way of detecting SuperFX games
  //todo: apply multiplier changes on reset, not just on game load?
  double multiplier = settingsWindow->advanced.superFXValue.text().natural() / 100.0;
  if(multiplier == 1.0) return;

  auto label = superFamicom.label;
  if(label == "NIDAN MORITASHOGI2") return;  //ST018 uses same clock speed as SuperFX

  auto document = BML::unserialize(superFamicom.manifest);

  //GSU-1, GSU-2 have a 21440000hz oscillator
  if(auto oscillator = document["game/board/oscillator"]) {
    if(oscillator["frequency"].text() == "21440000") {
      oscillator["frequency"].setValue(uint(21440000 * multiplier));
      superFamicom.manifest = BML::serialize(document);
    }
    return;
  }

  //MARIO CHIP 1 uses CPU oscillator; force it to use its own crystal to overclock it
  bool marioChip1 = false;
  if(label == "STAR FOX" || label == "STAR WING") marioChip1 = true;
  if(marioChip1) {
    document("game/board/oscillator/frequency").setValue(uint(21440000 * multiplier));
    superFamicom.manifest = BML::serialize(document);
  }
}
