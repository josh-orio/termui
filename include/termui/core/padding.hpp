#ifndef PADDING_HPP
#define PADDING_HPP

#include <sys/types.h>

namespace termui {

struct Padding {
  uint left, right, top, bottom;
};

namespace PaddingStyle {
inline Padding none{0, 0, 0, 0};
inline Padding label(1, 1, 0, 0);
inline Padding one(1, 1, 1, 1);
} // namespace PaddingStyle

} // namespace termui

#endif
