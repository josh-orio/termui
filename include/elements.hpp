#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include "util.hpp"
#include <string>

/* UI elements */

// elements should not be aware of position in terminal, only of their own properties such as width and height
// using absolute cursor positioning is prohibited
// using relative cursor positioning is encouraged

namespace termui {

class Text {
public:
  std::string data;
  int w, h;
  int fg_col, bg_col;

  Text(std::string data, int w, int h, int fg_col, int bg_col);

  std::string render();
};

class Input {
public:
  std::string field, value, placeholder;
  int ftc;
  bool active, vertical;
  int max_w;

  Input(std::string field, std::string &value, std::string placeholder, int ftc /* field text color */, bool vertical);

  std::string render();
};

class Box {
public:
  int w, h;
  int color;

  Box(int w, int h, int color);

  std::string render();
};

class Button {
public:
  int w;
  bool selected;
  std::string text;
  int active_color, idle_color;

  Button(int w, std::string text, int active_color, int idle_color);

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

class Select { /* one shot selection */
public:
  std::vector<std::string> elements;
  int w, h;
  int cursor;
  int line_spacing;
  int active_color;

  Select(std::vector<std::string> elements, int w, int h, int ls = 0);

  std::string render();

  void cursor_up();
  void cursor_down();

private:
  int visible_lines;
  int start_line;

  void internal_update();
};

class MultiSelect { /* multi shot selection */
public:
  std::vector<std::string> elements;
  std::vector<bool> selection_map;
  int w, h;
  int cursor;
  int line_spacing;
  int active_color;

  MultiSelect(std::vector<std::string> elements, int w, int h, int ls = 0);

  std::string render();

  void cursor_up();
  void cursor_down();
  void select(); // toggle

private:
  int visible_lines;
  int start_line;

  void internal_update();
};

class FancySelect {
public:
  std::vector<std::string> elements, desc;
  int w, h;
  int cursor;
  int line_spacing;
  int active_color;

  FancySelect(std::vector<std::string> elements, std::vector<std::string> desc, int w, int h, int ls = 1);

  std::string render();

  void cursor_up();
  void cursor_down();

private:
  int visible_rows;
  int start_line;

  void internal_update();
};

class Table {
public:
  std::vector<std::string> columns;
  std::vector<int> column_widths;
  std::vector<std::vector<std::string>> cells; // organized as row major

  int cursor;
  int line_seperation; // number of blank lines between rows
  int cell_height;     // number of lines to a cell
  int active_color;    // background of selected row
  int box_color;       // color of table borders

  Table(std::vector<std::string> columns, std::vector<std::vector<std::string>> cells, int table_height,
        std::vector<int> column_widths, int cell_height = 1, int line_seperation = 0);

  std::string render();

  void cursor_up();
  void cursor_down();

private:
  int visible_rows; // number of table rows that fit the h restraint
  int start_line;   // index value of first visible row
  int overhead;     // number of lines reserved for header & footer
  int table_width;
  int table_height;

  void internal_update();
};

} // namespace termui

#endif
