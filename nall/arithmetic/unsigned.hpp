#pragma once

namespace nall {

template<typename T, enable_if_t<is_unsigned<T>::value>> alwaysinline auto upper(T value) -> T {
  return value >> sizeof(T) * 4;
}

template<typename T, enable_if_t<is_unsigned<T>::value>> alwaysinline auto lower(T value) -> T {
  static const T Mask = T(0) - 1 >> sizeof(T) * 4;
  return value & Mask;
}

alwaysinline auto square(uintmax value) -> uintmax {
  return value * value;
}

template<typename T, typename U> alwaysinline auto rol(const T& lhs, const U& rhs, enable_if_t<is_unsigned<T>::value>* = 0) -> T {
  return lhs << rhs | lhs >> (sizeof(T) * 8 - rhs);
}

template<typename T, typename U> alwaysinline auto ror(const T& lhs, const U& rhs, enable_if_t<is_unsigned<T>::value>* = 0) -> T {
  return lhs >> rhs | lhs << (sizeof(T) * 8 - rhs);
}

#if INTMAX_BITS >= 128
inline auto operator"" _u128(const char* s) -> uint128_t {
  uint128_t p = 0;
  if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
    s += 2;
    while(*s) {
      auto c = *s++;
      if(c == '\'');
      else if(c >= '0' && c <= '9') p = (p << 4) + (c - '0');
      else if(c >= 'a' && c <= 'f') p = (p << 4) + (c - 'a' + 10);
      else if(c >= 'A' && c <= 'F') p = (p << 4) + (c - 'A' + 10);
      else break;
    }
  } else {
    while(*s) {
      auto c = *s++;
      if(c == '\'');
      else if(c >= '0' && c <= '9') p = (p << 3) + (p << 1) + (c - '0');
      else break;
    }
  }
  return p;
}
#endif

}
