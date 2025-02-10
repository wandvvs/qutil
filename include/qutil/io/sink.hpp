#pragma once

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string_view>
#include <type_traits>
#include <utility>

namespace qutil::io {

enum class log_level : std::size_t { DEBUG, INFO, WARNING, ERROR };

constexpr auto level_to_underlying(log_level level) {
  return static_cast<std::underlying_type_t<log_level>>(level);
}

// ERROR is always last level
constexpr std::size_t log_level_count = level_to_underlying(log_level::ERROR) + 1;

constexpr std::string_view level_to_string(log_level level) {
  const std::array<std::string_view, log_level_count> levels{"DEBUG", "INFO", "WARNING", "ERROR"};
  return levels.at(level_to_underlying(level));
}

struct log_message {
  std::string_view str;
  log_level level{log_level::INFO};
};

class sink {
 public:
  sink(sink&&) = delete;
  sink& operator=(sink&&) = delete;
  sink(const sink&) = default;

  void log(log_message message) {
    if(message.level >= level_) {
      write(message.str);
    }
  }

  virtual ~sink() = default;

  void set_level(log_level new_level) {
    level_ = new_level;
  }

 protected:
  virtual void flush() = 0;
  virtual void write(std::string_view str) = 0;

  sink() = default;

 private:
  log_level level_{log_level::INFO};
};

class ostream_sink : public sink {
 public:
  explicit ostream_sink(std::ostream& os) : os_(os) {
  }

  ~ostream_sink() override = default;

 protected:
  void write(std::string_view str) override {
    os_ << str;
  }

  void flush() override {
    os_.flush();
  };

 private:
  std::ostream& os_;
};

template <std::size_t BufferSize = 4096>
class file_sink : public sink {  // TODO(sue): async_file_sink
 public:
  file_sink() = delete;
  file_sink(const file_sink&) = delete;
  file_sink& operator=(const file_sink&) = delete;
  file_sink(file_sink&& other) noexcept : os_(std::move(other.os_)), custom_buffer_(std::move(other.custom_buffer_)) {
  }
  file_sink& operator=(file_sink&& other) {
    os_.close();
    new(this) file_sink<BufferSize>(std::move(other));
  }

  explicit file_sink(const std::filesystem::path& path) : os_(path) {
    if(!os_.fail()) {
      os_.rdbuf()->pubsetbuf(custom_buffer_.data(), BufferSize);
    }
  }

  ~file_sink() override = default;

 protected:
  void write(std::string_view str) override {
    if(os_.good()) {
      os_ << str;
    }
  }

  void flush() override {
    os_.flush();
  };

 private:
  std::array<char, BufferSize> custom_buffer_;
  std::ofstream os_;
};

}  // namespace qutil::io
