#pragma once

// #include <algorithm>
#include <chrono>
#include <cstring>
#include <ctime>
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

  void log(log_level level,
           std::string_view message,
           const std::source_location& loc = std::source_location::current()) {  // TODO(sue): make thread-safe
    const auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::ostringstream result_message;
    result_message << (std::put_time(std::localtime(&now_time), "%F %T")) << " " << level_to_string(level);
    result_message << " at " << source_location_str(loc);
    result_message << " > " << message << "\n";

    qutil::containers::for_each(sinks_, [&result_message, &level](auto& elem) {
      elem.log({result_message.str(), level});
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
    std::stringstream r;
    r << loc.file_name() << ":" << loc.line() << ":" << loc.column() << ":";

    return r.str();
  }

  sinks_t sinks_;
};

using file_logger = logger<file_sink<2024>>;
using ostream_logger = logger<ostream_sink>;

}  // namespace qutil::io