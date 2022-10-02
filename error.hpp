#ifndef L4SL694ZCOEI59XOLN1D8ZAP18YA2ZWE0K7SNB6B
#define L4SL694ZCOEI59XOLN1D8ZAP18YA2ZWE0K7SNB6B

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <system_error>
#include <string>
#include <sstream>
#include <utility>

namespace sys {

struct Error : public std::system_error
{
    Error()
        : std::system_error{errno, std::system_category()}
        { }

    Error(const std::string& what_arg)
        : std::system_error{errno, std::system_category(), what_arg}
        { }
};

template <typename... T>
inline auto error(const T&... args) {
    std::stringstream serr;
    (serr << ... << args);
    return Error{serr.str()};
}

template <typename T>
inline auto error(const T& msg) {
    return Error{msg};
}

template <typename F, typename... Ts>
inline auto __invoke(F f, const char* msg, Ts&&... params)
{
    auto ret = f(std::forward<Ts>(params)...);
    if (ret == -1) throw Error{msg};
    return ret;
}

} // namespace sys

#define SYS_INV(f, ...) sys::__invoke(::f, #f, __VA_ARGS__)

#endif // L4SL694ZCOEI59XOLN1D8ZAP18YA2ZWE0K7SNB6B
