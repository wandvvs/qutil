#include <qutil/io/dump.hpp>
#include <qutil/io/read.hpp>
#include <string_view>
#include <vector>

constexpr std::string_view PATH = "build/examples/read/out.txt";

auto main() -> int {
  std::vector v{1, 5, 7, 4};

  qutil::io::dump(std::ofstream(PATH.data()), v, "abc");
  qutil::io::dump(qutil::io::read(PATH));
}