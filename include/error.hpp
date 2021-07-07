#ifndef L4SL694ZCOEI59XOLN1D8ZAP18YA2ZWE0K7SNB6B
#define L4SL694ZCOEI59XOLN1D8ZAP18YA2ZWE0K7SNB6B

#include <exception>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <utility>

namespace sys {

class Error : public std::exception {

public:
    explicit Error() noexcept
        : errnum_{errno}
        , desc_{std::strerror(errno)}
        { };

    const char *what() const noexcept override
        { return desc_; }

    int errnum() const
        { return errnum_; }

private:
    int  errnum_;
    const char *desc_;
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
