#if defined(Hiro_ComboButton)

namespace hiro {

auto pComboButtonItem::construct() -> void {
}

auto pComboButtonItem::destruct() -> void {
}

auto pComboButtonItem::setIcon(const image& icon) -> void {
  if(auto parent = _parent()) {
    if(icon) {
      auto copy = icon;
      auto size = pFont::size(self().font(true), " ").height();
      copy.scale(size, size);
      auto pixbuf = CreatePixbuf(copy);
      gtk_list_store_set(parent->gtkListStore, &gtkIter, 0, pixbuf, -1);
    } else {
      gtk_list_store_set(parent->gtkListStore, &gtkIter, 0, nullptr, -1);
    }
  }
}

auto pComboButtonItem::setSelected() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    gtk_combo_box_set_active(parent->gtkComboBox, self().offset());
    parent->unlock();
  }
}

auto pComboButtonItem::setText(const string& text) -> void {
  if(auto parent = _parent()) {
    gtk_list_store_set(parent->gtkListStore, &gtkIter, 1, text.data(), -1);
  }
}

auto pComboButtonItem::_parent() -> pComboButton* {
  if(auto parent = self().parentComboButton()) return parent->self();
  return nullptr;
}

}

#endif
