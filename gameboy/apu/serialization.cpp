#ifdef APU_CPP

void APU::serialize(serializer &s) {
  square1.serialize(s);
  square2.serialize(s);
  wave.serialize(s);
  noise.serialize(s);
  master.serialize(s);
}

#endif
