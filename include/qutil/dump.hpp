#include <iostream>
#include <ostream>
#include <sstream>
#include <utility>

namespace qutil {

namespace detail {

template <typename T>
concept is_iterable = requires(T e) {
  e.begin();
  e.end();
};

template <typename T>
concept is_printable = requires(T t) {
  { std::cout << t } -> std::same_as<std::ostream&>;
};

template <typename T>
struct dump_type {
  void operator()(const T element, std::ostringstream& ss)
    requires is_iterable<T>
  {
    ss << "{ ";

    for(const auto e : element) {
      this->operator()(std::forward<decltype(e)>(e), ss);
      ss << ", ";
    }
    ss << "}, ";
  }

  void operator()(const T element, std::ostringstream& ss)
    requires is_printable<T>
  {
    ss << element;
  }

  // private:
  template <typename E>
  void operator()(const E& element, std::ostringstream& ss)
    requires is_iterable<E>
  {
    ss << "{ ";

    for(const auto& e : element) {
      this->operator()(std::forward<decltype(e)>(e), ss);
      ss << ", ";
    }
    ss << "}, ";
  }

  template <typename E>
  void operator()(const E& element, std::ostringstream& ss)
    requires is_printable<E>
  {
    ss << element;
  }
};
}  // namespace detail

template <typename... Args>
struct dump : detail::dump_type<Args>... {
  using detail::dump_type<Args>::operator()...;

  explicit dump(Args&&... elements) {
    data_ << "[ ";
    (detail::dump_type<Args>::operator()(std::forward<Args>(elements), data_), ...);
    data_ << " ]";
  }

  std::ostringstream& data() {
    return data_;
  }

  std::string str() {
    return data_.str();
  }

  operator std::ostringstream() && {
    return std::ostringstream(data_);  // todo: delete copying
  }

 private:
  std::ostringstream data_;
};

template <typename... Args>
dump(Args&&... elements) -> dump<Args...>;

}  // namespace qutil