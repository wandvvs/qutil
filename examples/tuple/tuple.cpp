#include <iostream>
#include <qutil/containers/tuple.hpp>

auto main() -> int {
  qutil::containers::tuple<int, char> t1(5, 'f');
  qutil::containers::tuple t2(1, 3.14, std::string("Hello"));

  auto t3 = qutil::containers::make_tuple(2, 2, 3.14f, 5u);
  auto t4 = qutil::containers::transform(t3, [](auto&& el) {
    return el *= 2;
  });

  qutil::containers::for_each(t3, [](auto&& el) {
    std::cout << el << " ";
  });

  std::cout << '\n';

  auto t5 = qutil::containers::tuple_cat(t1, t2, t3, t4);
  std::cout << qutil::containers::tuple_size_v<decltype(t5)> << std::endl;
}
