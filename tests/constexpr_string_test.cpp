#include <gtest/gtest.h>

#include "qutil/containers/constexpr_string.hpp"

TEST(CS_Tostring, BasicAssertions) {
  constexpr auto sv = qutil::containers::make_constexpr_string("ababa");
  ASSERT_EQ(sv, "ababa");

  constexpr std::string_view sv2("ababa");
  ASSERT_EQ(sv2, sv);

  auto st = sv.to_string();
  constexpr bool is_same = std::is_same_v<std::string, decltype(st)>;
}

TEST(CS_Copy, BasicAssertions) {
  constexpr auto sv = qutil::containers::make_constexpr_string("ababa");
  ASSERT_EQ(sv, "ababa");

  constexpr std::string_view sv2("ababa");
  ASSERT_EQ(sv2, sv);
}

TEST(CS_Concat, BasicAsserions) {
  constexpr auto str1 = qutil::containers::make_constexpr_string("Hello");
  constexpr auto c = str1.find('l');
  ASSERT_EQ(c, 2);

  constexpr qutil::containers::constexpr_string str2(" world");
  constexpr qutil::containers::constexpr_string<4> str3(" by");

  constexpr auto t = str1 + str2 + str3;

  ASSERT_EQ(t.size(), 14);
}

TEST(CS_Consistent, BasicAssertions) {
  constexpr auto str4 = qutil::containers::make_constexpr_string("Hello");
  ASSERT_EQ(str4.starts_with("Hel"), true);
  ASSERT_EQ(str4.end_with("lo"), true);
  ASSERT_EQ(str4.contains('e'), true);

  constexpr bool is_npos = str4.find('e') == decltype(str4)::npos;
  ASSERT_EQ(is_npos, false);
}

TEST(CS_Find, BasicAssertions) {
  constexpr auto str1 = qutil::containers::make_constexpr_string("Hello");
  constexpr auto c = str1.find('l');
  ASSERT_EQ(c, 2);
}