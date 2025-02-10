#include "qutil/io/logger.hpp"

#include <cassert>
#include <iostream>

int main() {
  auto& l3 = qutil::io::file_logger::get_logger("log.txt");

  for(std::size_t i = 0; i < 50; ++i) {
    l3.log(qutil::io::log_level::ERROR, "example");
  }

  l3.set_level(qutil::io::log_level::ERROR);

  for(std::size_t i = 0; i < 50; ++i) {
    l3.log(qutil::io::log_level::WARNING, "example");
  }
}