#include <termui/ui/ui.hpp>

#include <algorithm>
#include <deque>

namespace termui {

Input::Input(const termui::string &value, const termui::string &placeholder, const Style &valStyle, const Style &plhStyle)
  : _value(value), _placeholder(placeholder), _valStyle(valStyle), _plhStyle(plhStyle){};

Input &Input::value(const termui::string &v) {
  _value = v;
  return *this;
}

Input &Input::placeholder(const termui::string &p) {
  _placeholder = p;
  return *this;
}

Input &Input::valueStyle(const Style &s) {
  _valStyle = s;
  return *this;
}

Input &Input::placeholderStyle(const Style &s) {
  _plhStyle = s;
  return *this;
}

Input &Input::width(uint width) {
  _w = width;
  return *this;
}

Input &Input::height(uint height) {
  _h = height;
  return *this;
}

Input &Input::size(uint width, uint height) {
  _w = width;
  _h = height;
  return *this;
}

const termui::string &Input::get_value() const { return _value; }

std::string Input::render() {
  if (_w == 0 || _h == 0) {
    return "";
  }

  std::string outbuff;

  // -- print 'background' --
  for (int i = 0; i < _h; i++) {
    outbuff += std::string(_w, ' ');
    outbuff += curs_left(_w) + curs_down(1);
  }
  outbuff += curs_up(_h);

  std::deque<std::string> formatted;

  std::string &v = _value;
  std::string &p = _placeholder;

  bool use_response = !v.empty(); //.text().empty();
  bool use_placeholder = !use_response && !p.empty();

  if (use_response) {
    std::string copy = _value; //.text();
    size_t      next(0);

    if (_h == 1) {
      formatted.push_back(std::string(copy.end() - reverse_max_visible_length(copy, _w), copy.end()));

    } else {
      while (copy.size() > 0) {
        // clang-format off
        next = std::min({
          copy.size(), 

          max_visible_length(copy, _w), 

          copy.find('\n') == std::string::npos 
            ? copy.size() 
            : copy.find('\n'),

          copy.rfind(' ', max_visible_length(copy, _w)) == std::string::npos || visible_length(copy) <= _w
            ? copy.size()
            : copy.rfind(' ', max_visible_length(copy, _w))
        });
        // clang-format on

        formatted.push_back(std::string(copy.begin(), copy.begin() + next)); // make substring of <=w printed symbols

        if (copy.begin() + next < copy.end()) {
          if (copy[next] == '\n' || copy[next] == ' ') { // special handling for these two chars as they will cause infinite loops otherwise
            copy = std::string(copy.begin() + next + 1, copy.end());
          } else {
            copy = std::string(copy.begin() + next, copy.end());
          }
        } else {
          copy = "";
        }
      }
    }
  } else if (use_placeholder) {
    std::string copy = _placeholder; //.text();
    std::size_t next(0);

    if (_h == 1) {
      formatted.push_back(std::string(copy.begin(), copy.begin() + max_visible_length(copy, _w)));

    } else {
      while (copy.size() > 0) {
        // clang-format off
      next = std::min({
        copy.size(), 

        max_visible_length(copy, _w), 

        copy.find('\n') == std::string::npos 
          ? copy.size() 
          : copy.find('\n'),

        copy.rfind(' ', max_visible_length(copy, _w)) == std::string::npos || visible_length(copy) <= _w
          ? copy.size()
          : copy.rfind(' ', max_visible_length(copy, _w))
      });
        // clang-format on

        formatted.push_back(std::string(copy.begin(), copy.begin() + next)); // make substring of <=w printed symbols

        if (copy.begin() + next < copy.end()) {
          if (copy[next] == '\n' || copy[next] == ' ') { // special handling for these two chars as they will cause infinite loops otherwise
            copy = std::string(copy.begin() + next + 1, copy.end());
          } else {
            copy = std::string(copy.begin() + next, copy.end());
          }
        } else {
          copy = "";
        }
      }
    }
  }

  if (formatted.back().size() == _w && _h > 1) { // moves cursor over to the next line, doesnt affect underlying data, just a visual effect.
    formatted.push_back("");
  }

  if (formatted.size() > _h) { // trim if too many lines are in the vector
    formatted = std::deque(formatted.end() - _h, formatted.end());
  }

  for (int i = 0; i < formatted.size(); i++) {
    if (i > 0) {
      outbuff += curs_down(1) + curs_left(formatted.at(i - 1).size());
    }

    outbuff += formatted.at(i);
  }

  if (use_placeholder) { // cursor should be position at the start of text if placeholder is being displayed
    if (formatted.size() > 1) {
      outbuff += curs_up(formatted.size() - 1);
    }
    if (formatted.back().size() > 0) {
      outbuff += curs_left(formatted.back().size());
    }
  }

  if (use_response) {
    return renderer.render(outbuff, _valStyle);

  } else if (use_placeholder) {
    return renderer.render(outbuff, _plhStyle);

  } else {
    return outbuff;
  }
}

} // namespace termui
