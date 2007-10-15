#include "../../base.h"

void OBC1::init() {}
void OBC1::enable() {}

void OBC1::power() {
  reset();
}

void OBC1::reset() {
  memset(cartridge.ram, 0xff, 0x2000);
  status.baseptr = (cartridge.ram[0x1ff5] & 1) ? 0x1800 : 0x1c00;
  status.address = (cartridge.ram[0x1ff6] & 0x7f);
  status.shift   = (cartridge.ram[0x1ff6] & 3) << 1;
}

uint8 OBC1::read(uint16 addr) {
  addr &= 0x1fff;

  if((addr & 0x1ff8) != 0x1ff0) {
    return cartridge.ram[addr];
  }

  switch(addr) {
  case 0x1ff0:
    return cartridge.ram[status.baseptr + (status.address << 2) + 0];
  case 0x1ff1:
    return cartridge.ram[status.baseptr + (status.address << 2) + 1];
  case 0x1ff2:
    return cartridge.ram[status.baseptr + (status.address << 2) + 2];
  case 0x1ff3:
    return cartridge.ram[status.baseptr + (status.address << 2) + 3];
  case 0x1ff4:
    return cartridge.ram[status.baseptr + (status.address >> 2) + 0x200];
  case 0x1ff5:
  case 0x1ff6:
  case 0x1ff7:
    return cartridge.ram[addr];
  }

//never used, blocks compiler warning
  return 0x00;
}

void OBC1::write(uint16 addr, uint8 data) {
  addr &= 0x1fff;

  if((addr & 0x1ff8) != 0x1ff0) {
    cartridge.ram[addr] = data;
    return;
  }

  switch(addr) {
  case 0x1ff0:
    cartridge.ram[status.baseptr + (status.address << 2) + 0] = data;
    break;
  case 0x1ff1:
    cartridge.ram[status.baseptr + (status.address << 2) + 1] = data;
    break;
  case 0x1ff2:
    cartridge.ram[status.baseptr + (status.address << 2) + 2] = data;
    break;
  case 0x1ff3:
    cartridge.ram[status.baseptr + (status.address << 2) + 3] = data;
    break;
  case 0x1ff4: {
  uint8 temp;
    temp = cartridge.ram[status.baseptr + (status.address >> 2) + 0x200];
    temp = (temp & ~(3 << status.shift)) | ((data & 3) << status.shift);
    cartridge.ram[status.baseptr + (status.address >> 2) + 0x200] = temp;
  } break;
  case 0x1ff5:
    status.baseptr = (data & 1) ? 0x1800 : 0x1c00;
    cartridge.ram[addr] = data;
    break;
  case 0x1ff6:
    status.address = (data & 0x7f);
    status.shift   = (data & 3) << 1;
    cartridge.ram[addr] = data;
    break;
  case 0x1ff7:
    cartridge.ram[addr] = data;
    break;
  }
}

OBC1::OBC1() {}
OBC1::~OBC1() {}
