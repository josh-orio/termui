/* UI elements */

#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include "util.hpp"
#include <iostream>
#include <string>

// elements should not be aware of position in terminal, only of their own properties such as width and height
// using absolute cursor positioning is prohibited
// using relative cursor positioning is encouraged

namespace termui {

class Box {
public:
  int w, h;
  std::string color;

  Box(int w, int h, std::string color);

  std::string render();
};

class Button {
public:
  int w;
  bool selected;
  std::string text;
  std::string active_color, idle_color;

  Button(int w, std::string text, std::string active_color, std::string idle_color);

  std::string render();
};

class ProgressBar {
public:
  int width;
  float completion;
  std::string fg_col, bg_col;

  ProgressBar(int width, float completion, std::string fg_col, std::string bg_col);

  std::string render();
};

class Text {
public:
  std::string data;
  int w, h;
  std::string fg_col, bg_col;

  Text(std::string data, int w, int h, std::string fg_col, std::string bg_col);

  std::string render();
};

class Table {
public:
  std::vector<std::string> columns;
  std::vector<int> column_widths;
  std::vector<std::vector<std::string>> cells; // organized as row major

  int cursor;          // current scroll value
  int visible_rows;    // number of table rows that will fit onto the terminal
  int start_line;      // index value of first row which shows in view
  int table_width;     // width of table in characters
  int table_height;    // height of table
  int line_seperation; // number of blank lines between elements
  int cell_height;     // number of lines dedicated to a cell
  int overhead;        // number of lines reserved for header & footer
  bool reprint;

  std::string row_color;

  Table(std::vector<std::string> columns, std::vector<int> column_widths, std::vector<std::vector<std::string>> cells);

  std::string render();
};

} // namespace termui

#endif