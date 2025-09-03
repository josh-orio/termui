#include "str.hpp"

// custom shared ptr strings for termUI

namespace termui {

string::string() : data(std::make_shared<std::string>("")) {}
string::string(const std::string &str) : data(std::make_shared<std::string>(str)) {}
string::string(std::shared_ptr<std::string> t) : data(t){};

const std::string &string::text() const { return *data; }
std::string &string::text() { return *data; }

const std::shared_ptr<std::string> string::share() const { return data; }
std::shared_ptr<std::string> string::share() { return data; }

strings::strings() : items(std::make_shared<std::vector<string>>()) {}
strings::strings(const std::vector<std::string> &strs) : items(std::make_shared<std::vector<string>>()) {
  items->reserve(strs.size());

  for (const auto &s : strs) {
    items->emplace_back(s);
  }
}
strings::strings(const termui::strings &strs) : items(strs.share()) {}
strings::strings(int n, const std::string &x) : items(std::make_shared<std::vector<string>>()) {
  items->reserve(n);

  for (int i = 0; i < n; i++) {
    items->emplace_back(x);
  }
}

const std::string &strings::getItem(int i) const { return items->at(i).text(); }
std::string &strings::getItem(int i) { return items->at(i).text(); }

const std::shared_ptr<std::string> strings::shareItem(int i) const { return items->at(i).share(); }
std::shared_ptr<std::string> strings::shareItem(int i) { return items->at(i).share(); }

const std::shared_ptr<std::vector<string>> strings::share() const { return items; }
std::shared_ptr<std::vector<string>> strings::share() { return items; }

size_t strings::size() const { return items->size(); }

} // namespace termui
