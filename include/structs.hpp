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
  ListItem();
  ListItem(std::string &t);
  ListItem(std::string &&t);
  ListItem(std::shared_ptr<std::string> shared);

  std::shared_ptr<std::string> share() const;

  const std::string &getText() const;
  std::string &getText();
};

struct MultiListItem {
private:
  std::shared_ptr<std::string> text;
  bool selected;

public:
  MultiListItem();
  MultiListItem(std::string &t);
  MultiListItem(std::string &&t);
  MultiListItem(std::shared_ptr<std::string> shared);

  std::shared_ptr<std::string> share() const;

  const std::string &getText() const;
  std::string &getText();

  bool &isSelected();

  void flip();
};

struct FancyListItem {
private:
  std::shared_ptr<std::string> text, desc;

public:
  FancyListItem();
  FancyListItem(std::string &t, const std::string &d);
  FancyListItem(std::string &&t, std::string &&d);
  FancyListItem(std::shared_ptr<std::string> t, std::shared_ptr<std::string> d);

  std::shared_ptr<std::string> share() const;

  const std::string &getText() const;
  std::string &getText();

  const std::string &getDesc() const;
  std::string &getDesc();
};

struct TableColumn {
private:
  std::shared_ptr<std::string> text;
  int w;

public:
  TableColumn();
  TableColumn(std::string &t, int w);
  TableColumn(std::string &&t, int w);
  TableColumn(std::shared_ptr<std::string> shared, int w);

  std::shared_ptr<std::string> share() const;

  const std::string &getText() const;
  std::string &getText();

  int getWidth();
};

struct TableRow {
public:
  std::shared_ptr<std::vector<std::string>> cells;

  TableRow();
  TableRow(std::vector<std::string> &t);
  TableRow(std::vector<std::string> &&t);
  TableRow(std::shared_ptr<std::vector<std::string>> sharedT); // caller still owns the original shared_ptr unless it was moved into the argument

  std::shared_ptr<std::vector<std::string>> share() const;

  const std::vector<std::string> &getCells() const;
  std::vector<std::string> &getCells();
};

} // namespace item

} // namespace termui

#endif
