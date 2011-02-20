void pComboBox::append(const string &text) {
  SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)(wchar_t*)utf16_t(text));
  if(SendMessage(hwnd, CB_GETCOUNT, 0, 0) == 1) setSelection(0);
}

void pComboBox::reset() {
  SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
}

unsigned pComboBox::selection() {
  return SendMessage(hwnd, CB_GETCURSEL, 0, 0);
}

void pComboBox::setSelection(unsigned row) {
  SendMessage(hwnd, CB_SETCURSEL, row, 0);
}

void pComboBox::constructor() {
  setParent(Window::None);
}

void pComboBox::setParent(Window &parent) {
  if(hwnd) DestroyWindow(hwnd);
  hwnd = CreateWindow(
    L"COMBOBOX", L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    0, 0, 0, 0,
    parent.p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&comboBox);
  setDefaultFont();
  foreach(text, comboBox.state.text) append(text);
  setSelection(comboBox.state.selection);
}
