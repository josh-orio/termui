#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <memory>
#include <string>

namespace termui {

namespace item {

struct ListItem {
private:
  std::shared_ptr<std::string> text;

public:
  ListItem() = default;
  ListItem(const std::string &t) : text(std::make_shared<std::string>(t)) {}
  ListItem(std::string &&t) : text(std::make_shared<std::string>(std::move(t))) {}
  ListItem(std::shared_ptr<std::string> shared) : text(std::move(shared)) {}

  std::shared_ptr<std::string> share() const { return text; }

  const std::string &getText() const { return *text; }
  std::string &getText() { return *text; }
};

struct MultiListItem {
private:
  std::shared_ptr<std::string> text;
  bool selected;

public:
  MultiListItem() = default;
  MultiListItem(const std::string &t) : text(std::make_shared<std::string>(t)), selected(false) {}
  MultiListItem(std::string &&t) : text(std::make_shared<std::string>(std::move(t))), selected(false) {}
  MultiListItem(std::shared_ptr<std::string> shared) : text(std::move(shared)), selected(false) {}

  std::shared_ptr<std::string> share() const { return text; }

  const std::string &getText() const { return *text; }
  std::string &getText() { return *text; }

  bool &isSelected() { return selected; }

  void flip() { selected = !selected; }
};

struct FancyListItem {
private:
  std::shared_ptr<std::string> text, desc;

public:
  FancyListItem() = default;
  FancyListItem(const std::string &t, const std::string &d) : text(std::make_shared<std::string>(t)), desc(std::make_shared<std::string>(d)) {}
  FancyListItem(std::string &&t, std::string &&d) : text(std::make_shared<std::string>(std::move(t))), desc(std::make_shared<std::string>(std::move(d))) {}
  FancyListItem(std::shared_ptr<std::string> t, std::shared_ptr<std::string> d) : text(std::move(t)), desc(std::move(d)) {}

  std::shared_ptr<std::string> share() const { return text; }

  const std::string &getText() const { return *text; }
  std::string &getText() { return *text; }

  const std::string &getDesc() const { return *desc; }
  std::string &getDesc() { return *desc; }
};

struct TableColumn {
private:
  std::shared_ptr<std::string> text;
  int w;

public:
  TableColumn() = default;
  TableColumn(const std::string &t, int w) : text(std::make_shared<std::string>(t)), w(w) {}
  TableColumn(std::string &&t, int w) : text(std::make_shared<std::string>(std::move(t))), w(w) {}
  TableColumn(std::shared_ptr<std::string> shared, int w) : text(std::move(shared)), w(w) {}

  std::shared_ptr<std::string> share() const { return text; }

  const std::string &getText() const { return *text; }
  std::string &getText() { return *text; }

  int getWidth() { return w; }
};

struct TableRow {
public:
  std::shared_ptr<std::vector<std::string>> cells;

  TableRow() = default;
  TableRow(const std::vector<std::string> &t) : cells(std::make_shared<std::vector<std::string>>(t)) {}
  TableRow(std::vector<std::string> &&t) : cells(std::make_shared<std::vector<std::string>>(std::move(t))) {}
  TableRow(std::shared_ptr<std::vector<std::string>> sharedT)
      : cells(std::move(sharedT)) {} // caller still owns the original shared_ptr unless it was moved into the argument

  std::shared_ptr<std::vector<std::string>> share() const { return cells; }

  const std::vector<std::string> &getCells() const { return *cells; }
  std::vector<std::string> &getCells() { return *cells; }
};

} // namespace item

} // namespace termui

#endif
