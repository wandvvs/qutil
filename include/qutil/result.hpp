#include <utility>
#include <variant>

namespace qutil {
template <class OkT, class ErrT>
struct result;

template <class T>
struct ok {
  T value_;

  explicit constexpr ok(const T& value) : value_(value) {
  }

  explicit constexpr ok(T&& value) : value_(std::move(value)) {
  }

  [[nodiscard]] constexpr auto value() -> decltype(auto) {
    return value_;
  }

  template <class Err>
  constexpr operator result<T, Err>() const {
    return result<T, Err>(ok(value_));
  }
};

template <class T>
struct err {
  T value_;

  explicit constexpr err(const T& value) : value_(value) {
  }

  explicit constexpr err(T&& value) : value_(std::move(value)) {
  }

  [[nodiscard]] constexpr auto value() -> decltype(auto) {
    return value_;
  }

  template <class Ok>
  constexpr operator result<Ok, T>() const {
    return result<Ok, T>(err(value_));
  }
};

template <class OkT, class ErrT>
struct result {
  std::variant<ok<OkT>, err<ErrT>> variant_;

  constexpr result(ok<OkT> value) : variant_(std::move(value)) {
  }
  constexpr result(err<ErrT> value) : variant_(std::move(value)) {
  }

  [[nodiscard]] constexpr auto is_okay() const -> bool {
    return std::holds_alternative<ok<OkT>>(variant_);
  }

  [[nodiscard]] constexpr auto is_err() const -> bool {
    return std::holds_alternative<err<ErrT>>(variant_);
  }

  [[nodiscard]] constexpr auto ok_value() const -> OkT {
    return std::get<ok<OkT>>(variant_).value_;
  }

  [[nodiscard]] constexpr auto err_value() const -> ErrT {
    return std::get<err<ErrT>>(variant_).value_;
  }

  constexpr auto take_ok_value() -> OkT&& {
    return std::get<ok<OkT>>(variant_).take_value();
  }

  constexpr auto take_err_value() -> ErrT&& {
    return std::get<err<ErrT>>(variant_).take_value();
  }
};

}  // namespace qutil
