#include "interfaces.hpp"
#include <memory>
#include <string>
#include <vector>

namespace termui {

Spreadsheet::Spreadsheet() = default;
Spreadsheet::Spreadsheet(const std::string &title, Table &table) : title(std::make_shared<std::string>(title)), data(std::make_shared<Table>(table)), voh(5) {
  cons = Console(false, false, false, true);
}
Spreadsheet::Spreadsheet(std::shared_ptr<std::string> title, std::shared_ptr<Table> table) : title(std::move(title)), data(std::move(table)), voh(5) {
  cons = Console(false, false, false, true);
}

void Spreadsheet::show() {
  cons.show(); // configure terminal

  do {
    display();
  } while (process_input());

  cons.close(); // reset terminal
}

void Spreadsheet::display() {
  update_size();

  cons.print(2, 2, bt(*title));
  cons.print(4, 2, (*data).render());
  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓] scroll  [↵] select"));
  cons.flush();
}

bool Spreadsheet::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::ENTER) {
    std::vector<std::string> cols;
    for (auto c : *(*data).columns) {
      cols.push_back(c.getText());
      std::cout << c.getText() << " ";
    } // this is also dumb

    std::shared_ptr<std::vector<std::string>> guh = std::make_shared<std::vector<std::string>>(cols);
    InputPage i(title, guh, ((*data).rows->at((*data).cursor).cells)); // cursed ngl

    i.show();

    return true;

  } else if (ec == key::U_ARROW) {
    (*data).cursor_up();
    return true;

  } else if (ec == key::D_ARROW) {
    (*data).cursor_down();
    return true;

  } else if (ec == key::ESC) { // ESC closes spreadsheet
    return false;

  } else {
    return true;
  }
}

void Spreadsheet::update_size() {
  cons.update_size();

  (*data).table_height = cons.height - voh;

  // need some logic to recalc col widths
}

} // namespace termui
