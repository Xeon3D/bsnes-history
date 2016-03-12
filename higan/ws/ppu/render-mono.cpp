auto PPU::renderMonoFetch(uint14 offset, uint3 y, uint3 x) -> uint2 {
  uint2 color;

  if(system.planar()) {
    uint16 data = iram.read(offset + (y << 1), Word);
    color |= data.bit( 7 - x) << 0;
    color |= data.bit(15 - x) << 1;
  }

  if(system.packed()) {
    uint8 data = iram.read(offset + (y << 1) + (x >> 2));
    color = data >> (6 - (x.bits(0,1) << 1));
  }

  return color;
}

auto PPU::renderMonoBack() -> void {
  uint4 poolColor = 15 - r.pool[r.backColor.bits(0,2)];
  pixel = {Pixel::Source::Back, poolColor << 0 | poolColor << 4 | poolColor << 8};
}

auto PPU::renderMonoScreenOne() -> void {
  if(!r.screenOneEnable) return;

  uint8 scrollY = status.vclk + r.scrollOneY;
  uint8 scrollX = status.hclk + r.scrollOneX;

  uint14 tilemapOffset = r.screenOneMapBase.bits(0,2) << 11;
  tilemapOffset += (scrollY >> 3) << 6;
  tilemapOffset += (scrollX >> 3) << 1;

  uint16 tile = iram.read(tilemapOffset, Word);
  uint14 tileOffset = 0x2000 + (tile.bits(0,8) << 4);
  uint3 tileY = (scrollY & 7) ^ (tile.bit(15) ? 7 : 0);
  uint3 tileX = (scrollX & 7) ^ (tile.bit(14) ? 7 : 0);
  uint2 tileColor = renderMonoFetch(tileOffset, tileY, tileX);
  if(tile.bit(11) && tileColor == 0) return;

  uint3 paletteColor = r.palette[tile.bits(9,12)].color[tileColor];
  uint4 poolColor = 15 - r.pool[paletteColor];
  pixel = {Pixel::Source::ScreenOne, poolColor << 0 | poolColor << 4 | poolColor << 8};
}

auto PPU::renderMonoScreenTwo() -> void {
  if(!r.screenTwoEnable) return;

  bool windowInside = status.vclk >= r.screenTwoWindowY0 && status.vclk <= r.screenTwoWindowY1
                   && status.hclk >= r.screenTwoWindowX0 && status.hclk <= r.screenTwoWindowX1;
  windowInside ^= r.screenTwoWindowInvert;
  if(r.screenTwoWindowEnable && !windowInside) return;

  uint8 scrollX = status.hclk + r.scrollTwoX;
  uint8 scrollY = status.vclk + r.scrollTwoY;

  uint14 tilemapOffset = r.screenTwoMapBase.bits(0,2) << 11;
  tilemapOffset += (scrollY >> 3) << 6;
  tilemapOffset += (scrollX >> 3) << 1;

  uint16 tile = iram.read(tilemapOffset, Word);
  uint14 tileOffset = 0x2000 + (tile.bits(0,8) << 4);
  uint3 tileY = (scrollY & 7) ^ (tile.bit(15) ? 7 : 0);
  uint3 tileX = (scrollX & 7) ^ (tile.bit(14) ? 7 : 0);
  uint2 tileColor = renderMonoFetch(tileOffset, tileY, tileX);
  if(tile.bit(11) && tileColor == 0) return;

  uint3 paletteColor = r.palette[tile.bits(9,12)].color[tileColor];
  uint4 poolColor = 15 - r.pool[paletteColor];
  pixel = {Pixel::Source::ScreenTwo, poolColor << 0 | poolColor << 4 | poolColor << 8};
}

auto PPU::renderMonoSprite() -> void {
  if(!r.spriteEnable) return;

  bool windowInside = status.hclk >= r.spriteWindowX0 && status.hclk <= r.spriteWindowX1;
  for(auto& sprite : sprites) {
    if(r.spriteWindowEnable && sprite.window && !windowInside) continue;
    if(status.hclk < sprite.x) continue;
    if(status.hclk > sprite.x + 7) continue;

    uint14 tileOffset = 0x2000 + (sprite.tile << 4);
    uint3 tileY = (uint8)(status.vclk - sprite.y) ^ (sprite.vflip ? 7 : 0);
    uint3 tileX = (uint8)(status.hclk - sprite.x) ^ (sprite.hflip ? 7 : 0);
    uint2 tileColor = renderMonoFetch(tileOffset, tileY, tileX);
    if(sprite.palette.bit(2) && tileColor == 0) continue;
    if(!sprite.priority && pixel.source == Pixel::Source::ScreenTwo) continue;

    uint3 paletteColor = r.palette[sprite.palette].color[tileColor];
    uint4 poolColor = 15 - r.pool[paletteColor];
    pixel = {Pixel::Source::Sprite, poolColor << 0 | poolColor << 4 | poolColor << 8};
    break;
  }
}
