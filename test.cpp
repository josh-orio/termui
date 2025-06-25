#include "include/interfaces.hpp"
#include "include/termui.hpp"
#include <iostream>

int main() {
  termui::Menu(termui::rv(termui::bt(" Menu ")),
               {"Option 1", "Option 2", "Option 3", "Option 4"})
      .show();

  termui::Input i(termui::rv(termui::bt(" Input ")),
                  {"Field 1", "Field 2", "Field 3", "Field 4", "Field 5",
                   "Field 6", "Field 7", "Field 8"});

  int i_status = 0;
  while (i_status != -1) {
    break; // remove to run

    i_status = i.show();
    if (i_status == 0) {
      continue;
    } else if (i_status > 0) {
      // maybe create a menu and feed the result back to i
      std::vector<std::string> o{"Option 1", "Option 2"};
      i.responses[i_status] =
          o[termui::Menu(termui::rv(termui::bt(" Sub-Menu ")), o).show()];
    }
  }

  std::vector<std::string> i_responses = i.get_responses();

  termui::Info(termui::rv(termui::bt(" Info ")),
               "this\nis\ninfo\nand\nthis\nis\nmore\ninfo\nand\nthis\nis\neven"
               "\nmore\ninfo.")
      .show();

  std::vector<std::string> c{"Name", "Job", "Address"};
  nlohmann::json j;
  for (int i = 0; i < 20; i++) {
    j[i] = {{"Name", std::to_string(i) +
                         "NameNameNameNameNameNameNameNameNameNameNameN"
                         "ameNameNameNameNameNameNameNameNameName"},
            {"Job", "Programmer"},
            {"Address", "Home"}};
  }

  termui::Table(termui::rv(termui::bt(" Table ")), c, j, 2, 1).show();

  return 0;
}