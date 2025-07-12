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
  } while (process_input());

  cons.close(); // reset terminal
}

void Info::display() {
  update_size();

  std::vector<std::string> formatted = {};
  std::string copy = content;

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

  cons.print(2, 2, title);

  int space_used = 0;
  for (int i = line_cursor; i < std::min(line_cursor + visible_lines, (int)formatted.size()); i++) {
    cons.print(space_used + 4, 3, formatted[i]);
    space_used++;
  }

  cons.print(cons.height, 2, faint_text("[←] return  [↑/↓] scroll"));
  cons.flush();
}

bool Info::process_input() {
  cons.poll_input(); // read in any unread chars

  std::vector<std::string> controls{key::U_ARROW, key::D_ARROW, key::L_ARROW}; // replacing this will be part of #12
  std::vector<size_t> cep(controls.size());                                    // control - earliest pos for each

  std::transform(controls.begin(), controls.end(), cep.begin(),
                 [this](const std::string &s) { return cons.inbuff.find(s); });

  int min = std::distance(std::begin(cep), std::min_element(std::begin(cep), std::end(cep)));

  if (cep[min] == std::string::npos) {
    return true;
  } // none of the controls are in the buffer

  std::string ec = controls[min]; // earliest control

  cons.inbuff.erase(0, cep[min] + ec.size()); // remove everything upto the end of the control (will erase anything
                                              // unintelligble before the control too)

  // ^ this implementation finds the first control key/sequence in the input buffer. this is done in the odd case that
  // multiple controls may be in the input buffer at once (incredibly unlikely)

  // now interface effects/changes can be handled
  if (ec == key::U_ARROW) {
    line_cursor -= (line_cursor > 0) ? 1 : 0; // decrement but dont let (cursor < 0)
    return true;

  } else if (ec == key::D_ARROW) {
    line_cursor += (line_cursor < content_lines - 1) ? 1 : 0; // increment but dont let (cursor > content_lines)
    return true;

  } else if (ec == key::L_ARROW) { // left arrow closes info page
    return false;

  } else {
    return true;
  }
}

void Info::update_size() {
  cons.update_size();

  text_width = cons.width - 4; // 2 space padding on each side
  visible_lines = cons.height - overhead;
}

} // namespace termui
