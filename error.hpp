#ifndef L4SL694ZCOEI59XOLN1D8ZAP18YA2ZWE0K7SNB6B
#define L4SL694ZCOEI59XOLN1D8ZAP18YA2ZWE0K7SNB6B

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <system_error>
#include <utility>

namespace sys {

struct Error : public std::system_error
{
    Error()
        : std::system_error{errno, std::system_category()}
        { }
};

template <typename F, typename E, typename... Ts>
inline auto __invoke(F f, Ts&&... params)
{
    auto ret = f(std::forward<Ts>(params)...);
    if (ret == -1) throw E{};
    return ret;
}

} // namespace sys

#define SYS_INV(f, E, ...) sys::__invoke<decltype(f), E>(f, __VA_ARGS__)

#endif // L4SL694ZCOEI59XOLN1D8ZAP18YA2ZWE0K7SNB6B
