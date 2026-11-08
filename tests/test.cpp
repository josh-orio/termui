#include <gtest/gtest.h>
#include <termui/termui.hpp>

#include <fcntl.h>
#include <string>
#include <unistd.h>

static std::string strip_csi(const std::string &raw) {
  std::string text;

  for (size_t i = 0; i < raw.size(); i++) {
    if (raw[i] == '\x1b' && i + 1 < raw.size() && raw[i + 1] == '[') {
      i += 2;
      while (i < raw.size() && !(raw[i] >= 0x40 && raw[i] <= 0x7E)) {
        i++;
      }
      continue;
    }
    if (raw[i] == '\x1b' && i + 1 < raw.size()) { // two byte escapes, like ESC 7 / ESC 8 (save / restore cursor)
      i++;
      continue;
    }
    text += raw[i];
  }

  return text;
}

// renders a Text, returning what it wrote with all CSI sequences removed
static std::string rendered_text(termui::Text text) {
  testing::internal::CaptureStdout();
  text.render();
  termui::terminal.flush();

  return strip_csi(testing::internal::GetCapturedStdout());
}

TEST(text, PadsByVisibleWidth) {
  using Alignment = termui::Text::Alignment;

  // 9 bytes but only 3 chars wide, so it needs padding even though size() >= w
  EXPECT_EQ(rendered_text(termui::Text("日本語", {}, 5, 1)), "日本語  ");
  EXPECT_EQ(rendered_text(termui::Text("日本語", {}, 5, 1).align(Alignment::Right)), "  日本語");
  EXPECT_EQ(rendered_text(termui::Text("日本語", {}, 5, 1).align(Alignment::Center)), " 日本語 ");
}

TEST(text, ExtraLinesAreTruncatedWithEllipsis) {
  EXPECT_EQ(rendered_text(termui::Text("a\nb\nc", {}, 5, 1)), "a…   ");
  EXPECT_EQ(termui::Text("a\nb\nc", {}, 5, 1).size(), (std::pair<unsigned int, unsigned int>{5, 1}));
  EXPECT_EQ(termui::Text("a\nb\nc", {}, 5, 2).size(), (std::pair<unsigned int, unsigned int>{5, 2}));
}

TEST(text, TrailingNewlinesAreNotTruncated) {
  EXPECT_EQ(rendered_text(termui::Text("a\n", {}, 5, 1)), "a    ");
  EXPECT_EQ(rendered_text(termui::Text("a\n\n", {}, 5, 1)), "a    ");
}

TEST(text, UnsetSizes) {
  EXPECT_EQ(rendered_text(termui::Text("hello world")), "hello world");
  EXPECT_EQ(termui::Text("hello world").size(), (std::pair<unsigned int, unsigned int>{11, 1}));
  EXPECT_EQ(rendered_text(termui::Text("a\nb", {}, std::nullopt, 1)), "a…");
}

TEST(text, EmptyTextMeasuresAsZero) {
  EXPECT_EQ(termui::Text("", {}, 5).size(), (std::pair<unsigned int, unsigned int>{0, 0}));
  EXPECT_EQ(termui::Text("").size(), (std::pair<unsigned int, unsigned int>{0, 0}));
}

// renders a focused Input, returning the text it wrote with all CSI sequences removed
static std::string rendered_text(const std::string &value, unsigned int w, unsigned int h) {
  // the cursor query is written to the captured stdout, so nothing would ever answer it on a real
  // terminal and the read would block. Giving it an empty stdin makes the read return straight away.
  const int saved_stdin = dup(STDIN_FILENO);
  const int devnull     = open("/dev/null", O_RDONLY);
  dup2(devnull, STDIN_FILENO);

  testing::internal::CaptureStdout();
  termui::Input(value, "").width(w).height(h).focus().render();
  termui::terminal.flush();
  const std::string raw = testing::internal::GetCapturedStdout();

  dup2(saved_stdin, STDIN_FILENO);
  close(saved_stdin);
  close(devnull);

  return strip_csi(raw);
}

TEST(library, VisibleLength) {
  EXPECT_EQ(termui::visible_length("test"), 4);
  EXPECT_EQ(termui::visible_length("test💵"), 5);
  // EXPECT_EQ(termui::visible_length("test" + termui::term::SAVE_CURSOR), 4);
  // EXPECT_EQ(termui::visible_length(termui::term::SAVE_CURSOR + "test"), 4);
  // EXPECT_EQ(termui::visible_length("test" + termui::term::SAVE_CURSOR + "test"), 8);
  // EXPECT_EQ(termui::visible_length("test" + termui::term::SAVE_CURSOR + "💵"), 5);
}

TEST(library, MaxVisibleLength) {
  EXPECT_EQ(termui::max_visible_length("test", 3), 3);
  EXPECT_EQ(termui::max_visible_length("test", 4), 4);
  EXPECT_EQ(termui::max_visible_length("test", 5), 4);

  EXPECT_EQ(termui::max_visible_length("test💵", 3), 3);
  EXPECT_EQ(termui::max_visible_length("test💵", 5), 4 + 4);
  EXPECT_EQ(termui::max_visible_length("test💵", 8), 4 + 4);

  // EXPECT_EQ(termui::max_visible_length("test" + termui::term::SAVE_CURSOR, 3), 3);
  // EXPECT_EQ(termui::max_visible_length("test" + termui::term::SAVE_CURSOR, 5), 4 + 3);
  // EXPECT_EQ(termui::max_visible_length("test" + termui::term::SAVE_CURSOR, 8), 4 + 3);

  // EXPECT_EQ(termui::max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 3), 3);
  // EXPECT_EQ(termui::max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 5), 4 + 3 + 1);
  // EXPECT_EQ(termui::max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 8), 4 + 3 + 4);
}

TEST(input, FittingValueIsFullyShown) { // control: proves rendered_text can see the value
  const std::string text = rendered_text("aaaaa bbb", 5, 2);

  EXPECT_NE(text.find("aaaaa"), std::string::npos) << text;
  EXPECT_NE(text.find("bbb"), std::string::npos) << text;
}

TEST(input, OverflowingValueKeepsEndVisible) {
  const std::string text = rendered_text("aaaaa bbbbb ccc", 5, 2);

  EXPECT_NE(text.find("ccc"), std::string::npos) << text;
}

TEST(input, OverflowingValueWithoutSpacesKeepsEndVisible) {
  const std::string text = rendered_text("aaaaabbbbbccc", 5, 2);

  EXPECT_NE(text.find("ccc"), std::string::npos) << text;
}

TEST(input, LastTypedCharIsAlwaysVisible) {
  const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

  for (size_t n = 1; n <= alphabet.size(); n++) { // one render per keystroke
    const std::string text = rendered_text(alphabet.substr(0, n), 5, 2);

    EXPECT_NE(text.find(alphabet[n - 1]), std::string::npos) << "after typing " << n << " chars: " << text;
  }
}

TEST(library, SgrSequences) {
  EXPECT_EQ(termui::sgr_sequences("plain"), "");
  EXPECT_EQ(termui::sgr_sequences("\x1b[31mab\x1b[1mcd"), "\x1b[31m\x1b[1m");
  EXPECT_EQ(termui::sgr_sequences("\x1b[31mab\x1b[1mcd", 7), "\x1b[31m");
  EXPECT_EQ(termui::sgr_sequences("a\x1b[2Ab\x1b[0m"), "\x1b[0m"); // cursor moves are left out
  EXPECT_EQ(termui::sgr_sequences("a\x1b[3"), "");                 // unterminated
}

TEST(library, ReverseMaxVisibleLength) {
  EXPECT_EQ(termui::reverse_max_visible_length("test", 3), 3);
  EXPECT_EQ(termui::reverse_max_visible_length("test", 4), 4);
  EXPECT_EQ(termui::reverse_max_visible_length("test", 5), 4);

  EXPECT_EQ(termui::reverse_max_visible_length("test💵", 3), 2 + 4);
  EXPECT_EQ(termui::reverse_max_visible_length("test💵", 4), 3 + 4);
  EXPECT_EQ(termui::reverse_max_visible_length("test💵", 5), 4 + 4);
  EXPECT_EQ(termui::reverse_max_visible_length("test💵", 8), 4 + 4);

  EXPECT_EQ(termui::reverse_max_visible_length("test💵test", 3), 3);
  EXPECT_EQ(termui::reverse_max_visible_length("test💵test", 4), 4);
  EXPECT_EQ(termui::reverse_max_visible_length("test💵test", 5), 4 + 4);
  EXPECT_EQ(termui::reverse_max_visible_length("test💵test", 8), 4 + 4 + 3);

  // the char straight after a CSI sequence is visible and must be counted
  EXPECT_EQ(termui::reverse_max_visible_length("ab\x1b[0mcd", 2), 2);
  EXPECT_EQ(termui::reverse_max_visible_length("ab\x1b[0mcd", 3), 2 + 4 + 1);

  // an unterminated CSI is invisible, matching visible_length
  EXPECT_EQ(termui::reverse_max_visible_length("ab\x1b[", 1), 1 + 2);
  EXPECT_EQ(termui::reverse_max_visible_length("ab\x1b[31", 2), 2 + 4);

  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR, 3), 3 + 3);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR, 5), 4 + 3);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR, 8), 4 + 3);

  // EXPECT_EQ(termui::reverse_max_visible_length(termui::term::SAVE_CURSOR + "test", 3), 3);
  // EXPECT_EQ(termui::reverse_max_visible_length(termui::term::SAVE_CURSOR + "test", 5), 4);
  // EXPECT_EQ(termui::reverse_max_visible_length(termui::term::SAVE_CURSOR + "test", 8), 4);

  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 3), 3);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 4), 4);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 5), 4 + 3 + 1);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 6), 4 + 3 + 2);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 7), 4 + 3 + 3);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 8), 4 + 3 + 4);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 9), 4 + 3 + 4);

  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 3), 3);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 4), 4);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 5), 4 + 3 + 1);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 6), 4 + 3 + 2);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 7), 4 + 3 + 3);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 8), 4 + 3 + 4);
  // EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 9), 4 + 3 + 4 + 3 + 1);
}
