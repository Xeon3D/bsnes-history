static const unsigned FixedStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_BORDER;
static const unsigned ResizableStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;

void Window::append(Menu &menu) {
  menu.action->parentMenu = window->menu;
  menu.create();
  AppendMenu(window->menu, MF_STRING | MF_POPUP, (UINT_PTR)menu.action->menu, utf16_t(menu.action->text));
}

void Window::setLayout(Layout &layout) {
  window->layout = &layout;
  Geometry geom = geometry();
  geom.x = geom.y = 0;
  layout.setParent(*this);
  layout.setGeometry(geom);
}

void Window::setResizable(bool resizable) {
  window->resizable = resizable;
  SetWindowLongPtr(window->window, GWL_STYLE, window->resizable ? ResizableStyle : FixedStyle);
  setGeometry(window->x, window->y, window->width, window->height);
}

void Window::setDefaultFont(Font &font) {
  window->defaultFont = font.font->font;
}

void Window::setFont(Font &font) {
  SendMessage(window->status, WM_SETFONT, (WPARAM)font.font->font, 0);
}

Geometry Window::frameGeometry() {
  RECT rc;
  GetWindowRect(window->window, &rc);

  signed x = rc.left;
  signed y = rc.top;
  unsigned width = (rc.right - rc.left);
  unsigned height = (rc.bottom - rc.top);

  return { x, y, width, height };
}

Geometry Window::geometry() {
  Geometry fm = frameMargin();
  RECT rc;
  GetWindowRect(window->window, &rc);

  signed x = rc.left + fm.x;
  signed y = rc.top + fm.y;
  unsigned width = (rc.right - rc.left) - fm.width;
  unsigned height = (rc.bottom - rc.top) - fm.height;

  return { x, y, width, height };
}

void Window::setFrameGeometry(signed x, signed y, unsigned width, unsigned height) {
  Geometry fm = frameMargin();
  setGeometry(x + fm.x, y + fm.y, width - fm.width, height - fm.height);
}

void Window::setGeometry(signed x, signed y, unsigned width, unsigned height) {
  object->locked = true;
  if(window->fullscreen == false) {
    window->x = x;
    window->y = y;
    window->width = width;
    window->height = height;
  }
  Geometry fm = frameMargin();
  SetWindowPos(window->window, NULL, x - fm.x, y - fm.y, width + fm.width, height + fm.height, SWP_NOZORDER | SWP_FRAMECHANGED);
  SetWindowPos(window->status, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED);
  Geometry geom = geometry();
  geom.x = geom.y = 0;
  if(window->layout) window->layout->setGeometry(geom);
  object->locked = false;
}

void Window::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue) {
  if(window->brush) DeleteObject(window->brush);
  window->brushColor = RGB(red, green, blue);
  window->brush = CreateSolidBrush(window->brushColor);
}

void Window::setTitle(const string &text) {
  SetWindowText(window->window, utf16_t(text));
}

void Window::setStatusText(const string &text) {
  SendMessage(window->status, SB_SETTEXT, 0, (LPARAM)(wchar_t*)utf16_t(text));
}

void Window::setMenuVisible(bool visible) {
  object->locked = true;
  SetMenu(window->window, visible ? window->menu : 0);
  setGeometry(window->x, window->y, window->width, window->height);
  object->locked = false;
}

void Window::setStatusVisible(bool visible) {
  object->locked = true;
  ShowWindow(window->status, visible ? SW_SHOWNORMAL : SW_HIDE);
  setGeometry(window->x, window->y, window->width, window->height);
  object->locked = false;
}

bool Window::fullscreen() {
  return window->fullscreen;
}

void Window::setFullscreen(bool fullscreen) {
  object->locked = true;
  window->fullscreen = fullscreen;
  if(window->fullscreen == false) {
    SetWindowLong(window->window, GWL_STYLE, WS_VISIBLE | (window->resizable ? ResizableStyle : FixedStyle));
    setGeometry(window->x, window->y, window->width, window->height);
  } else {
    SetWindowLong(window->window, GWL_STYLE, WS_VISIBLE | WS_POPUP);
    setGeometry(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
  }
  object->locked = false;
}

bool Window::visible() {
  return GetWindowLongPtr(window->window, GWL_STYLE) & WS_VISIBLE;
}

void Window::setVisible(bool visible) {
  ShowWindow(window->window, visible ? SW_SHOWNORMAL : SW_HIDE);
}

bool Window::focused() {
  return (GetForegroundWindow() == window->window);
}

void Window::setFocused() {
  if(visible() == false) setVisible(true);
  SetFocus(window->window);
}

Window::Window() {
  window = new Window::Data;

  window->window = CreateWindowEx(
    0, L"phoenix_window", L"", ResizableStyle,
    0, 0, 64, 64, 0, 0, GetModuleHandle(0), 0
  );
  window->menu = CreateMenu();
  window->status = CreateWindowEx(
    0, STATUSCLASSNAME, L"", WS_CHILD,
    0, 0, 0, 0, window->window, 0, GetModuleHandle(0), 0
  );
  //StatusBar will be capable of receiving tab focus if it is not disabled
  SetWindowLongPtr(window->status, GWL_STYLE, GetWindowLong(window->status, GWL_STYLE) | WS_DISABLED);
  SetWindowLongPtr(window->window, GWLP_USERDATA, (LONG_PTR)this);
  setGeometry(128, 128, 256, 256);
}

//internal

Geometry Window::frameMargin() {
  //determine the frame overhead (window border, menu bar, status bar) to compute client geometry
  //x,y: pixels from the top-left of window to the client area
  //width,height: total pixels of window geometry that are not part of client area
  //width-x,height-y: pixels from the bottom-right of window to the client area
  RECT rc = { 0, 0, 640, 480 };
  AdjustWindowRect(&rc, window->resizable ? ResizableStyle : FixedStyle, (bool)GetMenu(window->window));
  unsigned statusHeight = 0;
  if(GetWindowLongPtr(window->status, GWL_STYLE) & WS_VISIBLE) {
    RECT src;
    GetClientRect(window->status, &src);
    statusHeight = src.bottom - src.top;
  }
  return { abs(rc.left), abs(rc.top), (rc.right - rc.left) - 640, statusHeight + (rc.bottom - rc.top) - 480 };
}
