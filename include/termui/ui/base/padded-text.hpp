#ifndef PADDEDTEXT_HPP
#define PADDEDTEXT_HPP

#include <termui/core/padding.hpp>
#include <termui/ui/base/text.hpp>

#include <algorithm>

namespace termui {

class PaddedText : public Text {
public:
  // ensure w/h > padding, otherwise text will not show
  PaddedText(const termui::string &str, Style style = {}, Padding pad_style = {});

  PaddedText                           &setPadding(Padding p);
  std::pair<unsigned int, unsigned int> size() override;

  unsigned int hPadding();
  unsigned int vPadding();

  void render() override;

private:
  Padding _padding;
};

} // namespace termui

#endif
