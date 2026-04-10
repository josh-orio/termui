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
#include <termui/ui/display/lines.hpp>
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

#endif
