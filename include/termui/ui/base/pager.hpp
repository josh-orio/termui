#ifndef PAGER_HPP
#define PAGER_HPP

#include <termui/core/str.hpp>
#include <termui/core/style.hpp>
#include <termui/ui/base/text.hpp>

namespace termui {

class Pager : public Text {
public:
  Pager(const termui::string &str, const Style &style = {}, uint width = 0, uint height = 0);

  Pager &cursor_up(uint count = 1);
  Pager &cursor_down(uint count = 1);

  uint get_cursor();

  std::string render();

private:
  uint _cursor; // treated as the 'start line'
};

} // namespace termui

#endif
