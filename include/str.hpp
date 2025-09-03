#ifndef TSTR_HPP
#define TSTR_HPP

#include <memory>
#include <string>
#include <vector>

// custom shared ptr strings for termUI

namespace termui {

class string {
private:
  std::shared_ptr<std::string> data;

public:
  string();
  explicit string(const std::string &str);
  explicit string(std::shared_ptr<std::string> t);

  const std::string &text() const;
  std::string &text();

  const std::shared_ptr<std::string> share() const;
  std::shared_ptr<std::string> share();
};

class strings {
private:
  std::shared_ptr<std::vector<termui::string>> items;

public:
  strings();
  explicit strings(const std::vector<std::string> &strs);
  explicit strings(const termui::strings &strs);
  explicit strings(int n, const std::string &x);

  const std::string &getItem(int i) const;
  std::string &getItem(int i);

  const std::shared_ptr<std::string> shareItem(int i) const;
  std::shared_ptr<std::string> shareItem(int i);

  const std::shared_ptr<std::vector<string>> share() const;
  std::shared_ptr<std::vector<string>> share();

  size_t size() const;
};

} // namespace termui

#endif
