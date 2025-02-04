#include <fstream>
#include <list>
#include <qutil/io/dump.hpp>
#include <vector>

auto main() -> int {
  std::vector v{1, 2, 3};
  std::list l{3, 2, 1};
  std::string s("abc");

  qutil::io::dump(v, s);
  std::cout << '\n';

  qutil::io::dumpln(std::ofstream("build/examples/dump/out.txt"), v, l, s, 1, 2, 3, "\n", "hello");
  qutil::io::dumpln("hello world");             // to stdout
  qutil::io::dumpln(std::cout, "hello world");  // to std:out
  qutil::io::dumpln(1, 2, 3, v, "abc");         // [1, 2, 3, [1, 2, 3], abc]
}