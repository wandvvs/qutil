#include "qutil/io/logger.hpp"

#include <algorithm>
#include <cassert>
#include <format>
#include <iostream>
#include <thread>
#include <vector>

static void log(qutil::io::ostream_logger& lger) {
  std::size_t id = std::hash<std::thread::id>{}(std::this_thread::get_id());
  for(std::size_t i = 0; i < 25; ++i) {
    lger.log(qutil::io::log_level::INFO, std::format("thread {}", id));
  }
}

int main() {
  // auto& l3 = qutil::io::file_logger::get_logger("log.txt");
  auto& l3 = qutil::io::ostream_logger::get_logger(std::cout);
  l3.set_level(qutil::io::log_level::INFO);

  std::vector<std::thread> ts;
  for(std::size_t i = 0; i < 3; ++i) {
    ts.emplace_back([&l3]() {
      std::size_t id = std::hash<std::thread::id>{}(std::this_thread::get_id());
      for(std::size_t i = 0; i < 25; ++i) {
        l3.log(qutil::io::log_level::INFO, std::format("thread {}", id));
      }
    });
  }

  std::for_each(ts.begin(), ts.end(), [](std::thread& t) {
    t.join();
  });

  // l3.log(qutil::io::log_level::ERROR, "example");

  // l3.set_level(qutil::io::log_level::ERROR);

  // l3.log(qutil::io::log_level::WARNING, "example");
}