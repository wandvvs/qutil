#include <qutil/containers/result.hpp>

using namespace qutil::containers;

auto main() -> int {
  constexpr auto f = [](int x) -> result<float, std::string_view> {
    if(x % 2 == 0) {
      return ok(static_cast<float>(x));
    } else {
      return err("x % 2 != 0");
    }
  }(4)
                                      .and_then([](float x) -> result<int, std::string_view> {
                                        if(x > 3) {
                                          return ok(static_cast<int>(x * x));
                                        } else {
                                          return err("x < 3");
                                        }
                                      })
                                      .and_then([](int x) -> result<int, void> {
                                        if(x > 5) {
                                          return ok(x * 2);
                                        } else {
                                          return err();
                                        }
                                      });

  static_assert(f.value_or(0) != 0);
  static_assert(f.is_ok() == true);
  static_assert(f.is_err() == false);
}