#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include "structs.hpp"
#include "unicode.hpp"
#include "util.hpp"
#include <memory>
#include <string>
#include <vector>

namespace termui {

class Text {
private:
  std::shared_ptr<std::string> text;

public:
  int w, h;
  int fg_col, bg_col;

  Text();
  Text(const std::string &rT, int w, int h, int fg_col, int bg_col);
  Text(std::string &&lT, int w, int h, int fg_col, int bg_col);
  Text(std::shared_ptr<std::string> sharedT, int w, int h, int fg_col, int bg_col);

  std::shared_ptr<std::string> share() const; // share function shares a pointer to the content for this class only text needs to be shared, but more advanced
                                              // classes may need multiple share function

  const std::string &getText() const;
  std::string &getText();

  std::string render();
};

class Input {
private:
  std::shared_ptr<std::string> val, plh; // value, placeholder

public:
  int w;
  int active_col, idle_col;

  Input();
  Input(const std::string &rVal, const std::string &rPlh, int w, int valCol, int plhCol);
  Input(const std::string &lVal, std::string &&lPlh, int w, int valCol, int plhCol);
  Input(std::shared_ptr<std::string> sharedVal, std::shared_ptr<std::string> sharedPlh, int w, int valCol, int plhCol);
  Input(std::shared_ptr<std::string> sharedVal, std::string &&lPlh, int w, int valCol, int plhCol);

  const std::string &getVal() const;
  std::string &getVal();

  const std::string &getPlh() const;
  std::string &getPlh();

  std::string render();
};

class Button {
private:
  std::shared_ptr<std::string> text;

public:
  int w;
  bool selected;
  int active_color, idle_color;

  Button();
  Button(const std::string &rT, int w, int active_color, int idle_color);
  Button(std::string &&lT, int w, int active_color, int idle_color);
  Button(std::shared_ptr<std::string> sharedT, int w, int active_color, int idle_color);

  const std::string &getText() const;
  std::string &getText();

  std::string render();
};

class List {
private:
  std::shared_ptr<std::vector<item::ListItem>> elements;

  int visible_lines;
  int start_line;

  void internal_update();

public:
  int w, h;
  int cursor;
  int line_spacing;
  int active_color;

  List();
  List(const std::vector<item::ListItem> &rE, int w, int h, int ls = 0, int col = clr::PASTELPINK);
  List(std::vector<item::ListItem> &&lE, int w, int h, int ls = 0, int col = clr::PASTELPINK);
  List(std::shared_ptr<std::vector<item::ListItem>> sharedE, int w, int h, int ls = 0, int col = clr::PASTELPINK);

  List(std::vector<std::string> &lStrs, int w, int h, int ls = 0, int col = clr::PASTELPINK);
  List(std::vector<std::string> &&rStrs, int w, int h, int ls = 0, int col = clr::PASTELPINK);
  List(std::shared_ptr<std::vector<std::string>> sharedStrs, int w, int h, int ls = 0, int col = clr::PASTELPINK);

  std::shared_ptr<std::vector<item::ListItem>> share() const;

  const std::vector<item::ListItem> &getElements() const;
  std::vector<item::ListItem> &getElements();

  std::string render();

  void cursor_up();
  void cursor_down();
};

class SelectList {
private:
  std::shared_ptr<std::vector<item::MultiListItem>> elements;

  int visible_lines;
  int start_line;

  void internal_update();

public:
  int w, h;
  int cursor;
  int line_spacing;
  int active_color;

  SelectList();
  SelectList(const std::vector<item::MultiListItem> &e, int w, int h, int ls = 0, int col = clr::PASTELPINK);
  SelectList(std::vector<item::MultiListItem> &&e, int w, int h, int ls = 0, int col = clr::PASTELPINK);
  SelectList(std::shared_ptr<std::vector<item::MultiListItem>> shared, int w, int h, int ls = 0, int col = clr::PASTELPINK);
  SelectList(std::vector<std::string> &lStrs, int w, int h, int ls = 0, int col = clr::PASTELPINK);
  SelectList(std::vector<std::string> &&rStrs, int w, int h, int ls = 0, int col = clr::PASTELPINK);
  SelectList(std::shared_ptr<std::vector<std::string>> sharedStrs, int w, int h, int ls = 0, int col = clr::PASTELPINK);

  std::shared_ptr<std::vector<item::MultiListItem>> share() const;

  const std::vector<item::MultiListItem> &getElements() const;
  std::vector<item::MultiListItem> &getElements();

  std::string render();

  void cursor_up();
  void cursor_down();
  void select(); // toggle selection on current element
};

class FancyList {
private:
  std::shared_ptr<std::vector<item::FancyListItem>> elements;

  int visible_rows;
  int start_line;

  void internal_update();

public:
  int w, h;
  int cursor;
  int line_spacing;
  int active_color;

  FancyList();
  FancyList(std::vector<item::FancyListItem> &e, int w, int h, int ls = 1, int col = clr::PASTELPINK);
  FancyList(std::vector<item::FancyListItem> &&e, int w, int h, int ls = 1, int col = clr::PASTELPINK);
  FancyList(std::shared_ptr<std::vector<item::FancyListItem>> shared, int w, int h, int ls = 1, int col = clr::PASTELPINK);
  FancyList(std::vector<std::string> &e, std::vector<std::string> &d, int w, int h, int ls = 1, int col = clr::PASTELPINK);
  FancyList(std::vector<std::string> &&e, std::vector<std::string> &&d, int w, int h, int ls = 1, int col = clr::PASTELPINK);
  FancyList(std::shared_ptr<std::vector<std::string>> e, std::shared_ptr<std::vector<std::string>> d, int w, int h, int ls = 1, int col = clr::PASTELPINK);

  std::shared_ptr<std::vector<item::FancyListItem>> share() const;

  const std::vector<item::FancyListItem> &getElements() const;
  std::vector<item::FancyListItem> &getElements();

  std::string render();

  void cursor_up();
  void cursor_down();
};

class Table {
public:
  std::shared_ptr<std::vector<item::TableColumn>> columns;
  std::shared_ptr<std::vector<item::TableRow>> rows;

  int cursor;
  int line_seperation; // number of blank lines between rows
  int cell_height;     // number of lines to a cell
  int active_color;    // background of selected row
  int box_color;       // color of table borders

  Table();
  Table(const std::vector<item::TableColumn> &cols, const std::vector<item::TableRow> &rs, int table_height, int cell_height = 1, int line_seperation = 0,
        int col = clr::BLUEPURPLE);
  Table(std::vector<item::TableColumn> &&cols, std::vector<item::TableRow> &&rs, int table_height, int cell_height = 1, int line_seperation = 0,
        int col = clr::BLUEPURPLE);
  Table(std::shared_ptr<std::vector<item::TableColumn>> cols, std::shared_ptr<std::vector<item::TableRow>> rs, int table_height, int cell_height = 1,
        int line_seperation = 0, int col = clr::BLUEPURPLE);

  std::string render();

  void cursor_up();
  void cursor_down();

  item::TableColumn &getCol(int i);

private:
  int visible_rows; // number of table rows that fit the h restraint
  int start_line;   // index value of first visible row
  int overhead;     // number of lines reserved for header & footer
  int table_height;
  int table_width;

  void internal_update();
};

class Box {
public:
  int w, h;
  int color;

  Box(int w, int h, int color);

  std::string render();
};

class ProgressBar {
public:
  int width;
  float completion;
  int fg_col, bg_col;

  ProgressBar(int width, float completion, int fg_col, int bg_col);

  std::string render();
};

class DashedArea {
public:
  int w, h;
  int col;

  DashedArea(int w, int h, int col);

  std::string render();
};

class Spinner {
public:
  Spinner();

  std::string render();

private:
  const std::string symbols = "⠋⠙⠸⠴⠦⠇";
};

} // namespace termui

#endif
