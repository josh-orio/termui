#include <termui/ui/ui.hpp>

// TODO: implement cursor system
// TODO: dont allow cursor to run off screen i.e. move pager down when screen is full and wraps to next line

namespace termui {

// clang-format off
StyleMap TextEditor::styles({
  {"title", Style(Color::Inherit(), Colors::Lavender, {SGR::Bold})}
});
// clang-format on

Text TextEditor::control_strip("[ESC] close  [↑/↓/←/→] reposition", Styles::faint);

TextEditor::TextEditor(const termui::string &t, const termui::string &c)
  : Interface(TermSetups::fullscreen), title(t), content(c), title_banner(title, styles.at("title"), PaddingStyle::label), pager(content) {}

void TextEditor::show() {
  term_setup.configure();

  state = State::Continue;
  reprint = true;

  do {
    if (reprint) {
      display();
      reprint = false;
    }
    process_input();

  } while (state == State::Continue);

  term_setup.reset();
}

void TextEditor::display() {
  update_size();

  terminal.ClearScreen();
  terminal.ClearScrollback();

  terminal.MoveCursor(2, 2);
  title_banner.render();

  terminal.MoveCursor(4, 5);
  pager.render();

  // draw line counters - must be after pager render otherwise cursor may be inaccurate
  auto first_line = pager.get_cursor(); // 0-based index of first visible line
  for (int i = 0; i < terminal.height() - voh; i++) {
    int line_number = first_line + i + 1; // +1 for 1-based display
    terminal.MoveCursor(i + 4, 0);

    terminal.StyleStack(Styles::faint);

    terminal.write(std::format("{:>3}{}", line_number, unicode::VERTICAL));

    terminal.StylePop();
  }

  terminal.MoveCursor(terminal.height(), 2);
  control_strip.render();

  terminal.flush();
}

void TextEditor::process_input() {
  std::string ec = terminal.read();

  if (ec == key::DEL) { // remove last char
    std::string &c = content;
    if (c.size() > 0) {
      c = std::string(c.begin(), c.end() - 1);
    }
    reprint = true;

  } else if (ec == key::L_ARROW) {
    // move cursor left

  } else if (ec == key::U_ARROW) {
    pager.cursor_up();
    reprint = true;

  } else if (ec == key::D_ARROW) {
    pager.cursor_down();
    reprint = true;

  } else if (ec == key::R_ARROW) {
    // move cursor right

  } else if (MouseInteraction(ec).match(EventType::ScrollUp)) {
    pager.cursor_up();
    reprint = true;

  } else if (MouseInteraction(ec).match(EventType::ScrollDown)) {
    pager.cursor_down();
    reprint = true;

  } else if (ec == key::ENTER) { // add line break
    std::string &c = content;
    c += '\n';
    reprint = true;

  } else if ((std::string{32} <= ec) && (ec <= std::string{126})) { // accept basically all chars
    std::string &c = content;
    c += ec;
    reprint = true;

  } else if (ec == key::ESC) { // left arrow closes info page
    state = State::Exit;
  }
}

void TextEditor::update_size() {
  title_banner.width(std::min(terminal.width() - toh, (unsigned int)visible_length(title) + title_banner.hPadding())).height(1);
  pager.width(terminal.width() - hoh).height(terminal.height() - voh);
  control_strip.width(terminal.width() - hoh).height(1);
}

} // namespace termui
