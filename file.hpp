#ifndef CD2KM3B0MVL3AROMJMJL2CGIFXOS2SKCJYMPJRVJ
#define CD2KM3B0MVL3AROMJMJL2CGIFXOS2SKCJYMPJRVJ

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <utility>

#include "error.hpp"

namespace sys {

/*
 * Errors
 */

struct Open_error : public Error
{
    explicit Open_error(const char* fn) : file_name{fn} {}
    const std::string file_name;
};

struct Mkstemp_error : public Error
{
    explicit Mkstemp_error(const char* ts) : tmpl{ts} {}
    const std::string tmpl;
};

struct Close_error : public Error {};

/*
 * File descriptor
 */

class File_des
{
public:
    constexpr explicit File_des(int fd = -1) noexcept : des_{fd} {}

    constexpr int get()     const noexcept { return des_; }
    constexpr int isValid() const noexcept { return (des_ >= 0); }

    void swap(File_des& rhs) noexcept
        { std::swap(des_, rhs.des_); }

    friend void close(File_des&);

private:
    int des_;
};

inline void swap(File_des& lhs, File_des& rhs) noexcept
    { lhs.swap(rhs); }

inline void close(File_des& f)
{
    if (!f.isValid()) return;
    SYS_INV(::close, Close_error, f.get());
    f.des_ = -1;
}

constexpr sys::File_des STDIN {STDIN_FILENO},
                        STDOUT{STDOUT_FILENO},
                        STDERR{STDERR_FILENO};

/*
 * File
 */

class File : public File_des
{
public:
    explicit File()       noexcept {}
    explicit File(int fd) noexcept : File_des{fd} {}

    File(File&)            = delete;
    File& operator=(File&) = delete;

    File(File&& f)            noexcept { swap(f); }
    File &operator=(File&& f) noexcept { swap(f); return *this; }

    ~File() { if (isValid()) ::close(get()); }
};

inline auto open(const char* name, int flags)
{
    auto fd = ::open(name, flags);
    if (fd == -1) throw Open_error{name};
    return File{fd};
}

inline auto open(const char* name, int flags, mode_t mode)
{
    auto fd = ::open(name, flags, mode);
    if (fd == -1) throw Open_error{name};
    return File{fd};
}

inline auto creat(const char* name, mode_t mode)
{
    auto fd = ::creat(name, mode);
    if (fd == -1) throw Open_error{name};
    return File{fd};
}

inline auto mkstemp(char *tmpl)
{
    auto fd = ::mkstemp(tmpl);
    if (fd == -1) throw Mkstemp_error{tmpl};
    return File{fd};
}

inline auto mkostemp(char *tmpl, int flags)
{
    auto fd = ::mkostemp(tmpl, flags);
    if (fd == -1) throw Mkstemp_error{tmpl};
    return File{fd};
}

inline auto mkstemps(char *tmpl, int suffixlen)
{
    auto fd = ::mkstemps(tmpl, suffixlen);
    if (fd == -1) throw Mkstemp_error{tmpl};
    return File{fd};
}

inline auto mkostemps(char *tmpl, int suffixlen, int flags)
{
    auto fd = ::mkostemps(tmpl, suffixlen, flags);
    if (fd == -1) throw Mkstemp_error{tmpl};
    return File{fd};
}

} // namespace sys

#endif // CD2KM3B0MVL3AROMJMJL2CGIFXOS2SKCJYMPJRVJ
