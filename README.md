<h1 align="center">qutil</h1>
<h3 align="center">Provides modern and useful containers and algorithms.</h3>

<p align="center">
     <a href="https://github.com/wandvvs/qutil/issues">
     <img src="https://img.shields.io/github/issues/wandvvs/qutil"
          alt="GitHub opened issues">
     <img src="https://img.shields.io/badge/status-in_development-red"
          alt="Status">
     <img src="https://img.shields.io/github/stars/wandvvs/qutil?color=lime"
          alt="Stars">
</p>

 ## **Installation**
```bash
git clone https://github.com/wandvvs/qutil.git
cd qutil
sudo ./install.sh
```
For an example of how to use qutil in a CMake project, see the following [CMakeLists.txt](https://github.com/wandvvs/qutil/blob/master/examples/tuple/CMakeLists.txt)

## Features examples
### result<T,E>:
```cpp
#include <qutil/result.hpp>

using namespace qutil;

auto main() -> int {
    constexpr auto f = [](int x) -> result<float, std::string_view> {
        if (x % 2 == 0) {
            return ok(static_cast<float>(x));
        } else {
            return err("x % 2 != 0");
        }
    }(4)
    .and_then([](float x) -> result<int, std::string_view> {
        if (x > 3) {
            return ok(static_cast<int>(x * x));
        } else {
            return err("x < 3");
        }
    })
    .and_then([](int x) -> result<int, void> {
        if (x > 5) {
            return ok(x * 2);
        } else {
            return err();
        }
    });

    static_assert(f.value_or(0) != 0);
    static_assert(f.is_ok() == true);
    static_assert(f.is_err() == false);
}
```
### tuple:
```cpp
#include <iostream>
#include <qutil/tuple.hpp>

auto main() -> int {
  qutil::tuple<int, char> t1(5, 'f');
  qutil::tuple t2(1, 3.14, std::string("Hello"));

  auto t3 = qutil::make_tuple(2, 2, 3.14f, 5u);
  auto t4 = qutil::transform(t3, [](auto&& el) {
    return el *= 2;
  });

  qutil::for_each(t3, [](auto&& el) {
    std::cout << el << " ";
  });

  std::cout << '\n';

  auto t5 = qutil::tuple_cat(t1, t2, t3, t4);
  std::cout << qutil::tuple_size_v<decltype(t5)> << std::endl;
}
```
### constexpr_string:
```cpp
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
```
