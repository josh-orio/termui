#include <termui.hpp>

int main() {
  termui::BinaryMenu bm("Make your choice", "Would you like?", "Yes!", "No.");

  bm.show();

  return 0;
}
