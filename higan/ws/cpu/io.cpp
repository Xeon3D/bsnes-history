auto CPU::portRead(uint16 addr) -> uint8 {
  //DMA_SRC
  if(addr == 0x0040) return r.dmaSource.b0;
  if(addr == 0x0041) return r.dmaSource.b1;
  if(addr == 0x0042) return r.dmaSource.b2;

  //DMA_DST
  if(addr == 0x0044) return r.dmaTarget.b0;
  if(addr == 0x0045) return r.dmaTarget.b1;

  //DMA_LEN
  if(addr == 0x0046) return r.dmaLength.b0;
  if(addr == 0x0047) return r.dmaLength.b1;

  //DMA_CTRL
  if(addr == 0x0048) return r.dmaEnable << 7 | r.dmaMode << 0;

  //WSC_SYSTEM
  if(addr == 0x0062) {
    return SC() << 7;
  }

  //HW_FLAGS
  if(addr == 0x00a0) {
    return (
      1 << 7      //1 = built-in self-test passed
    | 1 << 2      //0 = 8-bit bus width; 1 = 16-bit bus width
    | !WS() << 1  //0 = WonderSwan; 1 = WonderSwan Color or SwanCrystal
    | 1 << 0      //0 = BIOS mapped; 1 = cartridge mapped
    );
  }

  //INT_BASE
  if(addr == 0x00b0) {
    if(WS()) return r.interruptBase | 3;
    return r.interruptBase;
  }

  //INT_ENABLE
  if(addr == 0x00b2) return r.interruptEnable;

  //INT_STATUS
  if(addr == 0x00b4) return r.interruptStatus;

  return 0x00;
}

auto CPU::portWrite(uint16 addr, uint8 data) -> void {
  //DMA_SRC
  if(addr == 0x0040) { r.dmaSource.b0 = data & ~1; return; }
  if(addr == 0x0041) { r.dmaSource.b1 = data; return; }
  if(addr == 0x0042) { r.dmaSource.b2 = data; return; }

  //DMA_DST
  if(addr == 0x0044) { r.dmaTarget.b0 = data & ~1; return; }
  if(addr == 0x0045) { r.dmaTarget.b1 = data; return; }

  //DMA_LEN
  if(addr == 0x0046) { r.dmaLength.b0 = data & ~1; return; }
  if(addr == 0x0047) { r.dmaLength.b1 = data; return; }

  //DMA_CTRL
  if(addr == 0x0048) {
    r.dmaEnable = (uint1)(data >> 7);
    r.dmaMode   = (uint1)(data >> 0);
    if(r.dmaEnable) dmaTransfer();
    return;
  }

  //WSC_SYSTEM
  if(addr == 0x0062) {
    //todo: d0 = 1 powers off system
    return;
  }

  //HW_FLAGS
  if(addr == 0x00a0) {
    //todo: d2 (bus width) bit is writable; but ... it will do very bad things
    return;
  }

  //INT_BASE
  if(addr == 0x00b0) {
    r.interruptBase = WS() ? (data & ~7) : (data & ~1);
    return;
  }

  //INT_ENABLE
  if(addr == 0x00b2) {
    r.interruptEnable = data;
    return;
  }

  //INT_ACK
  if(addr == 0x00b6) {
    r.interruptStatus &= ~data;
    return;
  }
}
