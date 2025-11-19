#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <ansi.hpp>

namespace py = pybind11;

void bind_ansi(py::module_ &m) {
  // format
  auto fmt = m.def_submodule("format");
  fmt.attr("NONE") = termui::format::NONE;
  fmt.attr("BOLD") = termui::format::BOLD;
  fmt.attr("FAINT") = termui::format::FAINT;
  fmt.attr("ITALIC") = termui::format::ITALIC;
  fmt.attr("UNDERLINE") = termui::format::UNDERLINE;
  fmt.attr("SLOW_BLINK") = termui::format::SLOW_BLINK;
  fmt.attr("RAPID_BLINK") = termui::format::RAPID_BLINK;
  fmt.attr("REVERSE_VIDEO") = termui::format::REVERSE_VIDEO;
  fmt.attr("CONCEAL") = termui::format::CONCEAL;
  fmt.attr("CROSSED_OUT") = termui::format::CROSSED_OUT;
  fmt.attr("BOLD_OFF") = termui::format::BOLD_OFF;
  fmt.attr("FAINT_OFF") = termui::format::FAINT_OFF;
  fmt.attr("ITALIC_OFF") = termui::format::ITALIC_OFF;
  fmt.attr("UNDERLINE_OFF") = termui::format::UNDERLINE_OFF;
  fmt.attr("BLINK_OFF") = termui::format::BLINK_OFF;
  fmt.attr("REVERSE_VIDEO_OFF") = termui::format::REVERSE_VIDEO_OFF;
  fmt.attr("CONCEAL_OFF") = termui::format::CONCEAL_OFF;
  fmt.attr("CROSSED_OUT_OFF") = termui::format::CROSSED_OUT_OFF;
  fmt.attr("FG_DEFAULT") = termui::format::FG_DEFAULT;
  fmt.attr("BG_DEFAULT") = termui::format::BG_DEFAULT;

  // term
  auto term = m.def_submodule("term");
  term.attr("CLEAR_CONSOLE") = termui::term::CLEAR_CONSOLE;
  term.attr("CLEAR_SCROLLBACK") = termui::term::CLEAR_SCROLLBACK;
  term.attr("CURSOR_HOME") = termui::term::CURSOR_HOME;
  term.attr("HIDE_CURSOR") = termui::term::HIDE_CURSOR;
  term.attr("SHOW_CURSOR") = termui::term::SHOW_CURSOR;
  term.attr("ALT_BUFFER") = termui::term::ALT_BUFFER;
  term.attr("PRIMARY_BUFFER") = termui::term::PRIMARY_BUFFER;
  term.attr("SAVE_CURSOR") = termui::term::SAVE_CURSOR;
  term.attr("RESTORE_CURSOR") = termui::term::RESTORE_CURSOR;
  term.attr("ENABLE_MOUSE_REPORTING") = termui::term::ENABLE_MOUSE_REPORTING;
  term.attr("DISABLE_MOUSE_REPORTING") = termui::term::DISABLE_MOUSE_REPORTING;
}
