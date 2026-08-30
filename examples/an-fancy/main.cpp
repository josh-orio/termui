#include <termui/termui.hpp>

int main() {

  auto focus = tui::stl(212, tui::clr::Inherit(), {tui::sgr::Bold});
  auto blur = tui::stl(tui::stls::none);

  tui::FancyList fl({{"gah", "damn"}, {"7.5", "10"}}, focus, blur);
  fl.width(15).height(10);
  fl.render();

  auto tmp = 0;

  return 0;
}
