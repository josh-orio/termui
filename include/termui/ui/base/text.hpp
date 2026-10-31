#ifndef TEXT_HPP
#define TEXT_HPP

#include <termui/core/str.hpp>
#include <termui/core/style.hpp>
#include <algorithm>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace termui {

class Text {
public:
  enum class Alignment { Left, Center, Right };

  Text(const termui::string &str, const Style &style = {}, std::optional<unsigned int> width = std::nullopt, std::optional<unsigned int> height = std::nullopt);

  Text &style(const Style &s);
  Text &align(const Alignment &a);
  Text &width(std::optional<unsigned int> w);
  Text &height(std::optional<unsigned int> h);
  Text &size(std::optional<unsigned int> w, std::optional<unsigned int> h);

  std::pair<unsigned int, unsigned int> size(); // measures and returns the dimensions of rendered text

  virtual void render();

protected:
  std::vector<std::string> layout() const; // wraps, truncates and aligns _text, at most h lines long

  termui::string              _text;
  termui::Style               _style;
  Alignment                   _align = Alignment::Left;
  std::optional<unsigned int> _w, _h;
};

} // namespace termui

#endif
