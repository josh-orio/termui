#include "include/elements.hpp"
#include "include/interfaces.hpp"
#include "include/str.hpp"
#include "include/termui.hpp"
#include "include/util.hpp"
#include <ostream>
#include <string>
#include <vector>

int main() {
  // Elements examples

  // Text
  termui::string example("some text in a shared string");
  termui::Text text(example, 10, 1);
  termui::Text text2(example, 10, 1, termui::clr::BLUE, termui::clr::DARKGREY);

  // Input
  termui::string placeholder("placeholder...");
  termui::string response;
  termui::Input input(response, placeholder, 20);

  // Button
  termui::string click("click me!");
  termui::Button button(click, 10, termui::clr::MAGENTA, termui::clr::DARKGREY);

  // Lists
  termui::strings people({"Peter", "Lois", "Chris", "Meg", "Stewie", "Brian"});
  termui::strings bio({"Father", "Mother", "Eldest Son", "Daughter", "Youngest Son", "Dog"});
  termui::List list1(people, 20, 5);
  termui::SelectList list2(people, 20, 5);
  termui::FancyList list3(people, bio, 20, 5);

  // Table
  termui::Table table(termui::strings({"Key", "Value"}), {termui::strings({"1", "One"}), termui::strings({"2", "Two"})}, {2, 2}, 10);

  // Decorators
  termui::Box box(10, 10, termui::clr::RED);
  termui::ProgressBar bar(10, 0.5, termui::clr::RED, termui::clr::DARKGREY);
  termui::DashedArea dash(10, 10, termui::clr::RED);

  // Interface examples

  // Info displays
  termui::string info_title("Info"), info("this is some information.");
  termui::InfoBox(info_title, info);
  termui::InfoPage(info_title, info);

  // Input methods
  termui::string input_title("Input"), input_response(""), input_placeholder("Start typing...");
  termui::strings input_fields(3, "Some value"), input_responses(3, "");
  termui::InputBox(input_title, input_response, input_placeholder);
  termui::InputPage(input_title, input_fields, input_responses);

  // List interfaces
  // reuses family guy strings
  termui::Menu(termui::string("Menu"), people);
  termui::MultiMenu(termui::string(" Multi Menu"), people);
  termui::FancyMenu(termui::string(" Multi Menu"), people, bio);
  termui::BinaryMenu("Question", "Do you like family guy?", "Yes", "No");

  // Text editor
  termui::string some_text_file("");
  termui::Editor("VSCode Lite", some_text_file);

  // Spreadsheet
  termui::Spreadsheet("Q2 earnings report", termui::strings({"Idk what an earnings report looks like", "\% Change"}),
                      {termui::strings({"Something", "1.54%"}), termui::strings({"Something else", "0.67%"})});

  return 0;
}