#include <termui/ui/ui.hpp>

#include <algorithm>

namespace termui {

Text::Text(const termui::string &str, const Style &style, std::optional<unsigned int> width, std::optional<unsigned int> height)
  : _text(str), _style(style), _w(width), _h(height) {}

Text &Text::style(const Style &s) {
  _style = s;
  return *this;
}

Text &Text::align(const Alignment &a) {
  _align = a;
  return *this;
}

Text &Text::width(std::optional<unsigned int> w) {
  _w = w;
  return *this;
}

Text &Text::height(std::optional<unsigned int> h) {
  _h = h;
  return *this;
}

Text &Text::size(std::optional<unsigned int> w, std::optional<unsigned int> h) {
  _w = w;
  _h = h;
  return *this;
}

std::vector<std::string> Text::layout() const {
  const bool   has_w = _w.has_value(), has_h = _h.has_value();
  const size_t w = _w.value_or(0), h = _h.value_or(0); // 0 when unset, so only use these behind has_w / has_h

  if ((has_w && w == 0) || (has_h && h == 0)) {
    return {};
  }

  const std::string       &src = _text;
  std::vector<std::string> formatted;
  size_t                   pos(0), next(0); // byte offsets into src: start and end of the current line

  const std::string ellipsis       = "…";                      // adjust to "..." if you want 3 literal dots
  const size_t      ellipsis_width = visible_length(ellipsis); // width the ellipsis consumes on-screen

  constexpr size_t npos = std::string::npos;

  while (pos < src.size() && !(has_h && formatted.size() == h)) {
    const size_t nl       = src.find('\n', pos);
    const size_t line_end = std::min(nl, src.size());

    // text left over after this line, trailing newlines don't count as they have nothing to show
    const bool more_lines   = nl != npos && src.find_first_not_of('\n', nl) != npos;
    const bool is_last_line = has_h && formatted.size() == h - 1;
    const bool overflows    = has_w && max_visible_length(src, w, pos) < line_end; // measures this line only

    if (is_last_line && (more_lines || overflows)) {
      // Last line and it doesn't all fit: ignore word boundaries, take as much
      // as possible while leaving room for the ellipsis, and stop.
      size_t budget = (w > ellipsis_width) ? (w - ellipsis_width) : 0;
      next          = has_w ? std::min(line_end, max_visible_length(src, budget, pos)) : line_end;

      formatted.push_back(src.substr(pos, next - pos) + ellipsis);
      break;
    }

    const size_t limit = has_w ? max_visible_length(src, w, pos) : npos; // byte index where this line runs out of width
    const size_t rsp   = has_w ? src.rfind(' ', limit) : npos;           // last space at or before limit, anywhere in src
    const size_t sp    = (rsp == npos || rsp < pos) ? npos : rsp;        // word-wrap point: rsp if on this line (earlier spaces belong to previous lines)
    const bool   fits  = !has_w || visible_length(src, pos) <= w;        // whether all remaining text fits on this line

    // clang-format off
    next = std::min({
      src.size(),
      limit,
      nl,
      (sp == npos || fits) ? src.size() : sp
    });
    // clang-format on

    formatted.push_back(src.substr(pos, next - pos));

    if (next >= src.size()) {
      break;
    }

    pos = (src[next] == '\n' || src[next] == ' ') ? next + 1 : next;
  }

  if (has_h) {
    while (formatted.size() < h) { // add 'overrun' strings
      formatted.push_back(std::string(w, ' '));
    }
  }

  // align text, compares visible chars as bytes overcount utf-8 and sgr text, which would skip it
  if (has_w) {
    for (std::string &line : formatted) {
      const size_t vizlen = visible_length(line);

      if (vizlen >= w) {
        continue;
      }

      if (_align == Alignment::Left) {
        line += std::string(w - vizlen, ' ');
      }
      else if (_align == Alignment::Center) {
        line = std::string((w - vizlen) / 2, ' ') + line + std::string(w - vizlen - (w - vizlen) / 2, ' ');
      }
      else /* _align == Right */ {
        line = std::string(w - vizlen, ' ') + line;
      }
    }
  }

  return formatted;
}

std::pair<unsigned int, unsigned int> Text::size() {
  const std::vector<std::string> formatted = layout();

  if (formatted.empty()) {
    return {0, 0};
  }

  return {visible_length(formatted.at(0)), formatted.size()}; // returned as width, height
}

void Text::render() {
  const std::vector<std::string> formatted = layout();

  if (formatted.empty()) {
    return;
  }

  terminal.StyleStack(_style);

  for (int i = 0; i < formatted.size(); i++) {
    if (i > 0) {
      terminal.CursorLeft(_w.has_value() ? *_w : visible_length(formatted.at(i - 1))).CursorDown(1);
    }

    terminal.write(formatted.at(i));
  }

  terminal.StylePop();
}

} // namespace termui
