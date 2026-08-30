#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <termui/core/str.hpp>
#include <termui/core/style.hpp>

namespace termui {

class Button {
public:
  Button(const termui::string &text, Style focus_style = {}, Style blur_style = {});

  Button &focus_style(Style s);
  Button &blur_style(Style s);
  Button &width(unsigned int w);

  void focus(), blur();

  void render();

private:
  termui::string _text;
  termui::Style  _focus_style, _blur_style;
  unsigned int           _w;
  bool           _selected;
};

} // namespace termui

#endif
