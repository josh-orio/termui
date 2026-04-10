#ifndef TEXT_HPP
#define TEXT_HPP

#include <termui/core/str.hpp>
#include <termui/core/style.hpp>

namespace termui {

class Text {
public:
  enum class Alignment { Left, Center, Right };

  Text(const termui::string &str, const Style &style = {}, uint width = 0, uint height = 0);

  Text &style(const Style &s);
  Text &align(const Alignment &a);
  Text &width(uint w);
  Text &height(uint h);
  Text &size(uint w, uint h);

  virtual std::string render();

protected:
  termui::string _text;
  termui::Style  _style;
  Alignment      _align = Alignment::Left;
  uint           _w, _h;
};

} // namespace termui

#endif
