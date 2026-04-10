#include <termui/termui.hpp>

int main() {
  termui::string               title("Spreadsheet Demo - Flight Schedule");
  termui::strings              columns({"Origin", "Destination", "Flight Number", "Duration"});
  std::vector<termui::strings> rows{{"London", "New York", "BAW1", "7:00"}, {"London", "Hong Kong", "CPA256", "14:30"}};

  termui::Spreadsheet s(title, columns, rows);

  s.show();

  return 0;
}
