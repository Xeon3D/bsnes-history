struct CPU : Processor::V30MZ, Thread, IO {
  static auto Enter() -> void;

  auto main() -> void;
  auto step(uint clocks) -> void;

  auto wait(uint clocks = 1) -> void override;
  auto read(uint20 addr) -> uint8 override;
  auto write(uint20 addr, uint8 data) -> void override;
  auto in(uint16 port) -> uint8 override;
  auto out(uint16 port, uint8 data) -> void override;

  auto power() -> void;

  //memory.cpp
  auto ramRead(uint16 addr) -> uint8;
  auto ramWrite(uint16 addr, uint8 data) -> void;

  //io.cpp
  auto portRead(uint16 addr) -> uint8 override;
  auto portWrite(uint16 addr, uint8 data) -> void override;

  //dma.cpp
  auto dmaTransfer() -> void;

  struct State {
    //$0040-0042 DMA_SRC
    uint20 dmaSource;

    //$0044-0045 DMA_DST
    uint16_ dmaTarget;

    //$0046-0047 DMA_LEN
    uint16_ dmaLength;

    //$0048 DMA_CTRL
    bool dmaEnable;
    bool dmaMode;  //0 = increment; 1 = decrement
  } s;
};

extern CPU cpu;
