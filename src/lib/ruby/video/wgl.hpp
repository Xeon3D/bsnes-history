class pVideoWGL;

class VideoWGL : public Video {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  bool lock(uint32_t *&data, unsigned &pitch);
  void unlock();

  void clear();
  void refresh(unsigned width, unsigned height);
  bool init();
  void term();

  VideoWGL();
  ~VideoWGL();

private:
  pVideoWGL &p;
};
