#include <iostream>
#include <qutil/result.hpp>

struct division_error {
  enum class error_kind { division_by_zero };

  explicit division_error(error_kind kind, std::string reason = "") : kind(kind), reason(std::move(reason)) {
  }

  error_kind kind;
  std::string reason;
};

auto division(float x, float y) -> qutil::result<float, division_error> {
  if(y == 0) {
    return qutil::err(division_error(division_error::error_kind::division_by_zero, "Division by zero"));
  }
  return qutil::ok(x / y);
}

auto main() -> int {
  auto div = division(5, 0);  // NOLINT

  if(div.is_okay()) {
    std::cout << div.ok_value() << std::endl;
  } else if(div.is_err()) {
    std::cout << div.err_value().reason;
  }
}
