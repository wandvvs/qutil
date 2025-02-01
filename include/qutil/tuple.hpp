#pragma once

#include <functional>
#include <type_traits>
#include <utility>

namespace qutil {
template <auto Value>
struct wrapper {
  static constexpr auto value = Value;
};

template <class...>
class tuple;

template <>
class tuple<> {
 public:
  constexpr tuple() = default;

  template <typename... Ts>
  constexpr auto operator+(const tuple<Ts...> &other) -> bool {
    return other;
  }

  constexpr auto operator==(const tuple<> &) -> bool {
    return true;
  }
};

template <class T>
struct tuple_size;

template <class... Types>
struct tuple_size<tuple<Types...>> : std::integral_constant<std::size_t, sizeof...(Types)> {};

template <class... Types>
struct tuple_size<const tuple<Types...>> : tuple_size<tuple<Types...>> {};

template <class... Types>
struct tuple_size<volatile tuple<Types...>> : tuple_size<tuple<Types...>> {};

template <class... Types>
struct tuple_size<const volatile tuple<Types...>> : tuple_size<tuple<Types...>> {};

template <class T>
constexpr std::size_t tuple_size_v = tuple_size<T>::value;

template <std::size_t I, typename T>
struct tuple_element;

template <std::size_t I, typename Head, typename... Tail>
struct tuple_element<I, tuple<Head, Tail...>> : tuple_element<I - 1, tuple<Tail...>> {};

template <std::size_t I, typename Head, typename... Tail>
struct tuple_element<I, const tuple<Head, Tail...>> : tuple_element<I - 1, const tuple<Tail...>> {};

template <std::size_t I, typename Head, typename... Tail>
struct tuple_element<I, volatile tuple<Head, Tail...>> : tuple_element<I - 1, volatile tuple<Tail...>> {};

template <std::size_t I, typename Head, typename... Tail>
struct tuple_element<I, const volatile tuple<Head, Tail...>> : tuple_element<I - 1, const volatile tuple<Tail...>> {};

template <typename Head, typename... Tail>
struct tuple_element<0, tuple<Head, Tail...>> {
  using type = Head;
};

template <typename Head, typename... Tail>
struct tuple_element<0, const tuple<Head, Tail...>> {
  using type = const Head;
};

template <typename Head, typename... Tail>
struct tuple_element<0, volatile tuple<Head, Tail...>> {
  using type = volatile Head;
};

template <typename Head, typename... Tail>
struct tuple_element<0, const volatile tuple<Head, Tail...>> {
  using type = const volatile Head;
};

template <class Head, class... Tail>
class tuple<Head, Tail...> {
 public:
  template <class H, class... T>
    requires(sizeof...(T) == sizeof...(Tail) && sizeof(H) == sizeof(Head))
  constexpr explicit tuple(H &&head, T &&...tail) : head_(std::forward<H>(head)), tail_(std::forward<T>(tail)...) {
  }

  constexpr explicit tuple(const Head &head, const Tail &...tail) : head_(head), tail_(tail...) {
  }

  constexpr tuple(const tuple &) = default;
  constexpr tuple(tuple &&) = default;

  constexpr auto operator==(const tuple<Head, Tail...> &other) const -> bool {
    return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
      return (... && (get<Is>(*this) == get<Is>(other)));
    }(std::index_sequence_for<Head, Tail...>());
  }

  template <typename... TTs>
  constexpr auto operator+(const tuple<TTs...> &other) const -> tuple<Head, Tail..., TTs...> {
    return [&]<std::size_t... Is, std::size_t... IIs>(std::index_sequence<Is...>, std::index_sequence<IIs...>) {
      return tuple<Head, Tail..., TTs...>(get<Is>(*this)..., get<IIs>(other)...);
    }(std::index_sequence_for<Head, Tail...>(), std::index_sequence_for<TTs...>());
  }

  template <auto Index>
  constexpr auto &operator[](wrapper<Index>) {
    return get<Index>(*this);
  }

  template <auto Index>
  constexpr const auto &operator[](wrapper<Index>) const {
    return get<Index>(*this);
  }

 private:
  template <std::size_t I, typename... Types>
  friend constexpr decltype(auto) get(tuple<Types...> &t);

  template <std::size_t I, typename... Types>
  friend constexpr decltype(auto) get(const tuple<Types...> &t);

  template <std::size_t I, typename... Types>
  friend constexpr decltype(auto) get(tuple<Types...> &&t);

  template <std::size_t I, typename... Types>
  friend constexpr decltype(auto) get(const tuple<Types...> &&t);

  template <typename T, typename H, typename... Types>
  friend constexpr decltype(auto) get(tuple<H, Types...> &t) noexcept;

  template <typename T, typename H, typename... Types>
  friend constexpr decltype(auto) get(const tuple<H, Types...> &t) noexcept;

  template <typename T, typename H, typename... Types>
  friend constexpr decltype(auto) get(tuple<H, Types...> &&t) noexcept;

  template <typename T, typename H, typename... Types>
  friend constexpr decltype(auto) get(const tuple<H, Types...> &&t) noexcept;

 private:
  Head head_;
  tuple<Tail...> tail_;
};

template <std::size_t I, typename... Types>
constexpr decltype(auto) get(tuple<Types...> &t) {
  if constexpr(I == 0) {
    return (t.head_);
  } else {
    return get<I - 1>(t.tail_);
  }
}

template <std::size_t I, typename... Types>
constexpr decltype(auto) get(const tuple<Types...> &t) {
  if constexpr(I == 0) {
    return (t.head_);
  } else {
    return get<I - 1>(t.tail_);
  }
}

template <std::size_t I, typename... Types>
constexpr decltype(auto) get(tuple<Types...> &&t) {
  if constexpr(I == 0) {
    return std::move(t.head_);
  } else {
    return get<I - 1>(std::move(t.tail_));
  }
}

template <std::size_t I, typename... Types>
constexpr decltype(auto) get(const tuple<Types...> &&t) {
  if constexpr(I == 0) {
    return std::move(t.head_);
  } else {
    return get<I - 1>(std::move(t.tail_));
  }
}

template <typename T, typename Head, typename... Types>
constexpr decltype(auto) get(tuple<Head, Types...> &t) noexcept {
  if constexpr(std::is_same_v<T, Head>) {
    return (t.head_);
  } else {
    return get<T>(t.tail_);
  }
}

template <typename T, typename Head, typename... Types>
constexpr decltype(auto) get(const tuple<Head, Types...> &t) noexcept {
  if constexpr(std::is_same_v<T, Head>) {
    return (t.head_);
  } else {
    return get<T>(t.tail_);
  }
}

template <typename T, typename Head, typename... Types>
constexpr decltype(auto) get(tuple<Head, Types...> &&t) noexcept {
  if constexpr(std::is_same_v<T, Head>) {
    return std::move(t.head_);
  } else {
    return get<T>(std::move(t.tail_));
  }
}

template <typename T, typename Head, typename... Types>
constexpr decltype(auto) get(const tuple<Head, Types...> &&t) noexcept {
  if constexpr(std::is_same_v<T, Head>) {
    return std::move(t.head_);
  } else {
    return get<T>(std::move(t.head_));
  }
}

template <class... Types>
constexpr auto make_tuple(Types &&...args) -> decltype(auto) {
  return qutil::tuple<std::unwrap_ref_decay_t<Types>...>(std::forward<Types>(args)...);
}

template <class... Types>
constexpr auto tie(Types &...args) -> decltype(auto) {
  return tuple<Types &...>(args...);
}

template <class... Types>
constexpr auto forward_as_tuple(Types &&...args) -> decltype(auto) {
  return tuple<Types &&...>(std::forward<Types>(args)...);
}

template <class Tuple, class F>
constexpr auto for_each(Tuple &&tuple, F &&f) -> decltype(auto) {
  return []<std::size_t... Is>(Tuple &&tuple, F &&f, std::index_sequence<Is...>) {
    (std::invoke(std::forward<F>(f), get<Is>(std::forward<Tuple>(tuple))), ...);
    return f;
  }(std::forward<Tuple>(tuple), std::forward<F>(f), std::make_index_sequence<tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

template <class Tuple, class F>
constexpr auto transform(Tuple &&tuple, F &&f) -> decltype(auto) {
  return []<std::size_t... Is>(Tuple &&tuple, F &&f, std::index_sequence<Is...>) {
    return make_tuple(std::forward<F>(f)(get<Is>(std::forward<Tuple>(tuple)))...);
  }(std::forward<Tuple>(tuple), std::forward<F>(f), std::make_index_sequence<tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

template <class Tuple, class Predicate>
constexpr auto find_if(Tuple &&tuple, Predicate predicate) -> decltype(auto) {
  std::size_t index = tuple_size_v<std::remove_reference_t<Tuple>>;
  std::size_t curr_index = 0;
  bool found = false;

  for_each(std::forward<Tuple>(tuple), [&](auto &&value) {
    if(!found && predicate(value)) {
      index = curr_index;
      found = true;
    }
    ++curr_index;
  });

  return index;
}

template <typename... Tuples>
constexpr auto tuple_cat(Tuples &&...tuples) {
  return (... + std::forward<Tuples>(tuples));
}

template <class Tuple, typename Predicate>
constexpr auto all_of(Tuple &&tuple, Predicate predicate) -> bool {
  return find_if(std::forward<Tuple>(tuple), std::not_fn(predicate)) == tuple_size_v<std::decay_t<Tuple>>;
}

template <class Tuple, typename Predicate>
constexpr auto none_of(Tuple &&tuple, Predicate predicate) -> bool {
  return find_if(std::forward<Tuple>(tuple), predicate) == tuple_size_v<std::decay_t<Tuple>>;
}

template <class Tuple, typename Predicate>
constexpr auto any_of(Tuple &&tuple, Predicate predicate) -> bool {
  return !none_of(std::forward<Tuple>(tuple), predicate);
}

template <class F, class T>
constexpr auto apply(F &&f, T &&t) -> decltype(auto) {
  return []<class Func, class Tuple, std::size_t... Is>(Func &&func, Tuple &&tuple, std::index_sequence<Is...>) {
    return std::invoke(std::forward<Func>(func), get<Is>(std::forward<Tuple>(tuple))...);
  }(std::forward<F>(f), std::forward<T>(t), std::make_index_sequence<tuple_size_v<std::remove_reference_t<T>>>{});
}

template <class... Types>
tuple(Types &&...) -> tuple<Types...>;

}  // namespace qutil
