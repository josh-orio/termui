#include "structs.hpp"

namespace termui {

namespace item {

MultiListItem::MultiListItem() = default;
MultiListItem::MultiListItem(const std::string &t) : text(std::make_shared<std::string>(t)), selected(false) {}
MultiListItem::MultiListItem(std::string &&t) : text(std::make_shared<std::string>(std::move(t))), selected(false) {}
MultiListItem::MultiListItem(std::shared_ptr<std::string> shared) : text(std::move(shared)), selected(false) {}

std::shared_ptr<std::string> MultiListItem::share() const { return text; }

const std::string &MultiListItem::getText() const { return *text; }
std::string &MultiListItem::getText() { return *text; }

bool &MultiListItem::isSelected() { return selected; }

void MultiListItem::flip() { selected = !selected; }

} // namespace item

} // namespace termui
