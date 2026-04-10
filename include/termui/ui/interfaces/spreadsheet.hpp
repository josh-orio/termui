#ifndef SPREADSHEET_HPP
#define SPREADSHEET_HPP

#include <termui/ui/base/padded-text.hpp>
#include <termui/ui/interfaces/interface.hpp>
#include <termui/ui/widgets/table.hpp>

namespace termui {

class Spreadsheet : public Interface {
public:
  Spreadsheet(const termui::string &title, const termui::strings &cols, const std::vector<termui::strings> &data);

  void show();

private:
  termui::string title;

  PaddedText      title_banner;
  Table           table;
  static Text     control_strip;
  static StyleMap styles;

  const int voh = 5; // vertical overhead
  const int hoh = 2; // horizontal overhead
  bool      reprint; // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

} // namespace termui

#endif
