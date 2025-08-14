#include "structs.hpp"

namespace termui {

namespace item {

TableRow::TableRow() = default;
TableRow::TableRow(std::vector<std::string> &t) : cells(std::make_shared<std::vector<std::string>>(t)) {}
TableRow::TableRow(std::vector<std::string> &&t) : cells(std::make_shared<std::vector<std::string>>(std::move(t))) {}
TableRow::TableRow(std::shared_ptr<std::vector<std::string>> sharedT)
    : cells(std::move(sharedT)) {} // caller still owns the original shared_ptr unless it was moved into the argument

std::shared_ptr<std::vector<std::string>> TableRow::share() const { return cells; }

const std::vector<std::string> &TableRow::getCells() const { return *cells; }
std::vector<std::string> &TableRow::getCells() { return *cells; }

} // namespace item

} // namespace termui
