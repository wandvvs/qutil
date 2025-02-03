<h1 align="center">🛠️ qutil</h1>
<h3 align="center">Provides modern and useful containers and algorithms</h3>

<p align="center">
     <a href="https://github.com/wandvvs/qutil/issues">
     <img src="https://img.shields.io/github/issues/wandvvs/qutil"
          alt="GitHub opened issues">
     <img src="https://img.shields.io/badge/status-in_development-red"
          alt="Status">
     <img src="https://img.shields.io/github/stars/wandvvs/qutil?color=lime"
          alt="Stars">
</p>

 ## **📥 Installation**
```bash
git clone https://github.com/wandvvs/qutil.git
cd qutil
sudo ./install.sh
```
For an example of how to use qutil in a CMake project, see the following [CMakeLists.txt](https://github.com/wandvvs/qutil/blob/master/examples/tuple/CMakeLists.txt)

## **📦 Modules**

### 🧰 `qutil::containers`
- `tuple<Args...>` - tuple implementation with additional support functions: `for_each`, `transform`, `find_if`, `all_of`, `none_of`, `any_of`.
- `constexpr_string<Size>` - string for working at compile time.
- `result<T, E>` - result class for error handling like Rust `Result<T, E`.

### 📁 `qutil::io`
- `dump(Args...)` - pretty print function for a values and iterable types like a Python `print` to the stream (`std::cout`) by default.

Check out [examples](https://github.com/wandvvs/qutil/tree/master/examples) of all the qutil features.
