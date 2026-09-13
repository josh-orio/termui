#ifndef UI_HPP
#define UI_HPP

// This header adds all of the UI related classes to the project without bloating termui.hpp

// --- Base ---
#include <termui/ui/base/input.hpp>
#include <termui/ui/base/padded-text.hpp>
// #include <termui/ui/base/pager.hpp>
#include <termui/ui/base/text.hpp>

// --- Display ---
#include <termui/ui/display/area.hpp>
#include <termui/ui/display/box.hpp>
#include <termui/ui/display/line.hpp>
#include <termui/ui/display/progress-bar.hpp>

// --- Widgets ---
#include <termui/ui/widgets/button.hpp>
#include <termui/ui/widgets/fancy-list.hpp>
#include <termui/ui/widgets/list.hpp>
#include <termui/ui/widgets/toggle-list.hpp>

// --- Interfaces ---
#include <termui/ui/interfaces/binary-menu.hpp>
#include <termui/ui/interfaces/fancy-menu.hpp>
#include <termui/ui/interfaces/info-box.hpp>
#include <termui/ui/interfaces/info-page.hpp>
#include <termui/ui/interfaces/input-box.hpp>
#include <termui/ui/interfaces/input-page.hpp>
#include <termui/ui/interfaces/interface.hpp>
#include <termui/ui/interfaces/menu.hpp>
#include <termui/ui/interfaces/spreadsheet.hpp>
#include <termui/ui/interfaces/text-editor.hpp>
#include <termui/ui/interfaces/toggle-menu.hpp>

namespace tui {
// ui/base/
using Input      = termui::Input;
using PaddedText = termui::PaddedText;
using Pager      = termui::Pager;
using Text       = termui::Text;

// ui/display/
using Area = termui::Area;
using Box  = termui::Box;
// using  Lines=lin ;
using ProgressBar = termui::ProgressBar;

// ui/interfaces/
using BinaryMenu  = termui::BinaryMenu;
using FancyMenu   = termui::FancyMenu;
using InfoBox     = termui::InfoBox;
using InfoPage    = termui::InfoPage;
using InputBox    = termui::InputBox;
using InputPage   = termui::InputPage;
using Interface   = termui::Interface;
using Menu        = termui::Menu;
using Spreadsheet = termui::Spreadsheet;
using TextEditor  = termui::TextEditor;
using ToggleMenu  = termui::ToggleMenu;

// ui/widgets/
using Button     = termui::Button;
using FancyList  = termui::FancyList;
using List       = termui::List;
using Table      = termui::Table;
using ToggleList = termui::ToggleList;
} // namespace tui

#endif
