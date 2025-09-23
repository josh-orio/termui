#include "interfaces.hpp"

namespace termui {

Editor::Editor(const std::string &title, const termui::string &content) : title(title), content(content), voh(5), hoh(5) {}
Editor::Editor(const termui::string &title, const termui::string &content) : title(title), content(content), voh(5), hoh(5) {}

void Editor::show() {
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

void Editor::display() {
  update_size();

  cons.print(2, 2, bold_text(title.text()));
  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓/←/→] reposition"));

  // draw line counters
  for (int i = 1; i <= cons.height - voh; i++) {
    cons.print(i + 3 /* start on line 3 */, 0, fg_apply(std::format("{:>3}{}", line_cursor + i, unicode::VERTICAL), clr::LIGHTGREY));
  }

  std::vector<std::string> formatted = {};
  std::string copy = content.text();

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

  int space_used = 0;
  for (int i = line_cursor; i < std::min(line_cursor + visible_lines, (int)formatted.size()); i++) {
    cons.print(space_used + 4, 5, formatted[i]);
    space_used++;
  }

  // move cursor to cursor position
  cons.ct.on();
  cons.flush();
}

void Editor::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::ESC) { // left arrow closes info page
    state = state::EXIT;

  } else if (ec == key::DEL) { // remove last char
    if (content.text().size() > 0) {
      content.text() = std::string(content.text().begin(), content.text().end() - 1);
    }
    reprint = true;
    state = state::CONTINUE;

  } else if (ec == key::L_ARROW) { // still need to implement cursor action
    state = state::CONTINUE;

  } else if (ec == key::U_ARROW) {
    state = state::CONTINUE;

  } else if (ec == key::D_ARROW) {
    state = state::CONTINUE;

  } else if (ec == key::R_ARROW) {
    state = state::CONTINUE;

  } else if (ec == key::ENTER) { // add line break
    content.text() += '\n';
    reprint = true;
    state = state::CONTINUE;

  } else if ((std::string{32} <= ec) && (ec <= std::string{126})) { // accept basically all chars
    content.text() += ec;
    reprint = true;
    state = state::CONTINUE;

  } else {
    state = state::CONTINUE;
  }
}

void Editor::update_size() {
  cons.update_size();

  visible_lines = cons.height - voh;
  text_width = cons.width - hoh;
}

} // namespace termui

// add start line and arrow controls to start line
// requires documenting cursor position
