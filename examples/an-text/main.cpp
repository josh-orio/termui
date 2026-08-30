#include <termui/termui.hpp>

int main() {
  // termui::Style s(57, termui::Color::Inherit());
  // termui::Text("Hello").width(7).height(1).style(s).render();

  // auto tmp = 0;

  tui::Text t1("Hello");
  t1.width(5).height(1).style(termui::Styles::bold);
  t1.render();

  tui::Text t2("world");
  t2.width(5).height(1);
  t2.render();

  tui::term.flush();

  auto tmp = 0;

  return 0;
}
