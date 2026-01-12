#include <termui/elements.hpp>
namespace termui {

Input::Input() = default;
Input::Input(const termui::string &val, const termui::string &plh, int w, int h, int valCol, int plhCol)
    : val(val.share()), plh(plh.share()), w(w), h(h), valCol(valCol), plhCol(plhCol) {}
Input::Input(const termui::string &val, const std::string &plh, int w, int h, int valCol, int plhCol)
    : val(val.share()), plh(plh), w(w), h(h), valCol(valCol), plhCol(plhCol) {}
Input::Input(const termui::string &val, int w, int h, int valCol) : val(val.share()), plh(""), w(w), h(h), valCol(valCol) {}

const std::string &Input::getVal() const { return val.text(); }
std::string &Input::getVal() { return val.text(); }

const std::string &Input::getPlh() const { return plh.text(); }
std::string &Input::getPlh() { return plh.text(); }

std::string Input::render() {
  std::string outbuff;

  bool use_response = !val.text().empty();
  bool use_placeholder = !use_response;

  std::deque<std::string> formatted;

  if (use_response) {
    std::string copy = val.text();
    std::size_t next(0);

    for (int i = 0; i < h; i++) {
      if (copy.rfind('\n') == std::string::npos) {
        next = std::max<size_t>({0, copy.size() - max_visible_length(copy, w)});
        formatted.push_front(std::string(copy.begin() + next, copy.end()));

      } else {
        next = std::max<size_t>({0, copy.rfind('\n'), copy.size() - max_visible_length(copy, w)});
        formatted.push_front(std::string(copy.begin() + next + 1, copy.end()));
      }

      formatted.front() = std::string(w - visible_length(formatted.front()), ' ') + formatted.front();

      copy = std::string(copy.begin(), std::min(copy.begin() + next, copy.end()));
    }

    if (copy.size() > 0) {
      formatted.front() = unicode::ELLIPSIS + std::string(formatted.front().begin() + 1, formatted.front().end());
    }

  } else if (use_placeholder) {
    std::string copy = plh.text();
    std::size_t next = 0;

    for (int i = 0; i < h; i++) {
      if (copy.find('\n') == std::string::npos) {
        next = std::min({copy.size(), max_visible_length(copy, w)});
      } else {
        next = std::min({copy.size(), max_visible_length(copy, w), copy.find('\n')});
      }

      formatted.push_back(std::string(copy.begin(), copy.begin() + next));        // make substring of <=w printed symbols
      formatted.back() += std::string(w - visible_length(formatted.back()), ' '); // add spacing to fill w

      if (copy[next] == '\n') {
        copy = std::string(std::min(copy.end(), copy.begin() + next + 1), copy.end());
      } else {
        copy = std::string(std::min(copy.end(), copy.begin() + next), copy.end());
      }
    }

    if (copy.size() > 0) {
      size_t idx = std::min<size_t>(formatted.back().find_last_not_of(' ') + 1, w - 1);
      // ^ dont want ellipsis to overwrite a char unless it must be in the final column

      formatted.back() = std::string(formatted.back().begin(), formatted.back().begin() + idx) + unicode::ELLIPSIS;

      formatted.back() += std::string(std::max<int>(0, w - formatted.back().length()), ' '); // add back missing padding
    }

  } else { // both blank?
    for (int i = 0; i < h; i++) {
      formatted.push_back(std::string(w, ' '));
    }
  }

  for (int i = 0; i < h; i++) {
    outbuff += formatted.at(i);

    if (i == h - 1) {
      continue; // continue to avoid interfering with other ui elements
    } else {
      outbuff += curs_down(1) + curs_left(w);
    }
  }

  if (use_placeholder) {
    outbuff = fg_apply(outbuff, plhCol);
  }

  return outbuff;
}

} // namespace termui
