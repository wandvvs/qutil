#include <functional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>

namespace qutil {
template <class T, class E>
struct result;

template <class T>
struct is_result : std::false_type {};

template <class T, class E>
struct is_result<result<T, E>> : std::true_type {};

template <class T>
inline constexpr bool is_result_v = is_result<T>::value;

namespace detail {
template <class T>
struct ok {
  T value;

  template <class U = T>
  explicit constexpr ok(U&& value) noexcept(std::is_nothrow_constructible_v<T, U&&>) : value(std::forward<U>(value)) {
  }

  constexpr auto take() && noexcept -> T&& {
    return std::move(value);
  }

  constexpr auto get() const& noexcept -> const T& {
    return value;
  }
  constexpr auto get() & noexcept -> T& {
    return value;
  }
};

template <>
struct ok<void> {
  constexpr ok() noexcept = default;
  constexpr void take() const noexcept {
  }
  constexpr void get() const noexcept {
  }
};

template <class E>
struct err {
  E value;

  template <class U = E>
  constexpr explicit err(U&& value) noexcept(std::is_nothrow_constructible_v<E, U&&>) : value(std::forward<E>(value)) {
  }

  constexpr auto take() && noexcept -> E&& {
    return std::move(value);
  }
  constexpr auto get() & noexcept -> E& {
    return value;
  }
  constexpr auto get() const& noexcept -> const E& {
    return value;
  }
};

template <>
struct err<void> {
  constexpr err() noexcept = default;
  constexpr void take() const noexcept {
  }
  constexpr void get() const noexcept {
  }
};

}  // namespace detail

template <class T, class E>
class result {
  std::variant<detail::ok<T>, detail::err<E>> storage_;

 public:
  using ok_type = T;
  using err_type = E;
  template <class U>
  constexpr result(detail::ok<U>&& ok) noexcept
    requires(std::is_constructible_v<T, U &&> || std::is_void_v<T>)
      :
      storage_([](detail::ok<U>&& o) -> std::variant<detail::ok<T>, detail::err<E>> {
        if constexpr(std::is_void_v<T>) {
          return detail::ok<void>();
        } else {
          return detail::ok<T>(std::move(o).take());
        }
      }(std::move(ok))) {
  }

  template <class U>
  constexpr result(detail::err<U>&& err) noexcept
    requires(std::is_constructible_v<E, U &&> || std::is_void_v<E>)
      :
      storage_([](detail::err<U>&& o) -> std::variant<detail::ok<T>, detail::err<E>> {
        if constexpr(std::is_void_v<E>) {
          return detail::err<void>();
        } else {
          return detail::err<E>(std::move(o).take());
        }
      }(std::move(err))) {
  }

  template <class F>
  constexpr auto and_then(F&& f) const& {
    using next_result = decltype(std::declval<F>()(std::declval<ok_type>()));
    static_assert(is_result_v<next_result>, "Function must return a result type");

    if(is_err()) {
      return next_result(detail::err<err_type>(err()));
    }

    if constexpr(std::is_void_v<ok_type>) {
      return f();
    } else {
      return f(ok());
    }
  }

  template <class F>
  constexpr auto and_then(F&& f) && {
    using next_result = decltype(std::declval<F>()(std::declval<ok_type>()));
    static_assert(is_result_v<next_result>, "Function must return a result type");

    if(is_err()) {
      return next_result(detail::err<err_type>(std::move(*this).err()));
    }

    if constexpr(std::is_void_v<ok_type>) {
      return f();
    } else {
      return f(std::move(*this).ok());
    }
  }

  template <class U>
  constexpr auto value_or(U&& default_val) const& {
    return is_ok() ? ok() : static_cast<T>(std::forward<U>(default_val));
  }

  constexpr auto is_ok() const noexcept {
    return std::holds_alternative<detail::ok<T>>(storage_);
  }
  constexpr auto is_err() const noexcept {
    return !is_ok();
  }
  constexpr explicit operator bool() {
    return is_ok();
  }
  constexpr auto ok() const& {
    if constexpr(!std::is_void_v<T>) {
      if(auto* val = std::get_if<0>(&storage_)) {
        return val->get();
      }
      throw std::runtime_error("Bad result access");
    } else {
      if(is_ok()) {
        return;
      }
      throw std::runtime_error("Bad result access");
    }
  }

  constexpr auto ok() && {
    if constexpr(!std::is_void_v<T>) {
      if(auto* val = std::get_if<0>(&storage_)) {
        return std::move(*val).take();
      }
      throw std::runtime_error("Bad result access");
    } else {
      if(is_ok()) {
        return;
      }
      throw std::runtime_error("Bad result access");
    }
  }

  constexpr auto err() const& {
    if constexpr(!std::is_void_v<E>) {
      if(auto* val = std::get_if<1>(&storage_)) {
        return val->get();
      }
      throw std::runtime_error("Bad result access");
    } else {
      if(is_err()) {
        return;
      }
      throw std::runtime_error("Bad result access");
    }
  }

  constexpr auto err() && {
    if constexpr(!std::is_void_v<E>) {
      if(auto* val = std::get_if<1>(&storage_)) {
        return std::move(*val).take();
      }
      throw std::runtime_error("Bad result access");
    } else {
      if(is_err()) {
        return;
      }
      throw std::runtime_error("Bad result access");
    }
  }
};

template <class TT>
constexpr auto ok(TT&& value) noexcept {
  return detail::ok<std::unwrap_ref_decay_t<TT>>(std::forward<TT>(value));
}

constexpr auto ok() noexcept {
  return detail::ok<void>();
}

template <class EE>
constexpr auto err(EE&& value) noexcept {
  return detail::err<std::unwrap_ref_decay_t<EE>>(std::forward<EE>(value));
}

constexpr auto err() noexcept {
  return detail::err<void>();
}

}  // namespace qutil