#include <iostream>
#include <vector>

#include "qutil/dump.hpp"

int main() {
  using qutil::dump;

  std::vector<int> v = {1, 2, 3, 4, 5, 155, 6};

  std::cout << qutil::dump(v).str() << std::endl;
}