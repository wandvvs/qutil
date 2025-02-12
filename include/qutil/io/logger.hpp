#pragma once

// #include <algorithm>
#include <chrono>
#include <cstring>
#include <ctime>
#include <format>
#include <iomanip>
#include <source_location>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <utility>

#include "../containers/tuple.hpp"
#include "sink.hpp"

namespace qutil::io {

template <typename... S>
  requires(std::is_base_of_v<sink, S> && ...)
class logger {
  using sinks_t = qutil::containers::tuple<S...>;

 public:
  logger(const logger&) = delete;
  logger(logger&&) = delete;
  logger& operator=(const logger&) = delete;
  logger& operator=(logger&&) = delete;

  void log(log_level level, std::string_view message, const std::source_location& loc = std::source_location::current()) {
    std::string result_message = std::format(
        "{0:%F %R} {1} at {2} > {3}\n", std::chrono::system_clock::now(), level_to_string(level), source_location_str(loc), message);

    qutil::containers::for_each(sinks_, [&result_message, &level](auto& elem) {
      elem.log({result_message, level});
    });
  }

  void set_level(log_level new_level) {
    qutil::containers::for_each(sinks_, [new_level](auto& elem) {
      elem.set_level(new_level);
    });
  }

  template <typename... SinkArgs>
  static logger& get_logger(SinkArgs&&... sink_args) noexcept {
    static logger<S...> logr = logger<S...>(S(std::forward<SinkArgs>(sink_args))...);
    return logr;
  }

  ~logger() = default;

 private:
  explicit logger(S... sinks) : sinks_(qutil::containers::make_tuple(std::move(sinks)...)) {
  }

  std::string source_location_str(const std::source_location& loc) {
    return std::format("{}:{}:{}:", loc.file_name(), loc.line(), loc.column());
  }

  sinks_t sinks_;
};

using file_logger = logger<file_sink<2024>>;
using ostream_logger = logger<ostream_sink>;

}  // namespace qutil::io