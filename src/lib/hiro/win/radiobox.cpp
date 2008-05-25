void pRadiobox::create(RadioboxGroup &group_, uint style, uint width, uint height, const char *text) {
  group = group_;
  hwnd = CreateWindow(L"BUTTON", utf16(text), WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_RADIOBUTTON,
    0, 0, width, height, phiro().default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)phiro().default_font, 0);
  if(group[0] == &self) check();
}

void pRadiobox::set_text(const char *text) {
  SetWindowText(hwnd, utf16(text));
}

void pRadiobox::check() {
  for(uint i = 0; i < group.size(); i++) {
    SendMessage(group[i]->p.hwnd, BM_SETCHECK, (WPARAM)(group[i] == &self), 0);
  }
}

bool pRadiobox::checked() {
  return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}

pRadiobox::pRadiobox(Radiobox &self_) : pFormControl(self_), self(self_) {
}
