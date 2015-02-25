namespace hiro {

struct pMenuItem : pAction {
  Declare(MenuItem, Action)

  auto setIcon(const image& icon) -> void;
  auto setText(const string& text) -> void;
};

}
