#include <qutil/constexpr_string.hpp>

auto main() -> int {
  constexpr auto str1 = qutil::make_constexpr_string("Hello");
  constexpr auto c = str1.find('l');
  static_assert(c == 2);

  constexpr qutil::constexpr_string str2(" world");
  constexpr qutil::constexpr_string<4> str3(" by");

  constexpr auto t = str1 + str2 + str3;

  static_assert(t.size() == 14);

  constexpr auto sv = qutil::make_constexpr_string("ababa");
  static_assert(sv == "ababa");

  constexpr std::string_view sv2("ababa");
  static_assert(sv2 == sv);

  auto st = sv.to_string();
  static_assert(std::is_same_v<std::string, decltype(st)>);
}