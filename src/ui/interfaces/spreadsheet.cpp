#include <termui/ui/ui.hpp>

namespace termui {

// clang-format off
StyleMap Spreadsheet ::styles({
  {"all", Style(Colors::White, 57, {SGR::Bold})}, 
});
// clang-format on

Text Spreadsheet::control_strip("[ESC] close  [↑/↓] scroll", Styles::faint);

Spreadsheet::Spreadsheet(const termui::string &title, const termui::strings &cols, const std::vector<termui::strings> &data)
  : Interface(TermSetups::fullscreen), title(title), title_banner(title, styles.at("all")), table(cols, data, {}) {}

void Spreadsheet::show() {
  term_setup.configure();

  state = Interface::State::CONTINUE;
  reprint = true;

  do {
    if (reprint) {
      display();
      reprint = false;
    }

    process_input();

  } while (state == State::CONTINUE);

  term_setup.reset();
}

void Spreadsheet::display() {
  update_size();

  terminal.ClearScreen();
  terminal.ClearScrollback();

  terminal.write(2, 2, title_banner.render());
  terminal.write(4, 2, table.render());
  terminal.write(terminal.height(), 2, control_strip.render());
  terminal.flush();
}

void Spreadsheet::process_input() {
  std::string ec = terminal.read();

  if (ec == key::U_ARROW) {
    table.cursor_up();
    reprint = true;

  } else if (ec == key::D_ARROW) {
    table.cursor_down();
    reprint = true;

  } else if (ec == key::ESC) { // ESC closes spreadsheet
    state = State::EXIT;

  } else {
  }
}

void Spreadsheet::update_size() {
  title_banner.width(std::min(terminal.width() - hoh, (uint)visible_length(title) + title_banner.hPadding())).height(1);

  int w = (terminal.width() - hoh - 1 - table.colCount()) / table.colCount();
  for (int i = 0; i < table.colCount(); i++) {
    table.column_width(i, w);
  }
  table.table_height(terminal.height() - voh);

  control_strip.width(terminal.width() - hoh).height(1);
}

} // namespace termui
