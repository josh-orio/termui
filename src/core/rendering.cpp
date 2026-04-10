#include <termui/core/rendering.hpp>

namespace termui {

Renderer::Renderer(ColorMode color_capability) : color_capability(color_capability) {}

std::string Renderer::render(const std::string &str, const Style &style) const {
  if (style == Styles::none) {
    return str;
  }

  std::string       copy = str;
  const std::string target = "\x1b[0m";
  size_t            pos;
  while ((pos = copy.find(target)) != std::string::npos) {
    copy.erase(pos, target.size());
  }

  std::vector<int> modifier_codes;

  if (color_capability != ColorMode::ASCII && !style.Attributes().empty()) {
    for (auto attr : style.Attributes()) {
      modifier_codes.push_back(static_cast<int>(attr));
    }
  }

  auto truecolor_to_ansi256 = [](uint8_t r, uint8_t g, uint8_t b) -> int {
    // map each component to 0..5
    auto conv = [](uint8_t c) -> int {
      if (c < 48)
        return 0;
      if (c < 115)
        return 1;
      return (int)((c - 35) / 40); // maps 0..255 to 0..5 roughly
    };
    int rr = conv(r), gg = conv(g), bb = conv(b);
    return 16 + 36 * rr + 6 * gg + bb;
  };

  // ansi256 code: ESC[38;5;{ID}m
  // trucol code:  ESC[38;2;{r};{g};{b}m

  if (style.Foreground().has_value()) {
    if (color_capability == ColorMode::ANSI256) {
      if (style.Foreground()->mode == ColorMode::ANSI256) {
        modifier_codes.push_back(38);
        modifier_codes.push_back(5);
        modifier_codes.push_back(style.Foreground()->value);

      } else if (style.Foreground()->mode == ColorMode::TRUECOLOR) {
        modifier_codes.push_back(38);
        modifier_codes.push_back(5);
        modifier_codes.push_back(truecolor_to_ansi256(style.Foreground()->rgb.r, style.Foreground()->rgb.g, style.Foreground()->rgb.b));
      }
    }

    else if (color_capability == ColorMode::TRUECOLOR) {
      if (style.Foreground()->mode == ColorMode::ANSI256) {
        modifier_codes.push_back(38);
        modifier_codes.push_back(5);
        modifier_codes.push_back(style.Foreground()->value);

      } else if (style.Foreground()->mode == ColorMode::TRUECOLOR) {
        modifier_codes.push_back(38);
        modifier_codes.push_back(2);
        modifier_codes.push_back(style.Foreground()->rgb.r);
        modifier_codes.push_back(style.Foreground()->rgb.g);
        modifier_codes.push_back(style.Foreground()->rgb.b);
      }
    }
  }

  if (style.Background().has_value()) {
    if (color_capability == ColorMode::ANSI256) {
      if (style.Background()->mode == ColorMode::ANSI256) {
        modifier_codes.push_back(48);
        modifier_codes.push_back(5);
        modifier_codes.push_back(style.Background()->value);

      } else if (style.Background()->mode == ColorMode::TRUECOLOR) {
        modifier_codes.push_back(48);
        modifier_codes.push_back(5);
        modifier_codes.push_back(truecolor_to_ansi256(style.Background()->rgb.r, style.Background()->rgb.g, style.Background()->rgb.b));
      }
    }

    else if (color_capability == ColorMode::TRUECOLOR) {
      if (style.Background()->mode == ColorMode::ANSI256) {
        modifier_codes.push_back(48);
        modifier_codes.push_back(5);
        modifier_codes.push_back(style.Background()->value);

      } else if (style.Background()->mode == ColorMode::TRUECOLOR) {
        modifier_codes.push_back(48);
        modifier_codes.push_back(2);
        modifier_codes.push_back(style.Background()->rgb.r);
        modifier_codes.push_back(style.Background()->rgb.g);
        modifier_codes.push_back(style.Background()->rgb.b);
      }
    }
  }

  if (!modifier_codes.empty()) {
    std::string attr_buff = "\x1b[";

    for (auto attr : modifier_codes) {
      attr_buff += std::to_string(static_cast<int>(attr)) + ";";
    }

    attr_buff = attr_buff.substr(0, attr_buff.size() - 1);

    attr_buff += "m";

    return attr_buff + copy + "\x1b[0m";
  }

  return str;
}

} // namespace termui
