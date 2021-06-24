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
    explicit Error(const char *s) noexcept
        : errnum_{errno}
    {
        std::snprintf(desc_, sizeof(desc_),
                      "%s: %s", s, std::strerror(errnum_));
    };

    const char *what() const noexcept override
    {
        return desc_;
    }

    int errnum() const { return errnum_; }

private:
    int  errnum_;
    char desc_[128];
};

template <typename F, typename... Ts>
inline auto __invoke(const char *name, F&& f, Ts&&... params)
{
    auto ret = f(std::forward<Ts>(params)...);
    if (ret == -1) throw sys::Error{name};
    return ret;
}

} // namespace sys

#define SYS_INV(f, ...) sys::__invoke(#f, ::f, __VA_ARGS__)

#endif // L4SL694ZCOEI59XOLN1D8ZAP18YA2ZWE0K7SNB6B
