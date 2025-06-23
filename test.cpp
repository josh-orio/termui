#include <iostream>
#include "include/termui.hpp"

int main() {
  Menu ("Menu", {"Option 1", "Option 2", "Option 3"}).show();

  Input("Input", {"Field 1", "Field 2", "Field 3", "Field 4", "Field 5",
                  "Field 6", "Field 7", "Field 8"})
      .show();

  Info ("Info",
  "this\nis\ninfo\nand\nthis\nis\nmore\ninfo\nand\nthis\nis\neven\nmore\ninfo.").show();

  std::vector<std::string> c{"Name", "Job", "Address"};
  nlohmann::json j;
  for (int i = 0; i < 20; i++) {
    j[i] = {{"Name", std::to_string(i) + "NameNameNameNameNameNameNameNameNameNameNameNameNameNameNameNameNameNameNameNameName"},
            {"Job", "Programmer"},
            {"Address", "Home"}};
  }
  Table("Table", c, j).show();

  return 0;
}