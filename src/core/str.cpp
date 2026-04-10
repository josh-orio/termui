#include <termui/core/str.hpp>

// custom shared ptr strings for termUI

namespace termui {

// --- String ---
string::string() : ptr(std::make_shared<std::string>()) {}
string::string(const termui::string &t) : ptr(t) {} // must copy shrd ptr, not the object
string::string(const std::string &str) : ptr(std::make_shared<std::string>(str)) {}
string::string(const char *str) : ptr(std::make_shared<std::string>(str)) {}

// std::string &string::text() noexcept { return *ptr; }
// const std::string &string::text() const noexcept { return *ptr; }

// std::shared_ptr<std::string> string::share() const noexcept { return ptr; }

// --- Strings ---
strings::strings() : items(std::make_shared<std::vector<termui::string>>()) {}
strings::strings(const termui::strings &strs) : items(strs.share()) {}
strings::strings(const std::vector<std::string> &strs) : items(std::make_shared<std::vector<termui::string>>()) {
  items->reserve(strs.size());

  for (const auto &s : strs) {
    items->emplace_back(s);
  }
}
strings::strings(std::initializer_list<std::string> strs) {
  items = std::make_shared<std::vector<termui::string>>();
  for (auto s : strs) {
    items->emplace_back(s);
  }
}
strings::strings(int n, const std::string &x) : items(std::make_shared<std::vector<termui::string>>()) {
  items->reserve(n);

  for (int i = 0; i < n; i++) {
    items->emplace_back(x);
  }
}

termui::string       &strings::at(size_t idx) { return items->at(idx); };
const termui::string &strings::at(size_t idx) const { return items->at(idx); };

std::shared_ptr<std::vector<string>> strings::share() const noexcept { return items; }

size_t strings::size() const { return items->size(); }

} // namespace termui
