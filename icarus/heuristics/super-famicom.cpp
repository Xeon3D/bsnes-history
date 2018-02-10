namespace Heuristics {

struct SuperFamicom {
  SuperFamicom(const uint8_t* data, uint size, bool hasMSU1 = false);
  explicit operator bool() const;

  auto manifest() const -> string;
  auto memory(string type, uint size, string name) const -> string;
  auto sha256() const -> string;
  auto region() const -> string;
  auto revision() const -> string;
  auto board() const -> string;
  auto label() const -> string;
  auto serial() const -> string;
  auto romSize() const -> uint;
  auto ramSize() const -> uint;
  auto expansionRamSize() const -> uint;

private:
  auto scoreHeader(uint address) -> uint;

  const uint8_t* data = nullptr;
  uint size = 0;
  uint headerAddress = 0;
  bool hasMSU1 = false;
};

SuperFamicom::SuperFamicom(const uint8_t* data, uint size, bool hasMSU1) : data(data), size(size), hasMSU1(hasMSU1) {
  if((size & 0x7fff) == 512) data += 512, size -= 512;  //skip copier header (if present)
  if(size < 0x8000) return;  //ignore images too small to be valid

  uint scoreLo = scoreHeader(  0x7fb0);
  uint scoreHi = scoreHeader(  0xffb0);
  uint scoreEx = scoreHeader(0x40ffb0);
  if(scoreEx) scoreEx += 4;

  if(scoreLo >= scoreHi && scoreLo >= scoreEx) headerAddress = 0x7fb0;
  else if(scoreHi >= scoreEx) headerAddress = 0xffb0;
  else headerAddress = 0x40ffb0;
}

SuperFamicom::operator bool() const {
  return headerAddress;
}

auto SuperFamicom::manifest() const -> string {
  if(!operator bool()) return "error\n";

  string output;
  output.append("game\n");
  output.append("  sha256:   ", sha256(), "\n");
  output.append("  region:   ", region(), "\n");
  output.append("  revision: ", revision(), "\n");
  output.append("  board:    ", board(), "\n");
  output.append("  label:    ", label(), "\n");

  auto board = this->board().split("-");

         if(board.left() == "CX4") {
    output.append(memory("ROM", size - 0xc00, "program.rom"));
    output.append(memory("ROM", 0xc00, "cx4.data.rom"));
  } else if(board.left() == "DSP") {
    output.append(memory("ROM", size - 0x2000, "program.rom"));
    output.append(memory("ROM", 0x1800, "dsp.program.rom"));
    output.append(memory("ROM", 0x800, "dsp.data.rom"));
  } else if(board.left() == "SGB") {
    output.append(memory("ROM", size - 0x100, "program.rom"));
    output.append(memory("ROM", 0x100, "sgb.boot.rom"));
  } else if(board.left() == "SPC7110") {
    output.append(memory("ROM", 0x100000, "program.rom"));
    output.append(memory("ROM", size - 0x100000, "data.rom"));
  } else if(board.left() == "ST010" || board.left() == "ST011") {
    output.append(memory("ROM", size - 0xd000, "program.rom"));
    output.append(memory("ROM", 0xc000, "st.program.rom"));
    output.append(memory("ROM", 0x1000, "st.data.rom"));
  } else if(board.left() == "ST018") {
    output.append(memory("ROM", size - 0x28000, "program.rom"));
    output.append(memory("ROM", 0x20000, "seta.program.rom"));
    output.append(memory("ROM", 0x8000, "seta.data.rom"));
  } else {
    output.append(memory("ROM", size, "program.rom"));
  }

  if(auto size = ramSize()) {
    auto type = board.right() == "NVRAM" ? "NVRAM" : "RAM";
    output.append(memory(type, size, "save.ram"));
  }

  if(auto size = expansionRamSize()) {
    auto type = board.right() == "NVRAM" ? "NVRAM" : "RAM";
    output.append(memory(type, size, "expansion.ram"));
  }

         if(board.left() == "BS" && board(1) == "NVRAM") {
    output.append(memory("NVRAM", 0x80000, "download.ram"));
  } else if(board.left() == "CX4") {
    output.append(memory("RAM", 0xc00, "cx4.data.ram"));
  } else if(board.left() == "DSP") {
    output.append(memory("RAM", 0x200, "dsp.data.ram"));
  } else if(board.left() == "RTC") {
    output.append(memory("NVRAM", 0x10, "rtc.ram"));
  } else if(board.left() == "SA1") {
    output.append(memory("RAM", 0x800, "internal.ram"));
  } else if(board.left() == "SPC7110" && board(1) == "RTC") {
    output.append(memory("NVRAM", 0x10, "rtc.ram"));
  } else if(board.left() == "ST010" || board.left() == "ST011") {
    output.append(memory("NVRAM", 0x1000, "st.save.ram"));
  } else if(board.left() == "ST018") {
    output.append(memory("NVRAM", 0x4000, "seta.save.ram"));
  }

  if(hasMSU1) {
    output.append("  msu1\n");
  }

  return output;
}

auto SuperFamicom::memory(string type, uint size, string name) const -> string {
  string output;
  output.append("  memory\n");
  output.append("    type: ", type, "\n");
  output.append("    size: 0x", hex(size), "\n");
  output.append("    name: ", name, "\n");
  return output;
}

auto SuperFamicom::sha256() const -> string {
  return Hash::SHA256(data, size).digest();
}

auto SuperFamicom::region() const -> string {
  string region;

  char A = data[headerAddress + 0x02];  //game type
  char B = data[headerAddress + 0x03];  //game code
  char C = data[headerAddress + 0x04];  //game code
  char D = data[headerAddress + 0x05];  //region code (new; sometimes ambiguous)
  auto E = data[headerAddress + 0x29];  //region code (old)

  auto valid = [](char n) { return (n >= '0' && n <= '9') || (n >= 'A' && n <= 'Z'); };
  if(data[headerAddress + 0x2a] == 0x33 && valid(A) && valid(B) & valid(C) & valid(D)) {
    string code{A, B, C, D};
    if(D == 'B') region = {"SNS-",  code, "-BRA"};
    if(D == 'C') region = {"SNSN-", code, "-ROC"};
    if(D == 'D') region = {"SNSP-", code, "-NOE"};
    if(D == 'E') region = {"SNS-",  code, "-USA"};
    if(D == 'F') region = {"SNSP-", code, "-FRA"};
    if(D == 'H') region = {"SNSP-", code, "-HOL"};
    if(D == 'I') region = {"SNSP-", code, "-ITA"};
    if(D == 'J') region = {"SHVC-", code, "-JPN"};
    if(D == 'K') region = {"SNSN-", code, "-KOR"};
    if(D == 'N') region = {"SNS-",  code, "-CAN"};
    if(D == 'P') region = {"SNSP-", code, "-EUR"};
    if(D == 'S') region = {"SNSP-", code, "-ESP"};
    if(D == 'U') region = {"SNSP-", code, "-AUS"};
    if(D == 'W') region = {"SNSP-", code, "-SCN"};
  }

  if(!region) {
    if(E == 0x00) region = {"SHVC-JPN"};
    if(E == 0x01) region = { "SNS-USA"};
    if(E == 0x02) region = {"SNSP-EUR"};
    if(E == 0x03) region = {"SNSP-SCN"};
    if(E == 0x06) region = {"SNSP-FRA"};
    if(E == 0x07) region = {"SNSP-HOL"};
    if(E == 0x08) region = {"SNSP-ESP"};
    if(E == 0x09) region = {"SNSP-NOE"};
    if(E == 0x0a) region = {"SNSP-ITA"};
    if(E == 0x0b) region = {"SNSP-ROC"};
    if(E == 0x0d) region = {"SNSP-KOR"};
    if(E == 0x0f) region = { "SNS-CAN"};
    if(E == 0x10) region = { "SNS-BRA"};
    if(E == 0x11) region = {"SNSP-AUS"};
  }

  return region ? region : "NTSC";
}

auto SuperFamicom::revision() const -> string {
  string revision;

  char A = data[headerAddress + 0x02];  //game type
  char B = data[headerAddress + 0x03];  //game code
  char C = data[headerAddress + 0x04];  //game code
  char D = data[headerAddress + 0x05];  //region code (new; sometimes ambiguous)
  auto E = data[headerAddress + 0x29];  //region code (old)
  uint F = data[headerAddress + 0x2b];  //revision code

  auto valid = [](char n) { return (n >= '0' && n <= '9') || (n >= 'A' && n <= 'Z'); };
  if(data[headerAddress + 0x2a] == 0x33 && valid(A) && valid(B) & valid(C) & valid(D)) {
    string code{A, B, C, D};
    if(D == 'B') revision = {"SNS-",  code, "-", F};
    if(D == 'C') revision = {"SNSN-", code, "-", F};
    if(D == 'D') revision = {"SNSP-", code, "-", F};
    if(D == 'E') revision = {"SNS-",  code, "-", F};
    if(D == 'F') revision = {"SNSP-", code, "-", F};
    if(D == 'H') revision = {"SNSP-", code, "-", F};
    if(D == 'I') revision = {"SNSP-", code, "-", F};
    if(D == 'J') revision = {"SHVC-", code, "-", F};
    if(D == 'K') revision = {"SNSN-", code, "-", F};
    if(D == 'N') revision = {"SNS-",  code, "-", F};
    if(D == 'P') revision = {"SNSP-", code, "-", F};
    if(D == 'S') revision = {"SNSP-", code, "-", F};
    if(D == 'U') revision = {"SNSP-", code, "-", F};
    if(D == 'W') revision = {"SNSP-", code, "-", F};
  }

  if(!revision) {
    if(E == 0x00) revision = {"SHVC-", F};
    if(E == 0x01) revision = { "SNS-", F};
    if(E == 0x02) revision = {"SNSP-", F};
    if(E == 0x03) revision = {"SSWE-", F};
    if(E == 0x06) revision = {"SFRA-", F};
    if(E == 0x07) revision = {"SHOL-", F};
    if(E == 0x08) revision = {"SESP-", F};
    if(E == 0x09) revision = {"SFRG-", F};
    if(E == 0x0a) revision = {"SITA-", F};
    if(E == 0x0b) revision = {"SSCN-", F};
    if(E == 0x0d) revision = {"SKOR-", F};
    if(E == 0x0f) revision = { "SNS-", F};
    if(E == 0x10) revision = {"SBRA-", F};
    if(E == 0x11) revision = {"SNSP-", F};
  }

  return revision ? revision : string{"1.", F};
}

auto SuperFamicom::board() const -> string {
  auto mapMode = data[headerAddress + 0x25];
  auto cartridgeType = data[headerAddress + 0x26];
  auto cartridgeTypeLo = cartridgeType & 15;
  auto cartridgeTypeHi = cartridgeType >> 4;
  auto cartridgeSubType = data[headerAddress + 0x0f];

  string board;

  string mode;
  if(mapMode == 0x20 || mapMode == 0x30) mode = "LOROM-";
  if(mapMode == 0x21 || mapMode == 0x31) mode = "HIROM-";
  if(mapMode == 0x22 || mapMode == 0x32) mode = "SDD1-";
  if(mapMode == 0x23 || mapMode == 0x33) mode = "SA1-";
  if(mapMode == 0x25 || mapMode == 0x35) mode = "EXHIROM-";
  if(mapMode == 0x2a || mapMode == 0x3a) mode = "SPC7110-";

  //many games will store an extra title character, overwriting the map mode
  if(!mode) {
    if(headerAddress ==   0x7fb0) mode = "LOROM-";
    if(headerAddress ==   0xffb0) mode = "HIROM-";
    if(headerAddress == 0x40ffb0) mode = "EXHIROM-";
  }

         if(serial() == "A9PJ") {
  //Sufami Turbo (JPN)
    board.append("ST-", mode);
  } else if(serial() == "ZSBJ") {
  //BS-X: Sore wa Namae o Nusumareta Machi no Monogatari (JPN)
    board.append("BS-");
  } else if(serial().match("Z\?\?J")) {
    board.append("BS-", mode);
  } else if(cartridgeTypeLo >= 0x3) {
    if(cartridgeTypeHi == 0x0) board.append("DSP-", mode);
    if(cartridgeTypeHi == 0x1) board.append("SUPERFX-");
    if(cartridgeTypeHi == 0x2) board.append("OBC1-", mode);
    if(cartridgeTypeHi == 0x3) board.append("SA1-");
    if(cartridgeTypeHi == 0x4) board.append("SDD1-");
    if(cartridgeTypeHi == 0x5) board.append("RTC-", mode);
    if(cartridgeTypeHi == 0xe && cartridgeTypeLo == 0x3) board.append("SGB-");
    if(cartridgeTypeHi == 0xf && cartridgeSubType == 0x00 && cartridgeTypeLo == 0x5) board.append("SPC7110-");
    if(cartridgeTypeHi == 0xf && cartridgeSubType == 0x00 && cartridgeTypeLo == 0x9) board.append("SPC7110-RTC-");
    if(cartridgeTypeHi == 0xf && cartridgeSubType == 0x01) board.append(romSize() == 0x100000 ? "ST010-" : "ST011-");
    if(cartridgeTypeHi == 0xf && cartridgeSubType == 0x02) board.append("ST018-", mode);
    if(cartridgeTypeHi == 0xf && cartridgeSubType == 0x10) board.append("CX4-", mode);
  }
  if(!board) board.append(mode);

  //grow ROM region and restrict RAM region for large LOROM boards ("EXLOROM")
  if(board.beginsWith("LOROM-") && romSize() > 0x380000 && ramSize()) board.prepend("EX");

  if(cartridgeTypeLo == 0x1 || cartridgeTypeLo == 0x4) board.append("RAM-");
  if(cartridgeTypeLo == 0x2 || cartridgeTypeLo == 0x5) board.append("NVRAM-");
  if(cartridgeTypeLo == 0x6) board.append("BATTERY-");

  return board.trimRight("-", 1L);
}

auto SuperFamicom::label() const -> string {
  string label;
  auto append = [&](char c) -> void {
    if(c >= 0x20 && c <= 0x7e) label.append(c);  //ASCII
    //todo: convert Shift-JIS half-width katakana to UTF-8 here
    else label.append("?");
  };
  for(uint n : range(0x15)) append(data[headerAddress + 0x10 + n]);
  return label.strip();
}

auto SuperFamicom::serial() const -> string {
  char A = data[headerAddress + 0x02];  //game type
  char B = data[headerAddress + 0x03];  //game code
  char C = data[headerAddress + 0x04];  //game code
  char D = data[headerAddress + 0x05];  //region code (new; sometimes ambiguous)

  auto valid = [](char n) { return (n >= '0' && n <= '9') || (n >= 'A' && n <= 'Z'); };
  if(data[headerAddress + 0x2a] == 0x33 && valid(A) && valid(B) & valid(C) & valid(D)) {
    return {A, B, C, D};
  }

  return "";
}

auto SuperFamicom::romSize() const -> uint {
  //subtract appended firmware size, if firmware is present
  if((size &  0x7fff) ==   0x100) return size -   0x100;
  if((size &  0x7fff) ==   0xc00) return size -   0xc00;
  if((size &  0x7fff) ==  0x2000) return size -  0x2000;
  if((size &  0xffff) ==  0xd000) return size -  0xd000;
  if((size & 0x3ffff) == 0x28000) return size - 0x28000;
  return size;

//auto romSize = data[headerAddress + 0x27] & 15;
//return 1024 << romSize;
}

auto SuperFamicom::ramSize() const -> uint {
  auto ramSize = data[headerAddress + 0x28] & 7;
  if(ramSize) return 1024 << ramSize;
  return 0;
}

auto SuperFamicom::expansionRamSize() const -> uint {
  if(data[headerAddress + 0x2a] != 0x33) return 0;
  auto ramSize = data[headerAddress + 0x0d] & 7;
  if(ramSize) return 1024 << ramSize;
  return 0;
}

auto SuperFamicom::scoreHeader(uint address) -> uint {
  int score = 0;
  if(size < address + 0x50) return score;

  uint8_t  mapMode     = data[address + 0x25] & ~0x10;  //ignore FastROM bit
  uint16_t complement  = data[address + 0x2c] << 0 | data[address + 0x2d] << 8;
  uint16_t checksum    = data[address + 0x2e] << 0 | data[address + 0x2f] << 8;
  uint16_t resetVector = data[address + 0x4c] << 0 | data[address + 0x4d] << 8;
  if(resetVector < 0x8000) return score;  //$00:0000-7fff is never ROM data

  uint8_t opcode = data[(address & ~0x7fff) | (resetVector & 0x7fff)];  //first instruction executed

  //most likely opcodes
  if(opcode == 0x78  //sei
  || opcode == 0x18  //clc (clc; xce)
  || opcode == 0x38  //sec (sec; xce)
  || opcode == 0x9c  //stz $nnnn (stz $4200)
  || opcode == 0x4c  //jmp $nnnn
  || opcode == 0x5c  //jml $nnnnnn
  ) score += 8;

  //plausible opcodes
  if(opcode == 0xc2  //rep #$nn
  || opcode == 0xe2  //sep #$nn
  || opcode == 0xad  //lda $nnnn
  || opcode == 0xae  //ldx $nnnn
  || opcode == 0xac  //ldy $nnnn
  || opcode == 0xaf  //lda $nnnnnn
  || opcode == 0xa9  //lda #$nn
  || opcode == 0xa2  //ldx #$nn
  || opcode == 0xa0  //ldy #$nn
  || opcode == 0x20  //jsr $nnnn
  || opcode == 0x22  //jsl $nnnnnn
  ) score += 4;

  //implausible opcodes
  if(opcode == 0x40  //rti
  || opcode == 0x60  //rts
  || opcode == 0x6b  //rtl
  || opcode == 0xcd  //cmp $nnnn
  || opcode == 0xec  //cpx $nnnn
  || opcode == 0xcc  //cpy $nnnn
  ) score -= 4;

  //least likely opcodes
  if(opcode == 0x00  //brk #$nn
  || opcode == 0x02  //cop #$nn
  || opcode == 0xdb  //stp
  || opcode == 0x42  //wdm
  || opcode == 0xff  //sbc $nnnnnn,x
  ) score -= 8;

  if(checksum + complement == 0xffff) score += 4;

  if(address == 0x7fb0 && mapMode == 0x20) score += 2;
  if(address == 0xffb0 && mapMode == 0x21) score += 2;

  return max(0, score);
}

}
