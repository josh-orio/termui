#include "structs.hpp"

namespace termui {

namespace item {

ListItem::ListItem() = default;
ListItem::ListItem(std::string &t) : text(std::make_shared<std::string>(t)) {}
ListItem::ListItem(std::string &&t) : text(std::make_shared<std::string>(std::move(t))) {}
ListItem::ListItem(std::shared_ptr<std::string> shared) : text(std::move(shared)) {}

std::shared_ptr<std::string> ListItem::share() const { return text; }

const std::string &ListItem::getText() const { return *text; }
std::string &ListItem::getText() { return *text; }

} // namespace item

} // namespace termui