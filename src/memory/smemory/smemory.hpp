class sBus : public Bus {
public:
  void load_cart();
  void unload_cart();
  bool cart_loaded();

  void power();
  void reset();

  sBus();
  ~sBus();

private:
  bool is_cart_loaded;

  void map_reset();
  void map_system();
  void map_generic();
  void map_generic_sram();

  void map_cx4();
  void map_dsp1();
  void map_dsp2();
  void map_dsp3();
  void map_dsp4();
  void map_obc1();
  void map_st010();
};
