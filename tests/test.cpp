#include <gtest/gtest.h>
#include <termui/termui.hpp>

TEST(library, VisibleLength) {
  EXPECT_EQ(termui::visible_length("test"), 4);
  EXPECT_EQ(termui::visible_length("test💵"), 5);
  EXPECT_EQ(termui::visible_length("test" + termui::term::SAVE_CURSOR), 4);
  EXPECT_EQ(termui::visible_length(termui::term::SAVE_CURSOR + "test"), 4);
  EXPECT_EQ(termui::visible_length("test" + termui::term::SAVE_CURSOR + "test"), 8);
  EXPECT_EQ(termui::visible_length("test" + termui::term::SAVE_CURSOR + "💵"), 5);
}

TEST(library, MaxVisibleLength) {
  EXPECT_EQ(termui::max_visible_length("test", 3), 3);
  EXPECT_EQ(termui::max_visible_length("test", 4), 4);
  EXPECT_EQ(termui::max_visible_length("test", 5), 4);

  EXPECT_EQ(termui::max_visible_length("test💵", 3), 3);
  EXPECT_EQ(termui::max_visible_length("test💵", 5), 4 + 4);
  EXPECT_EQ(termui::max_visible_length("test💵", 8), 4 + 4);

  EXPECT_EQ(termui::max_visible_length("test" + termui::term::SAVE_CURSOR, 3), 3);
  EXPECT_EQ(termui::max_visible_length("test" + termui::term::SAVE_CURSOR, 5), 4 + 3);
  EXPECT_EQ(termui::max_visible_length("test" + termui::term::SAVE_CURSOR, 8), 4 + 3);

  EXPECT_EQ(termui::max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 3), 3);
  EXPECT_EQ(termui::max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 5), 4 + 3 + 1);
  EXPECT_EQ(termui::max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 8), 4 + 3 + 4);
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

  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR, 3), 3 + 3);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR, 5), 4 + 3);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR, 8), 4 + 3);

  EXPECT_EQ(termui::reverse_max_visible_length(termui::term::SAVE_CURSOR + "test", 3), 3);
  EXPECT_EQ(termui::reverse_max_visible_length(termui::term::SAVE_CURSOR + "test", 5), 4);
  EXPECT_EQ(termui::reverse_max_visible_length(termui::term::SAVE_CURSOR + "test", 8), 4);

  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 3), 3);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 4), 4);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 5), 4 + 3 + 1);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 6), 4 + 3 + 2);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 7), 4 + 3 + 3);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 8), 4 + 3 + 4);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test", 9), 4 + 3 + 4);

  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 3), 3);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 4), 4);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 5), 4 + 3 + 1);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 6), 4 + 3 + 2);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 7), 4 + 3 + 3);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 8), 4 + 3 + 4);
  EXPECT_EQ(termui::reverse_max_visible_length("test" + termui::term::SAVE_CURSOR + "test" + termui::term::SAVE_CURSOR + "test", 9), 4 + 3 + 4 + 3 + 1);
}
