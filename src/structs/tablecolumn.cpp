#include "structs.hpp"

namespace termui {

namespace item {

TableColumn::TableColumn() = default;
TableColumn::TableColumn(std::string &t, int w) : text(std::make_shared<std::string>(t)), w(w) {}
TableColumn::TableColumn(std::string &&t, int w) : text(std::make_shared<std::string>(std::move(t))), w(w) {}
TableColumn::TableColumn(std::shared_ptr<std::string> shared, int w) : text(std::move(shared)), w(w) {}

std::shared_ptr<std::string> TableColumn::share() const { return text; }

const std::string &TableColumn::getText() const { return *text; }
std::string &TableColumn::getText() { return *text; }

int TableColumn::getWidth() { return w; }

} // namespace item

} // namespace termui
