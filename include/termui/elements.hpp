#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include <algorithm>
#include <memory>
#include <string>
#include <termui/str.hpp>
#include <termui/util.hpp>
#include <vector>

namespace termui {

class Text {
private:
  termui::string text;

public:
  int w, h;
  int fg_col, bg_col;

  Text();
  Text(const termui::string &t, int w, int h, int fg_col = clr::DEFAULT, int bg_col = clr::DEFAULT);
  Text(const std::string &t, int w, int h, int fg_col = clr::DEFAULT, int bg_col = clr::DEFAULT);
  Text(std::shared_ptr<std::string> &t, int w, int h, int fg_col = clr::DEFAULT, int bg_col = clr::DEFAULT);

  std::shared_ptr<std::string> share() const;

  const std::string &getText() const;
  std::string &getText();

  std::string render();
};

class Input {
private:
  termui::string val, plh; // value, placeholder

public:
  int w, h;
  int valCol, plhCol; // color codes for value and placeholder text

  Input();
  Input(const termui::string &val, const termui::string &plh, int w, int h, int val_color = clr::DEFAULT, int placeholder_color = clr::LIGHTGREY);
  Input(const termui::string &val, const std::string &plh, int w, int h, int val_color = clr::DEFAULT, int placeholder_color = clr::LIGHTGREY);
  Input(const termui::string &val, int w, int h, int val_color = clr::DEFAULT);

  // ctors:
  // with placeholder
  // std str placeholder
  // no placeholder

  std::shared_ptr<std::string> shareVal() const;
  std::shared_ptr<std::string> sharePlh() const;

  const std::string &getVal() const;
  std::string &getVal();

  const std::string &getPlh() const;
  std::string &getPlh();

  std::string render();
};

class Button {
private:
  termui::string text;

public:
  int w;
  bool selected;
  int active_color, idle_color;

  Button();
  Button(const termui::string &t, int w, int active_color, int idle_color);
  Button(const std::string &t, int w, int active_color, int idle_color);

  std::shared_ptr<std::string> shareText() const;

  const std::string &getText() const;
  std::string &getText();

  std::string render();
};

class List {
private:
  termui::strings elements;

  int visible_lines;
  int start_line;

  void internal_update();

public:
  int w, h;
  int cursor;
  int line_spacing;
  int active_color;

  List(const termui::strings &strs, int w, int h, int ls = 0, int col = clr::PASTELPINK);

  const termui::strings &getStrings() const { return elements; }
  termui::strings &getStrings() { return elements; }

  const std::string &getElement(int i) const { return elements.getItem(i); }
  std::string &getElement(int i) { return elements.getItem(i); }

  std::string render();

  void cursor_up();
  void cursor_down();
};

class SelectList {
private:
  termui::strings elements;
  std::vector<bool> selmap;

  int visible_lines;
  int start_line;

  void internal_update();

public:
  int w, h;
  int cursor;
  int line_spacing;
  int active_color;

  SelectList(const termui::strings &strs, int w, int h, int ls = 0, int col = clr::PASTELPINK);

  const termui::strings &getStrings() const;
  termui::strings &getStrings();

  const std::string &getElement(int i) const;
  std::string &getElement(int i);

  bool getSelection(int i);

  std::string render();

  void cursor_up();
  void cursor_down();
  void select(); // toggle selection on current element
};

class FancyList {
private:
  termui::strings text, desc;

  int visible_rows;
  int start_line;

  void internal_update();

public:
  int w, h;
  int cursor;
  int line_spacing;
  int active_color;

  FancyList(const termui::strings &tstrs, const termui::strings &dstrs, int w, int h, int ls = 1, int col = clr::PASTELPINK);

  const std::string &getText(int i) const;
  std::string &getText(int i);

  const std::string &getDesc(int i) const;
  std::string &getDesc(int i);

  std::string render();

  void cursor_up();
  void cursor_down();
};

class Table {
private:
  termui::strings cols;
  std::vector<int> colwidths;
  std::vector<termui::strings> rows;

public:
  int cursor;
  int line_seperation; // number of blank lines between rows
  int cell_height;     // number of lines to a cell
  int active_color;    // background of selected row
  int table_height;
  int table_width;

  Table(const termui::strings &cols, const std::vector<termui::strings> &rows, std::vector<int> colwidths, int table_height, int cell_height = 1,
        int line_seperation = 0, int col = clr::BLUEPURPLE);

  const termui::strings &getCols() const;
  termui::strings &getCols();

  // get colw
  const int &colWidth(int i) const;
  int &colWidth(int i);

  // get row
  const termui::strings &getRow(int i) const;
  termui::strings &getRow(int i);

  // get cell by x y
  const std::string &getCell(int row, int col) const;
  std::string &getCell(int row, int col);

  // dimensions
  int colCount();
  int rowCount();

  std::string render();

  void cursor_up();
  void cursor_down();

private:
  int visible_rows; // number of table rows that fit the h restraint
  int start_line;   // index value of first visible row
  int overhead;     // number of lines reserved for header & footer
  int box_color;    // color of table borders

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

} // namespace termui

#endif
