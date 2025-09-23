#include "interfaces.hpp"
#include <string>
#include <vector>

namespace termui {

Spreadsheet::Spreadsheet(const termui::string &title, const termui::strings &cols, const std::vector<termui::strings> &data)
    : title(title), table(termui::strings(cols), data, std::vector<int>(cols.size(), 1), 1), voh(4), hoh(2) {}
Spreadsheet::Spreadsheet(const std::string &title, const termui::strings &cols, const std::vector<termui::strings> &data)
    : title(title), table(termui::strings(cols), data, std::vector<int>(cols.size(), 1), 1), voh(4), hoh(2) {}
Spreadsheet::Spreadsheet(const std::string &title, const std::vector<std::string> &cols, const std::vector<termui::strings> &data)
    : title(title), table(termui::strings(cols), data, std::vector<int>(cols.size(), 1), 1), voh(4), hoh(2) {}

void Spreadsheet::show() {
  cons.show(); // configure terminal

  do {
    display();
    process_input();
  } while (state == state::CONTINUE);

  cons.close(); // reset terminal
}

void Spreadsheet::display() {
  update_size();

  cons.print(2, 2, bold_text(title.text()));
  cons.print(4, 2, table.render());
  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓] scroll  [↵] select"));
  cons.flush();
}

void Spreadsheet::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::ENTER) {
    cons.close();

    InputPage i(title, table.getCols(), table.getRow(table.cursor));
    i.show();

    cons.show();

    reprint = true;
    state = state::CONTINUE;

  } else if (ec == key::U_ARROW) {
    table.cursor_up();
    reprint = true;
    state = state::CONTINUE;

  } else if (ec == key::D_ARROW) {
    table.cursor_down();
    reprint = true;
    state = state::CONTINUE;

  } else if (ec == key::ESC) { // ESC closes spreadsheet
    state = state::EXIT;

  } else {
    state = state::CONTINUE;
  }
}

void Spreadsheet::update_size() {
  cons.update_size();

  table.table_height = cons.height - voh;

  int w = (cons.width - hoh - 1 - table.colCount()) / table.colCount();

  for (int i = 0; i < table.colCount(); i++) {
    table.colWidth(i) = w;
  }
}

} // namespace termui
