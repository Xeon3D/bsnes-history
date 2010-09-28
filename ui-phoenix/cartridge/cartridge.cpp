#include "../base.hpp"
Cartridge cartridge;

bool Cartridge::loadNormal(const char *basename) {
  unload();
  if(loadCartridge(SNES::memory::cartrom, baseXML, basename) == false) return false;
  baseName = nall::basename(basename);
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, lstring() << baseXML);
  loadMemory(SNES::memory::cartram, baseName, ".srm");
  loadMemory(SNES::memory::cartrtc, baseName, ".rtc");
  utility.cartridgeLoaded();
  return true;
}

bool Cartridge::loadBsxSlotted(const char *basename, const char *slotname) {
  unload();
  if(loadCartridge(SNES::memory::cartrom, baseXML, basename) == false) return false;
  loadCartridge(SNES::memory::bsxflash, slotAXML, slotname);
  baseName = nall::basename(basename);
  slotAName = nall::basename(slotname);
  SNES::cartridge.load(SNES::Cartridge::Mode::BsxSlotted, lstring() << baseXML << slotAXML);
  loadMemory(SNES::memory::cartram, baseName, ".srm");
  loadMemory(SNES::memory::cartrtc, baseName, ".rtc");
  utility.cartridgeLoaded();
  return true;
}

bool Cartridge::loadBsx(const char *basename, const char *slotname) {
  unload();
  if(loadCartridge(SNES::memory::cartrom, baseXML, basename) == false) return false;
  loadCartridge(SNES::memory::bsxflash, slotAXML, slotname);
  baseName = nall::basename(basename);
  slotAName = nall::basename(slotname);
  SNES::cartridge.load(SNES::Cartridge::Mode::Bsx, lstring() << baseXML << slotAXML);
  loadMemory(SNES::memory::bsxram, baseName, ".srm");
  loadMemory(SNES::memory::bsxpram, baseName, ".psr");
  utility.cartridgeLoaded();
  return true;
}

bool Cartridge::loadSufamiTurbo(const char *basename, const char *slotAname, const char *slotBname) {
  unload();
  if(loadCartridge(SNES::memory::cartrom, baseXML, basename) == false) return false;
  loadCartridge(SNES::memory::stArom, slotAXML, slotAname);
  loadCartridge(SNES::memory::stBrom, slotBXML, slotBname);
  baseName = nall::basename(basename);
  slotAName = nall::basename(slotAname);
  slotBName = nall::basename(slotBname);
  SNES::cartridge.load(SNES::Cartridge::Mode::SufamiTurbo, lstring() << baseXML << slotAXML << slotBXML);
  loadMemory(SNES::memory::stAram, slotAName, ".srm");
  loadMemory(SNES::memory::stBram, slotBName, ".srm");
  utility.cartridgeLoaded();
  return true;
}

bool Cartridge::loadSuperGameBoy(const char *basename, const char *slotname) {
  unload();
  if(loadCartridge(SNES::memory::cartrom, baseXML, basename) == false) return false;
  loadCartridge(SNES::memory::gbrom, slotAXML, slotname);
  baseName = nall::basename(basename);
  slotAName = nall::basename(slotname);
  SNES::cartridge.load(SNES::Cartridge::Mode::SuperGameBoy, lstring() << baseXML << slotAXML);
  loadMemory(SNES::memory::gbram, slotAName, ".sav");
  loadMemory(SNES::memory::gbrtc, slotAName, ".rtc");
  utility.cartridgeLoaded();
  return true;
}

void Cartridge::unload() {
  if(SNES::cartridge.loaded() == false) return;
  saveMemory(SNES::memory::cartram, baseName, ".srm");
  saveMemory(SNES::memory::cartrtc, baseName, ".rtc");
  saveMemory(SNES::memory::bsxram, baseName, ".srm");
  saveMemory(SNES::memory::bsxpram, baseName, ".psr");
  saveMemory(SNES::memory::stAram, slotAName, ".srm");
  saveMemory(SNES::memory::stBram, slotBName, ".srm");
  saveMemory(SNES::memory::gbram, slotAName, ".sav");
  saveMemory(SNES::memory::gbrtc, slotAName, ".rtc");
  baseName = slotAName = slotBName = "";
  utility.cartridgeUnloaded();
}

bool Cartridge::loadCartridge(SNES::MappedRAM &memory, string &XML, const char *filename) {
  if(file::exists(filename) == false) return false;
  file fp;
  if(fp.open(filename, file::mode_read) == false) return false;

  unsigned size = fp.size();
  if((size & 0x7fff) == 512) {
    size -= 512;
    fp.seek(512);
  }
  uint8_t *data = new uint8_t[size];
  fp.read(data, size);
  fp.close();

  memory.copy(data, size);
  XML = snes_information(data, size).xml_memory_map;
  delete[] data;
  return true;
}

bool Cartridge::loadMemory(SNES::MappedRAM &memory, string filename, const char *extension) {
  if(memory.size() == 0 || memory.size() == ~0) return true;
  filename = string(filename, extension);
  if(file::exists(filename) == false) return false;
  file fp;
  if(fp.open(filename, file::mode_read)) {
    fp.read(memory.data(), min(memory.size(), fp.size()));
    fp.close();
  }
  return true;
}

bool Cartridge::saveMemory(SNES::MappedRAM &memory, string filename, const char *extension) {
  if(memory.size() == 0 || memory.size() == ~0) return true;
  filename = string(filename, extension);
  file fp;
  if(fp.open(filename, file::mode_write) == false) return false;
  fp.write(memory.data(), memory.size());
  fp.close();
  return true;
}
