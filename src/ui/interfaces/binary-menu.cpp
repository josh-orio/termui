#include <chrono>
#include <termui/ui/ui.hpp>
#include <thread>

namespace termui {

// clang-format off
StyleMap BinaryMenu::styles({
  {"title", Style(57, std::nullopt, {SGR::Bold})},
  {"focus", Style(std::nullopt, 219, {SGR::Bold})},
  {"blur", Style(std::nullopt, 238)},
  {"box", Styles::faint}
});
// clang-format on

Text BinaryMenu::control_strip(std::format("[{}/{}] move [{}] select", unicode::L_ARROW, unicode::R_ARROW, unicode::ENTER), Styles::faint);

BinaryMenu::BinaryMenu(const termui::string &t, const termui::string &tx, const termui::string &affirmative, const termui::string &negative)
  : Interface(TermSetups::fullscreen),
    title(t),
    text(tx),
    affirmative(affirmative),
    negative(negative),
    title_label(title, styles.at("title")),
    body(text),
    aff(affirmative, styles.at("focus"), styles.at("blur")),
    neg(negative, styles.at("focus"), styles.at("blur")),
    box(0, 0, Borders::rounded, styles.at("box")),
    selection(true) {}

bool BinaryMenu::show() {
  term_setup.configure();

  state = State::CONTINUE;
  reprint = true;

  do {
    if (reprint) {
      display();
      reprint = false;
    }
    process_input();

  } while (state == State::CONTINUE);

  term_setup.reset();
  return selection;
}

void BinaryMenu::display() {
  update_size();

  terminal.ClearScreen();
  terminal.ClearScrollback();

  uint origin_row = (terminal.height() - static_cast<uint>(h)) / 2, origin_col = (terminal.width() - static_cast<uint>(w)) / 2;

  terminal.write(origin_row, origin_col, box.render());
  terminal.write(origin_row + 1, origin_col + 2, title_label.render());
  terminal.write(origin_row + 3, origin_col + 2, body.render());
  terminal.write(aff_y, aff_x1, aff.render());
  terminal.write(neg_y, neg_x1, neg.render());
  terminal.write(terminal.height(), 2, control_strip.render());
  terminal.flush();
}

void BinaryMenu::process_input() {
  std::string ec = terminal.read();

  if (ec == key::ENTER) {
    state = State::SELECT;

  } else if (ec == key::L_ARROW) {
    if (!selection) { // avoids reprint if status already == t
      selection = true;
      reprint = true;
    }

  } else if (ec == key::R_ARROW) {
    if (selection) { // avoids reprint if status already == f
      selection = false;
      reprint = true;
    }

  } else if (MouseInteraction(ec).match(aff_x1, aff_x2, aff_y, aff_y, EventType::ButtonPress, MouseButton::Left)) {
    std::string release = terminal.read(); // check for mouse release code

    if (MouseInteraction(release).match(aff_x1, aff_x2, aff_y, aff_y, EventType::ButtonRelease, MouseButton::Left)) {
      selection = true; // ensure pressed button is selected
      state = Interface::State::SELECT;

      display();                                                   // redraw incase button selection has changed
      std::this_thread::sleep_for(std::chrono::milliseconds(100)); // brief pause for visual feedback
    }

  } else if (MouseInteraction(ec).match(neg_x1, neg_x2, neg_y, neg_y, EventType::ButtonPress, MouseButton::Left)) {
    std::string release = terminal.read();

    if (MouseInteraction(release).match(neg_x1, neg_x2, neg_y, neg_y, EventType::ButtonRelease, MouseButton::Left)) {
      selection = false;
      state = Interface::State::SELECT;

      display();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
}

void BinaryMenu::update_size() {
  w = 0.25 * terminal.width();
  h = 8;

  // resize title banner
  title_label.width(w - 4).height(1);

  // resize main text
  body.width(w - 4).height(2);

  // set button widths
  aff.width(8);
  neg.width(8);

  // calculate button positions
  aff_x1 = ((terminal.width() - w) / 2) + w - (8 + 8 + 3);
  aff_x2 = aff_x1 + 8 - 1; // x2 should == x1+7 in this case, because that will display as 8 cols
  aff_y = ((terminal.height() - h) / 2) + h - 2;

  neg_x1 = ((terminal.width() - w) / 2) + w - (8 + 2);
  neg_x2 = neg_x1 + 8 - 1;
  neg_y = ((terminal.height() - h) / 2) + h - 2;

  if (selection) {
    aff.focus();
    neg.blur();

  } else /* status == f */ {
    aff.blur();
    neg.focus();
  }

  // resize boundary box
  box.resize(w, h);

  // resize control strip
  control_strip.width(terminal.width() - 2).height(1);
}

} // namespace termui
