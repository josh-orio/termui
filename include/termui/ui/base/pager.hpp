#ifndef PAGER_HPP
#define PAGER_HPP

#include <termui/core/str.hpp>
#include <termui/core/style.hpp>
#include <termui/ui/base/text.hpp>

namespace termui {

class Pager : public Text {
public:
  Pager(const termui::string &str, const Style &style = {}, unsigned int width = 0, unsigned int height = 0);

  Pager &cursor_up(unsigned int count = 1);
  Pager &cursor_down(unsigned int count = 1);

  unsigned int get_cursor();

  void render();

private:
  unsigned int _cursor; // treated as the 'start line'
};

} // namespace termui

#endif
