#include <fstream>
#include <list>
#include <qutil/io/dump.hpp>
#include <vector>

auto main() -> int {
  std::vector v{1, 2, 3};
  std::list l{3, 2, 1};
  std::string s("abc");

  qutil::io::dump(std::ofstream("build/examples/dump/out.txt"), v, l, s, 1, 2, 3, "\n", "hello");
  qutil::io::dump("hello world\n");             // to stdout
  qutil::io::dump(std::cout, "hello world\n");  // to std:out
  qutil::io::dump(1, 2, 3, v, "abc");           // [1, 2, 3, [1, 2, 3], abc]
}