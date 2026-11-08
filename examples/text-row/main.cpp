#include <termui/termui.hpp>

int main() {
  termui::Style name(termui::Colors::Cyan, termui::Color::Inherit(), {termui::SGR::Bold});
  termui::Style hash(termui::Colors::Orange, termui::Color::Inherit(), {termui::SGR::Bold});
  termui::Style date(termui::Colors::Lavender, termui::Color::Inherit(), {termui::SGR::Italic});

  termui::terminal.ClearScreen();

  // the same row at shrinking widths: the segment that runs out of room is cut, the date keeps its reserved 20
  int widths[] = {70, 50, 35, 22};

  for (int i = 0; i < 4; i++) {
    termui::terminal.MoveCursor(2 + i * 2, 3);

    termui::TextRow(widths[i])
        .add(termui::Text("User A\nAAA", name))
        .add(termui::Text("sent a message to", termui::Styles::faint))
        .add(termui::Text("User B", hash))
        .add(termui::Text("Jan 1, 2000 00:00", date).align(termui::Text::Alignment::Right), 20)
        .render();
  }

  // the same rows 2 tall: each segment is a column that wraps within its width before it is cut
  for (int i = 0; i < 4; i++) {
    termui::terminal.MoveCursor(11 + i * 3, 3);

    termui::TextRow(widths[i], 2)
        .add(termui::Text("User A\nAAA", name))
        .add(termui::Text("sent a message to", termui::Styles::faint))
        .add(termui::Text("User B", hash))
        .add(termui::Text("Jan 1, 2000 00:00", date).align(termui::Text::Alignment::Right), 20)
        .render();
  }

  termui::terminal.flush();

  termui::terminal.read();

  return 0;
}
