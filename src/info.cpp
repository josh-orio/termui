#include "interfaces.hpp"

namespace termui {

Info::Info(const std::string &t, const std::string &c) : title(t), content(c) {
  cons = Console(false, false, false, true);
  line_cursor = 0;
  overhead = 4; // header(3) + footer(1)
}

void Info::show() {
  cons.show(); // configure terminal

  do {
    display();
  } while (await_input());

  cons.close(); // reset terminal
}

void Info::display() {
  update_size();

  std::vector<std::string> formatted = {};
  std::string copy = content;

  // a bit of preprocessing to fit the text in the terminal
  while (copy.length() > 0) {
    int next = 0;
    if (copy.find('\n') == std::string::npos) {
      next = std::min({(int)copy.size(), text_width});
    } else {
      next = std::min({(int)copy.size(), text_width, (int)copy.find('\n')});
    }

    formatted.push_back(std::string(copy.begin(), copy.begin() + next));

    if (copy.begin() + next + 1 < copy.end()) {
      if (copy[next] == '\n') {
        copy = std::string(copy.begin() + next + 1, copy.end());
      } else {
        copy = std::string(copy.begin() + next, copy.end());
      }
    } else {
      copy = "";
    }
  }

  content_lines = formatted.size();

  cons.print(2, 2, title);

  int space_used = 0;
  for (int i = line_cursor; i < std::min(line_cursor + visible_lines, (int)formatted.size()); i++) {
    cons.print(space_used + 4, 3, formatted[i]);
    space_used++;
  }

  cons.print(cons.height, 2, faint_text("[←] return  [↑/↓] scroll"));
  cons.flush();
}

bool Info::await_input() {
  if (std::getchar() == keys::ESC) {
    if (std::getchar() == '[') { // random char in escape sequence
      switch (std::getchar()) {
      case keys::U_ARROW:
        // decrement but dont let (cursor < 0)
        line_cursor -= (line_cursor > 0) ? 1 : 0;
        return true;

      case keys::D_ARROW:
        // increment but dont let (cursor > options.size)
        line_cursor += (line_cursor < content_lines - 1) ? 1 : 0;
        return true;

      case keys::L_ARROW:
        // left arrow closes info page
        return false;
      }
    }
  }

  return true;
}

void Info::update_size() {
  cons.update_size();

  text_width = cons.width - 4; // 2 space padding on each side
  visible_lines = cons.height - overhead;
}

} // namespace termui
