#ifndef PADDEDTEXT_HPP
#define PADDEDTEXT_HPP

#include <termui/core/padding.hpp>
#include <termui/ui/base/text.hpp>

namespace termui {

class PaddedText : public Text {
public:
  // ensure w/h > padding, otherwise text will not show
  PaddedText(const termui::string &str, Style style = {}, Padding pad_style = {});

  uint hPadding();
  uint vPadding();

  std::string render() override;

private:
  Padding _padding;
};

} // namespace termui

#endif
