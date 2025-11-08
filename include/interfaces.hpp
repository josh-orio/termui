#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#include "console.hpp"
#include "elements.hpp"
#include "util.hpp"
#include <algorithm>
#include <memory>
#include <vector>

namespace termui {

enum class state { EXIT, CONTINUE, SELECT };

class InfoBox {
public:
  termui::string title, content;
  int w, h;

  InfoBox(const termui::string &t, const termui::string &c);
  InfoBox(const std::string &t, const std::string &c);

  void show();

private:
  Console cons;
  termui::state state;

  void display();
  void process_input();
  void update_size();
};

class InfoPage {
public:
  termui::string title, content;

  InfoPage(const termui::string &t, const termui::string &c);
  InfoPage(const std::string &t, const std::string &c);

  void show();

private:
  Console cons;
  termui::state state;

  int line_cursor;   // current scroll value
  int text_width;    // width of text accounting for padding
  int content_lines; // total number of lines of content
  int visible_lines; // total number of lines in view
  int voh;           // vertical overhead (3 header + 2 footer)
  int hoh;           // horizontal overhead (2 left + 2 right)
  bool reprint;      // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

class InputBox {
public:
  termui::string field, response, placeholder;
  int w, h;

  InputBox(const termui::string &field, const termui::string &response, const termui::string &placeholder);
  InputBox(const std::string &field, const termui::string &response, const std::string &placeholder);

  std::shared_ptr<std::string> shareField() const;

  const std::string &getField() const;
  std::string &getField();

  std::shared_ptr<std::string> shareResponse() const;

  const std::string &getResponse() const;
  std::string &getResponse();

  void show();

private:
  Console cons;
  termui::state state;

  bool reprint; // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

class InputPage {
public:
  termui::string title;
  termui::strings fields, responses;

  int col;

  InputPage(const termui::string &title, const termui::strings &fields, const termui::strings &responses, int ls = 0, int col = clr::PASTELPINK);
  InputPage(const std::string &title, const termui::strings &fields, const termui::strings &responses, int ls = 0, int col = clr::PASTELPINK);
  InputPage(const std::string &title, const std::vector<std::string> &fields, const termui::strings &responses, int ls = 0, int col = clr::PASTELPINK);

  // std::shared_ptr<std::vector<std::string>> shareFields() const;

  // const std::vector<std::string> &getFields() const;
  // std::vector<std::string> &getFields();

  // std::shared_ptr<std::vector<std::string>> shareResponses() const;

  // const std::vector<std::string> &getResponses() const;
  // std::vector<std::string> &getResponses();

  void show();

private:
  Console cons;
  termui::state state;

  int cursor;          // current scroll value
  bool selected;       // element selection indicator
  int visible_lines;   // total number of lines in view
  int start_line;      // index value of first line which shows in view
  int line_seperation; // number of blank lines between elements
  int voh;             // vertical overhead (3 header + 2 footer)
  bool reprint;        // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

class Menu {
public:
  termui::string title;
  termui::strings elements;

  int line_seperation;

  Menu(const termui::string &t, const termui::strings &e, int ls = 0);
  Menu(const std::string &t, const termui::strings &e, int ls = 0);
  Menu(const std::string &t, const std::vector<std::string> &e, int ls = 0);

  // std::shared_ptr<List> shareList() const { return list; }

  // const std::vector<std::shared_ptr<item::ListItem>> &getElements() const { return (*list).getElements(); }
  // std::vector<std::shared_ptr<item::ListItem>> &getElements() { return (*list).getElements(); }

  int show();             // -1 = exit, >=0 = element selected at index
  int cursor();           // returns cursor
  termui::state status(); // when the interface closes, it will either be EXIT or SELECT

private:
  Console cons;
  List list;
  termui::state state;

  int lvo;      // list vertical overhead
  int lho;      // list horizontal overhead
  bool reprint; // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

class MultiMenu {
public:
  termui::string title;
  termui::strings elements;
  int line_seperation;

  MultiMenu(const termui::string &t, const termui::strings &e, int ls = 0);
  MultiMenu(const std::string &t, const termui::strings &e, int ls = 0);
  MultiMenu(const std::string &t, const std::vector<std::string> &e, int ls = 0);

  bool isSelected(int i);

  int show();             // -1 = exit, >=0 = element selected at index
  int cursor();           // returns cursor
  termui::state status(); // when the interface closes, it will either be EXIT or SELECT

private:
  Console cons;
  SelectList list;
  termui::state state;

  int lvo;      // list vertical overhead
  int lho;      // list horizontal overhead
  bool reprint; // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

class FancyMenu {
public:
  termui::string title;
  termui::strings text, desc;
  int line_seperation;

  FancyMenu(const termui::string &title, const termui::strings &texts, const termui::strings &descs, int ls = 1);
  FancyMenu(const std::string &title, const termui::strings &texts, const termui::strings &descs, int ls = 1);
  FancyMenu(const std::string &title, const std::vector<std::string> &texts, const std::vector<std::string> &descs, int ls = 1);

  int show();             // -1 = exit, >=0 = element selected at index
  int cursor();           // returns cursor
  termui::state status(); // when the interface closes, it will either be EXIT or SELECT

private:
  Console cons;
  FancyList list;
  termui::state state;

  int lvo;      // list vertical overhead
  int lho;      // list horizontal overhead
  bool reprint; // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

class BinaryMenu {
public:
  BinaryMenu(const termui::string &title, const termui::string &text, const termui::string &affirmative, const termui::string &negative);
  BinaryMenu(const std::string &title, const std::string &text, const std::string &affirmative, const std::string &negative);

  bool show(); // t = affirmative, f = negative

private:
  Console cons;
  termui::state state;

  bool selection; // t = affirmative selected, f = negative selected
  bool reprint;   // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();

  termui::string title, text, affirmative, negative;
  size_t w, h;
  size_t aff_x1, aff_x2, aff_y; // these values hold bounds for the buttons
  size_t neg_x1, neg_x2, neg_y; // which is used to detect if 'pressed'
};

class Editor {
public:
  termui::string title, content;

  Editor(const termui::string &title, const termui::string &content);
  Editor(const std::string &title, const termui::string &content);

  void show();

private:
  Console cons;
  termui::state state;

  int line_cursor;   // current scroll value
  int text_width;    // width of text accounting for padding
  int visible_lines; // total number of lines in view
  int voh;           // vertical overhead
  int hoh;           // horizontal overhead
  bool reprint;      // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

class Spreadsheet {
public:
  termui::string title;
  Table table;

  Spreadsheet(const termui::string &title, const termui::strings &cols, const std::vector<termui::strings> &data);
  Spreadsheet(const std::string &title, const termui::strings &cols, const std::vector<termui::strings> &data);
  Spreadsheet(const std::string &title, const std::vector<std::string> &cols, const std::vector<termui::strings> &data);

  void show();

private:
  Console cons;
  termui::state state;

  int voh;      // vertical overhead
  int hoh;      // horizontal overhead
  bool reprint; // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

} // namespace termui

#endif
