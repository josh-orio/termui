#ifndef TOGGLEMENU_HPP
#define TOGGLEMENU_HPP

#include <termui/ui/base/padded-text.hpp>
#include <termui/ui/interfaces/interface.hpp>
#include <termui/ui/widgets/toggle-list.hpp>

namespace termui {

class ToggleMenu : public Interface {
public:
  ToggleMenu(const termui::string &title, const termui::strings &elements, unsigned int line_seperation = 1);

  void         show();
  unsigned int cursor(); // returns cursor (cant really see a use case)

  bool                      isSelected(int i);
  std::vector<unsigned int> selmap(); // returns idxs of each selected element

private:
  termui::string  title;
  termui::strings elements;

  PaddedText      title_banner;
  ToggleList      list;
  static Text     control_banner;
  static StyleMap styles;

  unsigned int       line_seperation;
  const unsigned int lvo = 5; // list vertical overhead
  const unsigned int lho = 4; // list horizontal overhead
  bool               reprint; // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

} // namespace termui

#endif
