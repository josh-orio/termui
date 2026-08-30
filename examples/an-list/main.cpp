#include <termui/termui.hpp>

int main() {
  auto focus = tui::stl(57, tui::clr::Inherit(), {termui::SGR::Bold});
  auto blur = tui::stls::none;

  tui::List l({
      "Gulfstream G500",
      "Gulfstream G700",
      "Gulfstream G800",
  });
  l.focus_style(focus).blur_style(blur).width(20).height(5).line_seperation(1);
  l.render();

  tui::term.flush();

  auto tmp = 0;

  return 0;
}
