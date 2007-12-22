#include "miu.h"

namespace ns_miu {

/* Miu (singleton) */

void Miu::init() { p.init(); }
void Miu::term() { p.term(); }
bool Miu::run() { return p.run(); }
bool Miu::pending() { return p.pending(); }
bool Miu::file_load(Window *focus, char *filename, const char *filter, const char *path) { return p.file_load(focus, filename, filter, path); }
bool Miu::file_save(Window *focus, char *filename, const char *filter, const char *path) { return p.file_save(focus, filename, filter, path); }
uint Miu::screen_width() { return p.screen_width(); }
uint Miu::screen_height() { return p.screen_height(); }
Miu& Miu::handle() { static Miu miu; return miu; }
Miu::Miu() : p(*new pMiu(*this)) {}
Miu::~Miu() { delete &p; }
Miu& miu() { return Miu::handle(); }

/* Widget */

void Widget::show(bool state) { p.show(state); }
void Widget::hide() { p.hide(); }
bool Widget::visible() { return p.visible(); }
uintptr_t Widget::handle() { return p.handle(); }
Widget::Widget() : p(*new pWidget(*this)) { type = WidgetType; }
Widget::Widget(pWidget &p_) : p(p_) { type = WidgetType; }
Widget::~Widget() { delete &p; }

/* Widget -> Window */

void Window::create(uint style, uint width, uint height, const char *text) { p.create(style, width, height, text); }
void Window::close() { p.close(); }
void Window::move(uint x, uint y) { p.move(x, y); }
void Window::resize(uint width, uint height) { p.resize(width, height); }
void Window::focus() { p.focus(); }
bool Window::focused() { return p.focused(); }
void Window::fullscreen() { p.fullscreen(); }
void Window::unfullscreen() { p.unfullscreen(); }
void Window::set_background_color(uint8 r, uint8 g, uint8 b) { p.set_background_color(r, g, b); }
void Window::set_text(const char *text) { p.set_text(text); }
void Window::attach(Window &window, uint x, uint y) { p.attach(window, x, y); }
void Window::attach(MenuGroup &menugroup) { p.attach(menugroup); }
void Window::attach(FormControl &formcontrol, uint x, uint y) { p.attach(formcontrol, x, y); }
void Window::move(Window &window, uint x, uint y) { p.move(window, x, y); }
void Window::move(FormControl &formcontrol, uint x, uint y) { p.move(formcontrol, x, y); }
void Window::menu_show(bool state) { p.menu_show(state); }
void Window::menu_hide() { p.menu_hide(); }
bool Window::menu_visible() { return p.menu_visible(); }
Window::Window() :
  base_from_member<pWindow&>(*new pWindow(*this)),
  Widget(base_from_member<pWindow&>::value),
  p(base_from_member<pWindow&>::value) { type = WindowType; }
Window::Window(pWindow &p_) :
  base_from_member<pWindow&>(p_),
  Widget(base_from_member<pWindow&>::value),
  p(base_from_member<pWindow&>::value) { type = WindowType; }

/* Widget -> MenuControl */

void MenuControl::enable(bool state) { p.enable(state); }
void MenuControl::disable() { p.disable(); }
bool MenuControl::enabled() { return p.enabled(); }
MenuControl::MenuControl() :
  base_from_member<pMenuControl&>(*new pMenuControl(*this)),
  Widget(base_from_member<pMenuControl&>::value),
  p(base_from_member<pMenuControl&>::value) { type = MenuControlType; }
MenuControl::MenuControl(pMenuControl &p_) :
  base_from_member<pMenuControl&>(p_),
  Widget(base_from_member<pMenuControl&>::value),
  p(base_from_member<pMenuControl&>::value) { type = MenuControlType; }

/* Widget -> MenuControl -> MenuGroup */

MenuGroup& MenuGroup::create(const char *text) { p.create(text); return *this; }
void MenuGroup::attach(MenuControl &menucontrol) { p.attach(menucontrol); }
MenuGroup::MenuGroup() :
  base_from_member<pMenuGroup&>(*new pMenuGroup(*this)),
  MenuControl(base_from_member<pMenuGroup&>::value),
  p(base_from_member<pMenuGroup&>::value) { type = MenuGroupType; }

/* Widget -> MenuControl -> MenuItem */

MenuItem& MenuItem::create(const char *text) { p.create(text); return *this; }
MenuItem::MenuItem() :
  base_from_member<pMenuItem&>(*new pMenuItem(*this)),
  MenuControl(base_from_member<pMenuItem&>::value),
  p(base_from_member<pMenuItem&>::value) { type = MenuItemType; }

/* Widget -> MenuControl -> MenuCheckItem */

MenuCheckItem& MenuCheckItem::create(const char *text) { p.create(text); return *this; }
void MenuCheckItem::check(bool state) { state ? p.check() : p.uncheck(); }
void MenuCheckItem::uncheck() { p.uncheck(); }
bool MenuCheckItem::checked() { return p.checked(); }
MenuCheckItem::MenuCheckItem() :
  base_from_member<pMenuCheckItem&>(*new pMenuCheckItem(*this)),
  MenuControl(base_from_member<pMenuCheckItem&>::value),
  p(base_from_member<pMenuCheckItem&>::value) { type = MenuCheckItemType; }

/* Widget -> MenuControl -> MenuRadioItem */

MenuRadioItem& MenuRadioItem::create(MenuRadioItemGroup &group, const char *text) { p.create(group, text); return *this; }
void MenuRadioItem::check() { p.check(); }
bool MenuRadioItem::checked() { return p.checked(); }
MenuRadioItem::MenuRadioItem() :
  base_from_member<pMenuRadioItem&>(*new pMenuRadioItem(*this)),
  MenuControl(base_from_member<pMenuRadioItem&>::value),
  p(base_from_member<pMenuRadioItem&>::value) { type = MenuRadioItemType; }

/* Widget -> MenuControl -> MenuSeparator */

MenuSeparator& MenuSeparator::create() { p.create(); return *this; }
MenuSeparator::MenuSeparator() :
  base_from_member<pMenuSeparator&>(*new pMenuSeparator(*this)),
  MenuControl(base_from_member<pMenuSeparator&>::value),
  p(base_from_member<pMenuSeparator&>::value) { type = MenuSeparatorType; }

/* Widget -> FormControl */

void FormControl::resize(uint width, uint height) { p.resize(width, height); }
void FormControl::focus() { p.focus(); }
bool FormControl::focused() { return p.focused(); }
void FormControl::enable(bool state) { p.enable(state); }
void FormControl::disable() { p.disable(); }
bool FormControl::enabled() { return p.enabled(); }
FormControl::FormControl() :
  base_from_member<pFormControl&>(*new pFormControl(*this)),
  Widget(base_from_member<pFormControl&>::value),
  p(base_from_member<pFormControl&>::value) { type = FormControlType; }
FormControl::FormControl(pFormControl &p_) :
  base_from_member<pFormControl&>(p_),
  Widget(base_from_member<pFormControl&>::value),
  p(base_from_member<pFormControl&>::value) { type = FormControlType; }

/* Widget -> FormControl -> Frame */

void Frame::create(uint style, uint width, uint height, const char *text) { p.create(style, width, height, text); }
void Frame::set_text(const char *text) { p.set_text(text); }
Frame::Frame() :
  base_from_member<pFrame&>(*new pFrame(*this)),
  FormControl(base_from_member<pFrame&>::value),
  p(base_from_member<pFrame&>::value) { type = FrameType; }

/* Widget -> FormControl -> Canvas */

void Canvas::create(uint style, uint width, uint height) { p.create(style, width, height); }
void Canvas::redraw() { p.redraw(); }
uint32* Canvas::buffer() { return p.buffer(); }
Canvas::Canvas() :
  base_from_member<pCanvas&>(*new pCanvas(*this)),
  FormControl(base_from_member<pCanvas&>::value),
  p(base_from_member<pCanvas&>::value) { type = CanvasType; }

/* Widget -> FormControl -> Label */

void Label::create(uint style, uint width, uint height, const char *text) { p.create(style, width, height, text); }
void Label::set_text(const char *text) { p.set_text(text); }
Label::Label() :
  base_from_member<pLabel&>(*new pLabel(*this)),
  FormControl(base_from_member<pLabel&>::value),
  p(base_from_member<pLabel&>::value) { type = LabelType; }

/* Widget -> FormControl -> Button */

void Button::create(uint style, uint width, uint height, const char *text) { p.create(style, width, height, text); }
void Button::set_text(const char *text) { p.set_text(text); }
Button::Button() :
  base_from_member<pButton&>(*new pButton(*this)),
  FormControl(base_from_member<pButton&>::value),
  p(base_from_member<pButton&>::value) { type = ButtonType; }

/* Widget -> FormControl -> Checkbox */

void Checkbox::create(uint style, uint width, uint height, const char *text) { p.create(style, width, height, text); }
void Checkbox::set_text(const char *text) { p.set_text(text); }
void Checkbox::check(bool state) { state ? p.check() : p.uncheck(); }
void Checkbox::uncheck() { p.uncheck(); }
bool Checkbox::checked() { return p.checked(); }
Checkbox::Checkbox() :
  base_from_member<pCheckbox&>(*new pCheckbox(*this)),
  FormControl(base_from_member<pCheckbox&>::value),
  p(base_from_member<pCheckbox&>::value) { type = CheckboxType; }

/* Widget -> FormControl -> Radiobox */

void Radiobox::create(RadioboxGroup &group, uint style, uint width, uint height, const char *text) { p.create(group, style, width, height, text); }
void Radiobox::set_text(const char *text) { p.set_text(text); }
void Radiobox::check() { p.check(); }
bool Radiobox::checked() { return p.checked(); }
Radiobox::Radiobox() :
  base_from_member<pRadiobox&>(*new pRadiobox(*this)),
  FormControl(base_from_member<pRadiobox&>::value),
  p(base_from_member<pRadiobox&>::value) { type = RadioboxType; }

/* Widget -> FormControl -> Editbox */

void Editbox::create(uint style, uint width, uint height, const char *text) { p.create(style, width, height, text); }
uint Editbox::get_text(char *text, uint length) { return p.get_text(text, length); }
void Editbox::set_text(const char *text) { p.set_text(text); }
Editbox::Editbox() :
  base_from_member<pEditbox&>(*new pEditbox(*this)),
  FormControl(base_from_member<pEditbox&>::value),
  p(base_from_member<pEditbox&>::value) { type = EditboxType; }

/* Widget -> FormControl -> Listbox */

void Listbox::create(uint style, uint width, uint height, const char *columns, const char *text) { p.create(style, width, height, columns, text); }
void Listbox::autosize_columns() { p.autosize_columns(); }
void Listbox::set_column_width(uint column, uint width) { p.set_column_width(column, width); }
void Listbox::add_item(const char *text) { p.add_item(text); }
void Listbox::set_item(uint index, const char *text) { p.set_item(index, text); }
int  Listbox::get_selection() { return p.get_selection(); }
void Listbox::set_selection(int index) { p.set_selection(index); }
void Listbox::reset() { p.reset(); }
Listbox::Listbox() :
  base_from_member<pListbox&>(*new pListbox(*this)),
  FormControl(base_from_member<pListbox&>::value),
  p(base_from_member<pListbox&>::value) { type = ListboxType; }

/* Widget -> FormControl -> Combobox */

void Combobox::create(uint style, uint width, uint height, const char *text) { p.create(style, width, height, text); }
void Combobox::add_item(const char *text) { p.add_item(text); }
int  Combobox::get_selection() { return p.get_selection(); }
void Combobox::set_selection(int index) { p.set_selection(index); }
void Combobox::reset() { p.reset(); }
Combobox::Combobox() :
  base_from_member<pCombobox&>(*new pCombobox(*this)),
  FormControl(base_from_member<pCombobox&>::value),
  p(base_from_member<pCombobox&>::value) { type = ComboboxType; }

/* Widget -> FormControl -> Progressbar */

void Progressbar::create(uint style, uint width, uint height) { p.create(style, width, height); }
uint Progressbar::get_progress() { return p.get_progress(); }
void Progressbar::set_progress(uint progress) { p.set_progress(progress); }
Progressbar::Progressbar() :
  base_from_member<pProgressbar&>(*new pProgressbar(*this)),
  FormControl(base_from_member<pProgressbar&>::value),
  p(base_from_member<pProgressbar&>::value) { type = ProgressbarType; }

/* Widget -> FormControl -> Slider */

void Slider::create(uint style, uint width, uint height, uint length) { p.create(style, width, height, length); }
uint Slider::get_position() { return p.get_position(); }
void Slider::set_position(uint position) { p.set_position(position); }
Slider::Slider() :
  base_from_member<pSlider&>(*new pSlider(*this)),
  FormControl(base_from_member<pSlider&>::value),
  p(base_from_member<pSlider&>::value) { type = SliderType; }

} //namespace ns_miu
