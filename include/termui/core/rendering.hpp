#ifndef RNDR_HPP
#define RNDR_HPP

#include <format>
#include <map>
#include <sstream>
#include <string>
#include <termui/core/color.hpp>
#include <termui/core/str.hpp>
#include <termui/core/style.hpp>

namespace termui {

class Renderer {
public:
  // disallow construction using other objects & assignment
  // Renderer(const Renderer &) = delete;
  // Renderer &operator=(const Renderer &) = delete;

  Renderer(ColorMode color_capability);

  std::string render(const std::string &str, const Style &style) const;

private:
  ColorMode color_capability;
};

} // namespace termui

#endif
