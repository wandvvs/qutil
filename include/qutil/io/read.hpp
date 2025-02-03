#pragma once

#include <fstream>
#include <string>
#include <system_error>

namespace qutil::io {
[[nodiscard]] auto read(std::string_view source) -> std::string {
  std::ifstream file(source.data(), std::ios::binary | std::ios::ate);

  if(!file.is_open()) {
    throw std::system_error(errno, std::system_category(), "Failed to open file");
  }

  const auto file_size = file.tellg();
  if(file_size == -1) {
    throw std::system_error(errno, std::system_category(), "Failed to get file size");
  }

  file.seekg(0, std::ios::beg);

  std::string content;
  content.resize(static_cast<size_t>(file_size));

  if(!file.read(content.data(), file_size)) {
    throw std::system_error(errno, std::system_category(), "Failed to read file content");
  }

  return content;
}
}  // namespace qutil::io