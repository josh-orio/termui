#ifndef TEXT_HPP
#define TEXT_HPP

#include <termui/core/str.hpp>
#include <termui/core/style.hpp>

namespace termui {

class Text {
public:
  enum class Alignment { Left, Center, Right };

  Text(const termui::string &str, const Style &style = {}, unsigned int width = 0, unsigned int height = 1);

  Text &style(const Style &s);
  Text &align(const Alignment &a);
  Text &width(unsigned int w);
  Text &height(unsigned int h);
  Text &size(unsigned int w, unsigned int h);

  virtual void render();

protected:
  termui::string _text;
  termui::Style  _style;
  Alignment      _align = Alignment::Left;
  unsigned int   _w, _h;
};

} // namespace termui

#endif
