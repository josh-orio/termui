#ifndef INPUTBOX_HPP
#define INPUTBOX_HPP

#include <termui/ui/base/input.hpp>
#include <termui/ui/base/padded-text.hpp>
#include <termui/ui/display/box.hpp>
#include <termui/ui/interfaces/interface.hpp>

namespace termui {
class InputBox : public Interface {
public:
  InputBox(const termui::string &field, const termui::string &response, const termui::string &placeholder);

  void show();

private:
  termui::string field, response, placeholder;

  Text            header;
  Input           input_field;
  static Text     control_strip;
  Box             box;
  static StyleMap styles;

  uint w, h;
  bool reprint; // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

} // namespace termui

#endif
