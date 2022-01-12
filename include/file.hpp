#ifndef CD2KM3B0MVL3AROMJMJL2CGIFXOS2SKCJYMPJRVJ
#define CD2KM3B0MVL3AROMJMJL2CGIFXOS2SKCJYMPJRVJ

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <utility>

#include "error.hpp"

namespace sys {

class Open_error    : public Error {};
class Creat_error   : public Error {};
class Mkstemp_error : public Error {};
class Close_error   : public Error {};

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

inline File open(const char* name, int flags)
    { return File{SYS_INV(::open, Open_error, name, flags)}; }

inline File open(const char* name, int flags, mode_t mode)
    { return File{SYS_INV(::open, Open_error, name, flags, mode)}; }

inline File creat(const char* name, mode_t mode)
    { return File{SYS_INV(::creat, Creat_error, name, mode)}; }

inline File mkstemp(char *tmpl)
    { return File{SYS_INV(::mkstemp, Mkstemp_error, tmpl)}; }

inline File mkostemp(char *tmpl, int flags)
    { return File{SYS_INV(::mkostemp, Mkstemp_error, tmpl, flags)}; }

inline File mkstemps(char *tmpl, int suffixlen)
    { return File{SYS_INV(::mkstemps, Mkstemp_error, tmpl, suffixlen)}; }

inline File mkostemps(char *tmpl, int suffixlen, int flags)
    { return File{SYS_INV(::mkostemps, Mkstemp_error, tmpl, suffixlen, flags)}; }

} // namespace sys

#endif // CD2KM3B0MVL3AROMJMJL2CGIFXOS2SKCJYMPJRVJ
