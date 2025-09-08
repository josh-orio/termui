#include "interfaces.hpp"

namespace termui {

InfoPage::InfoPage(const termui::string &t, const termui::string &c) : title(t), content(c), cons(), voh(5), hoh(4), line_cursor(0) {}
InfoPage::InfoPage(const std::string &t, const std::string &c) : title(t), content(c), cons(), voh(5), hoh(4), line_cursor(0) {}

void InfoPage::show() {
  cons.show(); // configure terminal

  reprint = true;

  do {
    if (reprint) {
      display();
      reprint = false;
    }
    process_input();
  } while (state == state::CONTINUE);

  cons.close(); // reset terminal
}

void InfoPage::display() {
  update_size();

  std::vector<std::string> formatted = {};
  std::string copy = content.text(); //(*content);

  // a bit of preprocessing to fit the text in the terminal
  while (copy.length() > 0) {
    int next = 0;
    if (copy.find('\n') == std::string::npos) { // no linebreaks in remaining text
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

  cons.print(2, 2, bt(title.text())); //*title));

  int space_used = 0;
  for (int i = line_cursor; i < std::min(line_cursor + visible_lines, (int)formatted.size()); i++) {
    cons.print(space_used + 4, 3, formatted[i]);
    space_used++;
  }

  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓] scroll"));
  cons.flush();
}

void InfoPage::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::U_ARROW) {
    line_cursor -= (line_cursor > 0) ? 1 : 0; // decrement but dont let (cursor < 0)
    reprint = true;
    state = state::CONTINUE;

  } else if (ec == key::D_ARROW) {
    line_cursor += (line_cursor < content_lines - 1) ? 1 : 0; // increment but dont let (cursor > content_lines)
    reprint = true;
    state = state::CONTINUE;

  } else if (ec == key::ESC) { // left arrow closes info page
    state = state::EXIT;

  } else {
    state = state::CONTINUE;
  }
}

void InfoPage::update_size() {
  cons.update_size();

  visible_lines = cons.height - voh;
  text_width = cons.width - hoh;
}

} // namespace termui
