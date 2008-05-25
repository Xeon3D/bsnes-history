void pFrame::create(uint style, uint width, uint height, const char *text) {
  hwnd = CreateWindow(L"BUTTON", utf16(text), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
    0, 0, width, height,
    phiro().default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)phiro().default_font, 0);
}

void pFrame::set_text(const char *text) {
  SetWindowText(hwnd, utf16(text));
}

pFrame::pFrame(Frame &self_) : pFormControl(self_), self(self_) {
}
