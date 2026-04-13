#ifndef INPUT_HPP
#define INPUT_HPP

#include <termui/core/str.hpp>
#include <termui/core/style.hpp>

namespace termui {

class Input {
public:
  Input(const termui::string &value = "", const termui::string &placeholder = "", const Style &valStyle = {}, const Style &plhStyle = Styles::faint);

  Input &value(const termui::string &v);
  Input &placeholder(const termui::string &p);
  Input &valueStyle(const Style &s);
  Input &placeholderStyle(const Style &s);
  Input &width(uint w);
  Input &height(uint h);
  Input &size(uint w, uint h); // convenience: set both at once

  const termui::string &get_value() const;

  std::string render();

private:
  termui::string _value;
  termui::string _placeholder;
  Style          _valStyle;
  Style          _plhStyle;
  uint           _w, _h;
};

} // namespace termui

#endif
