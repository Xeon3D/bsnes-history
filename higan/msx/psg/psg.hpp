//General Instrument AY-3-8910

struct PSG : Thread {
  shared_pointer<Stream> stream;

  //psg.cpp
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto power() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;
};

extern PSG psg;
