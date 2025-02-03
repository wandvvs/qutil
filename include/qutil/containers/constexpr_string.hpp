#pragma once

#include <algorithm>
#include <array>
#include <ostream>
#include <ranges>

namespace qutil::containers {
template <std::size_t Size>
class constexpr_string {
 public:
  explicit constexpr_string() = default;
  constexpr explicit constexpr_string(std::string_view str) {
    std::ranges::copy_n(str.begin(), str.size() > this->size() ? str.size() : this->size(), str_.begin());
  }

  constexpr constexpr_string(const char (&str)[Size]) {  // NOLINT
    std::ranges::copy_n(str, Size, this->str_.begin());
  };

  constexpr explicit operator std::string_view() const& {
    return {this->begin()};
  }

  constexpr auto operator==(std::string_view other) const -> bool {
    return static_cast<std::string_view>(*this) == other;
  }

  constexpr auto operator==(const char* other) const -> bool {
    return static_cast<std::string_view>(*this) == other;
  }

  constexpr auto operator==(const constexpr_string<Size>& other) -> bool {
    return static_cast<std::string_view>(*this) == static_cast<std::string_view>(other);
  }

  template <std::size_t Ssize>
  constexpr auto operator==(const constexpr_string<Ssize>& other) -> bool {
    return false;
  }

  constexpr auto operator==(const std::string& other) {
    return static_cast<std::string_view>(*this) == other;
  }
  template <std::size_t Ssize>
  constexpr auto operator+(const constexpr_string<Ssize>& other) const -> const constexpr_string<Size + Ssize - 1> {
    constexpr_string<Size + Ssize - 1> res;
    std::ranges::copy_n(this->begin(), Size - 1, res.begin());
    std::ranges::copy_n(other.begin(), Ssize, res.begin() + Size - 1);

    return res;
  }

  constexpr static std::size_t npos = static_cast<std::size_t>(-1);

  constexpr auto find(char ch, std::size_t pos = 0) const -> std::size_t {
    const auto sub = std::ranges::views::drop(*this, pos);
    const auto it = std::ranges::find(sub, ch);
    return std::ranges::find(sub, ch) != sub.end() ? std::ranges::distance(begin(), it) : npos;
  }

  [[nodiscard]] constexpr auto to_string() const -> std::string {
    return {str_.data()};
  }

  constexpr auto size() const {
    return Size == 0 ? 0 : Size - 1;
  }

  [[nodiscard]] constexpr auto begin() const -> const char* {
    return str_.begin();
  }

  constexpr auto begin() -> char* {
    return str_.begin();
  }

  [[nodiscard]] constexpr auto end() const -> const char* {
    return str_.end();
  }

  constexpr auto end() -> char* {
    return str_.end();
  }

 private:
  std::array<char, Size> str_{};
};

template <std::size_t Size>
constexpr constexpr_string<Size> make_constexpr_string(const char (&str)[Size]) {  // NOLINT
  return qutil::containers::constexpr_string<Size>(str);
}

}  // namespace qutil::containers

template <std::size_t N>
constexpr auto operator<<(std::ostream& os, const qutil::containers::constexpr_string<N>& str) -> std::ostream& {
  os << static_cast<std::string_view>(str);
  return os;
}
