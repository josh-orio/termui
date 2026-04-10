#ifndef TOGGLEMENU_HPP
#define TOGGLEMENU_HPP

#include <termui/ui/base/padded-text.hpp>
#include <termui/ui/interfaces/interface.hpp>
#include <termui/ui/widgets/toggle-list.hpp>

namespace termui {

class ToggleMenu : public Interface {
public:
  ToggleMenu(const termui::string &title, const termui::strings &elements, uint line_seperation = 1);

  void show();
  uint cursor(); // returns cursor (cant really see a use case)

  bool              isSelected(int i);
  std::vector<uint> selmap(); // returns idxs of each selected element

private:
  termui::string  title;
  termui::strings elements;

  PaddedText      title_banner;
  ToggleList      list;
  static Text     control_banner;
  static StyleMap styles;

  uint       line_seperation;
  const uint lvo = 5; // list vertical overhead
  const uint lho = 4; // list horizontal overhead
  bool       reprint; // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

} // namespace termui

#endif
