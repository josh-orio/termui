#include "structs.hpp"

namespace termui {

namespace item {

FancyListItem::FancyListItem() = default;
FancyListItem::FancyListItem(const std::string &t, const std::string &d) : text(std::make_shared<std::string>(t)), desc(std::make_shared<std::string>(d)) {}
FancyListItem::FancyListItem(std::string &&t, std::string &&d)
    : text(std::make_shared<std::string>(std::move(t))), desc(std::make_shared<std::string>(std::move(d))) {}
FancyListItem::FancyListItem(std::shared_ptr<std::string> t, std::shared_ptr<std::string> d) : text(std::move(t)), desc(std::move(d)) {}

std::shared_ptr<std::string> FancyListItem::share() const { return text; }

const std::string &FancyListItem::getText() const { return *text; }
std::string &FancyListItem::getText() { return *text; }

const std::string &FancyListItem::getDesc() const { return *desc; }
std::string &FancyListItem::getDesc() { return *desc; }

} // namespace item

} // namespace termui
