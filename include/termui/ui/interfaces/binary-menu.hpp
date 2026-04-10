#ifndef BINARYMENU_HPP
#define BINARYMENU_HPP

#include <termui/ui/base/padded-text.hpp>
#include <termui/ui/display/box.hpp>
#include <termui/ui/interfaces/interface.hpp>
#include <termui/ui/widgets/button.hpp>

namespace termui {

class BinaryMenu : public Interface {
public:
  BinaryMenu(const termui::string &title, const termui::string &text, const termui::string &affirmative, const termui::string &negative);

  bool show(); // t = affirmative, f = negative

private:
  termui::string title, text, affirmative, negative;

  Text        title_label;
  Text        body;
  Button      aff, neg;
  Box         box;
  static Text control_strip;

  size_t w, h;
  size_t aff_x1, aff_x2, aff_y; // these values hold bounds for the buttons
  size_t neg_x1, neg_x2, neg_y; // which is used to detect if 'pressed'
  bool   selection;             // t = affirmative selected, f = negative selected
  bool   reprint;               // flag indicates if reprint is required

  static StyleMap styles;

  void display();
  void process_input();
  void update_size();
};

} // namespace termui

#endif
