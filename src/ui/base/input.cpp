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

void Input::render() {
  if (_w == 0 || _h == 0) {
    return;
  }

  std::string &v = _value;
  std::string &p = _placeholder;

  bool use_response = !v.empty();
  bool use_placeholder = !use_response && !p.empty();

  if (use_response) {
    terminal.StyleStack(_valStyle);

  } else if (use_placeholder) {
    terminal.StyleStack(_plhStyle);
  }

  // -- print 'background' --
  for (int i = 0; i < _h; i++) {
    terminal.write(std::string(_w, ' '));
    terminal.CursorLeft(_w).CursorDown(1);
  }
  terminal.CursorUp(_h);

  std::deque<std::string> formatted;

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

  if (formatted.size() > 0 && formatted.back().size() == _w &&
      _h > 1) { // moves cursor over to the next line, doesnt affect underlying data, just a visual effect.
    formatted.push_back("");
  }

  if (formatted.size() > _h) { // trim if too many lines are in the vector
    formatted = std::deque(formatted.end() - _h, formatted.end());
  }

  for (int i = 0; i < formatted.size(); i++) {
    if (i > 0) {
      terminal.CursorDown(1).CursorLeft(formatted.at(i - 1).size());
    }

    terminal.write(formatted.at(i));
  }

  if (use_placeholder) { // cursor should be position at the start of text if placeholder is being displayed
    if (formatted.size() > 1) {
      terminal.CursorUp(formatted.size() - 1);
    }
    if (formatted.back().size() > 0) {
      terminal.CursorLeft(formatted.back().size());
    }
  }

  terminal.StylePop();
}

} // namespace termui
