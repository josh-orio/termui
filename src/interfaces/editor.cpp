#include "interfaces.hpp"

namespace termui {

Editor::Editor() = default;
Editor::Editor(const std::string &title, const std::string &content)
    : title(std::make_shared<std::string>(title)), content(std::make_shared<std::string>(content)), cons(), voh(5), hoh(5), line_cursor(0) {}
Editor::Editor(std::shared_ptr<std::string> sharedTitle, std::shared_ptr<std::string> sharedContent)
    : title(std::move(sharedTitle)), content(std::move(sharedContent)), cons(), voh(5), hoh(5), line_cursor(0) {}

void Editor::show() {
  cons.show(); // configure terminal

  do {
    display();
  } while (process_input());

  cons.close(); // reset terminal
}

void Editor::display() {
  update_size();

  cons.print(2, 2, bt(*title));
  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓/←/→] reposition"));

  // draw line counters
  for (int i = 1; i <= cons.height - voh; i++) {
    cons.print(i + 3 /* start on line 3 */, 0, fg_apply(std::format("{:>3}{}", line_cursor + i, unicode::VERTICAL), clr::LIGHTGREY));
  }

  std::vector<std::string> formatted = {};
  std::string copy = *content;

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

  cons.ct.on();
  cons.flush();
}

bool Editor::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::ESC) { // left arrow closes info page
    return false;

  } else if (ec == key::DEL) { // remove last char
    if ((*content).size() > 0) {
      (*content) = std::string((*content).begin(), (*content).end() - 1);
    }
    return true;

  } else if (ec == key::L_ARROW) { // remove last ch
    return true;

  } else if (ec == key::U_ARROW) { // remove last ch
    return true;

  } else if (ec == key::D_ARROW) { // remove last ch
    return true;

  } else if (ec == key::R_ARROW) { // remove last ch
    return true;

  } else if (ec == key::ENTER) { // line break not in range
    (*content) += '\n';
    return true;

  } else if ((std::string{32} <= ec) && (ec <= std::string{126})) {
    // add char accept ascii characters in range 32 - 126 input sanitization is done in classes, not here
    (*content) += ec;
    return true;

  } else {
    return true;
  }
}

void Editor::update_size() {
  cons.update_size();
  visible_lines = cons.height - voh;

  text_width = cons.width-6;
}

} // namespace termui

// add start line and arrow controls to start line
// requires documenting cursor position
