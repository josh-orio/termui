#ifndef TSTR_HPP
#define TSTR_HPP

#include <memory>
#include <string>
#include <vector>

// custom shared ptr strings for termUI

namespace termui {

class string {
private:
  std::shared_ptr<std::string> ptr;

public:
  string();
  string(const termui::string &t);
  string(const std::string &str);
  string(const char *str);

  // Replaces text() — implicit conversion to std::string ref (derefs ptr)
  operator std::string &() noexcept { return *ptr; }
  operator const std::string &() const noexcept { return *ptr; }

  // Replaces share() — implicit conversion to shared_ptr
  operator std::shared_ptr<std::string>() const noexcept { return ptr; }

  // Assign from raw string/char* — assigns into the existing shared object
  termui::string &operator=(const std::string &str) {
    *ptr = str;
    return *this;
  }
  termui::string &operator=(const char *str) {
    *ptr = str;
    return *this;
  }

  // Assign from another termui::string — rebinds this ptr to theirs (shared ownership)
  termui::string &operator=(const termui::string &t) {
    ptr = t.ptr;
    return *this;
  }
};

// class string {
// private:
//   std::shared_ptr<std::string> ptr;

// public:
//   string();
//   string(const termui::string &t);
//   string(const std::string &str);
//   string(const char *str);

//   std::string &text() noexcept;
//   const std::string &text() const noexcept;

//   std::shared_ptr<std::string> share() const noexcept;
// };

class strings {
private:
  std::shared_ptr<std::vector<termui::string>> items;

public:
  strings(); // this is kinda pointless without an add() function
  strings(const termui::strings &strs);
  strings(const std::vector<std::string> &strs);
  strings(std::initializer_list<std::string> strs);
  strings(int n, const std::string &x);

  termui::string       &at(size_t idx);
  const termui::string &at(size_t idx) const;

  std::shared_ptr<std::vector<string>> share() const noexcept;

  size_t size() const;
};

} // namespace termui

#endif
