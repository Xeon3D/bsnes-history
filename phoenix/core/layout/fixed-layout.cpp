void FixedLayout::append(Sizable &sizable, const Geometry &geometry) {
  children.append({ &sizable, geometry });
  synchronize();
}

void FixedLayout::append(Sizable &sizable) {
  foreach(child, children) if(child.sizable == &sizable) return;
  Layout::append(sizable);
}

bool FixedLayout::enabled() {
  if(layout()) return state.enabled && layout()->enabled();
  return state.enabled;
}

Geometry FixedLayout::minimumGeometry() {
  unsigned width = MinimumSize, height = MinimumSize;
  foreach(child, children) {
    width  = max(width,  child.sizable->minimumGeometry().width);
    height = max(height, child.sizable->minimumGeometry().height);
  }
  return { 0, 0, width, height };
}

void FixedLayout::remove(Sizable &sizable) {
  for(unsigned n = 0; n < children.size(); n++) {
    if(children[n].sizable == &sizable) {
      children.remove(n);
      Layout::remove(sizable);
      break;
    }
  }
}

void FixedLayout::reset() {
  foreach(child, children) {
    if(window() && dynamic_cast<Widget*>(child.sizable)) window()->remove((Widget&)*child.sizable);
  }
}

void FixedLayout::setEnabled(bool enabled) {
  state.enabled = enabled;
  foreach(child, children) {
    child.sizable->setVisible(dynamic_cast<Widget*>(child.sizable) ? child.sizable->enabled() : enabled);
  }
}

void FixedLayout::setGeometry(const Geometry &geometry) {
}

void FixedLayout::setVisible(bool visible) {
  state.visible = visible;
  foreach(child, children) {
    child.sizable->setVisible(dynamic_cast<Widget*>(child.sizable) ? child.sizable->visible() : visible);
  }
}

void FixedLayout::synchronize() {
  foreach(child, children) {
    Layout::append(*child.sizable);
    child.sizable->setGeometry(child.geometry);
  }
}

bool FixedLayout::visible() {
  if(layout()) return state.visible && layout()->visible();
  return state.visible;
}

FixedLayout::FixedLayout() {
  state.enabled = true;
  state.visible = true;
}

FixedLayout::~FixedLayout() {
  while(children.size()) remove(*children[0].sizable);
}
