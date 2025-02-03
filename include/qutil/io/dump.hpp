#pragma once

#include <iostream>
#include <qutil/containers/tuple.hpp>
#include <string>

namespace qutil::io {
template <typename T>
concept Iterable = !std::is_same_v<std::remove_cvref_t<T>, std::string> && requires(T collection) {
  collection.begin();
  collection.end();
};

namespace detail {
template <typename T>
void print_element(std::ostream& os, T&& element) {
  if constexpr(Iterable<std::remove_cvref_t<T>>) {
    os << "[";
    bool inner_first = true;
    for(auto&& i : element) {
      if(!inner_first) {
        os << ", ";
      }
      inner_first = false;
      os << i;
    }
    os << "]";
  } else {
    os << std::forward<T>(element);
  }
}
}  // namespace detail

template <class First, class... Args>
void dump(First&& first, Args&&... args) {
  if constexpr(std::is_base_of_v<std::ostream, std::remove_reference_t<First>>) {
    std::ostream& os = first;
    if constexpr(sizeof...(args) == 0) {
      os << "\n";
    } else if constexpr(sizeof...(args) == 1) {
      qutil::containers::tuple t(args...);
      qutil::containers::for_each(t, [&os](auto&& arg) {
        detail::print_element(os, std::forward<decltype(arg)>(arg));
      });
      os << "\n";
    } else {
      qutil::containers::tuple t(args...);
      os << "[";
      bool first_elem = true;
      qutil::containers::for_each(t, [&os, &first_elem](auto&& arg) {
        if(!first_elem) {
          os << ", ";
        }
        first_elem = false;
        detail::print_element(os, std::forward<decltype(arg)>(arg));
      });
      os << "]\n";
    }
  } else {
    if constexpr(sizeof...(args) == 0) {
      detail::print_element(std::cout, first);
      std::cout << "\n";
    } else {
      dump(std::cout, std::forward<First>(first), std::forward<Args>(args)...);
    }
  }
}

}  // namespace qutil::io