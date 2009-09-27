#include "main.hpp"
#include "resource/resource.rcc"

//nall::string <> QString interface: allows string streaming; automatically converts to UTF-16
class utf8 : public nall::string {
public:
  template<typename T> utf8& operator<<(T t) { string::operator<<(t); return *this; }
  operator const QString() const { return QString::fromUtf8(*this); }
};

#if defined(PLATFORM_X)
  #include "platform/platform_x.cpp"
#elif defined(PLATFORM_OSX)
  #include "platform/platform_osx.cpp"
#elif defined(PLATFORM_WIN)
  #include "platform/platform_win.cpp"
#else
  #error "unsupported platform"
#endif

#include "libqb/libqb.cpp"
#include "config.cpp"
#include "interface.cpp"

const char defaultStylesheet[] =
  "QLabel.title {"
  "  font: bold 18px \"Georgia\";"
  "  margin-bottom: 5px;"
  "  margin-left: -5px;"
  "  margin-top: 5px;"
  "}"
  "\n"
  "#backdrop {"
  "  background: #000000;"
  "}"
  "\n"
  "#mouse-capture-box {"
  "  border: 1px solid #808080;"
  "  color: #000000;"
  "  font-weight: bold;"
  "}"
  "\n";

#include "application/application.cpp"
#include "debugger/debugger.cpp"
#include "input/input.cpp"
#include "utility/utility.cpp"

int main(int argc, char **argv) {
  return application.main(argc, argv);
}
