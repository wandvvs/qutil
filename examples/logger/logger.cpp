#include "qutil/io/logger.hpp"

#include <cassert>
#include <iostream>

#include "qutil/io/sink/sink.hpp"

int main() {
  auto& l3 = qutil::io::file_logger::get_logger("log.txt");

  for(std::size_t i = 0; i < 50; ++i) {
    l3.log(qutil::io::log_level::ERROR, "example");
  }
  for(std::size_t i = 0; i < 50; ++i) {
    l3.log(qutil::io::log_level::WARNING, "example");
  }
}