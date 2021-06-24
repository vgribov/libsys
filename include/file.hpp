#ifndef CD2KM3B0MVL3AROMJMJL2CGIFXOS2SKCJYMPJRVJ
#define CD2KM3B0MVL3AROMJMJL2CGIFXOS2SKCJYMPJRVJ

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <utility>

#include "error.hpp"

namespace sys {

class File_des
{
public:
    constexpr explicit File_des(int fd) noexcept : des_{fd} {}

    constexpr int get() const noexcept { return des_; }

protected:
    int des_;
};

constexpr sys::File_des STDIN {STDIN_FILENO},
                        STDOUT{STDOUT_FILENO},
                        STDERR{STDERR_FILENO};

class File : public File_des
{
public:
    explicit File() noexcept : File_des{-1} {}
    ~File() noexcept { close(); }

    File(File&) = delete;
    File &operator=(File&) = delete;

    File(File&& f) noexcept : File_des{-1} { swap(*this, f); }
    File &operator=(File&& f) noexcept     { swap(*this, f); return *this; }

    friend void swap(File& lhs, File& rhs) noexcept;

    friend File open(const char*, int);
    friend File open(const char*, int, mode_t);
    friend File creat(const char*, mode_t);

    friend File mkstemp(char*);
    friend File mkostemp(char*, int);
    friend File mkstemps(char*, int);
    friend File mkostemps(char*, int, int);

    friend void close(File&);

private:
    explicit File(int fd) noexcept : File_des{fd} {}

    int close() noexcept
    {
        if (des_ == -1) return 0;
        int ret = ::close(des_);
        des_ = -1;
        return ret;
    }
};

inline void swap(File& lhs, File& rhs) noexcept
    { std::swap(lhs.des_, rhs.des_); }

inline File open(const char* name, int flags)
    { return File{SYS_INV(open, name, flags)}; }

inline File open(const char* name, int flags, mode_t mode)
    { return File{SYS_INV(open, name, flags, mode)}; }

inline File creat(const char* name, mode_t mode)
    { return File{SYS_INV(creat, name, mode)}; }

inline File mkstemp(char *tmpl)
    { return File{SYS_INV(mkstemp, tmpl)}; }

inline File mkostemp(char *tmpl, int flags)
    { return File{SYS_INV(mkostemp, tmpl, flags)}; }

inline File mkstemps(char *tmpl, int suffixlen)
    { return File{SYS_INV(mkstemps, tmpl, suffixlen)}; }

inline File mkostemps(char *tmpl, int suffixlen, int flags)
    { return File{SYS_INV(mkostemps, tmpl, suffixlen, flags)}; }

inline void close(File& f)
    { __invoke("close", [&f]() { return f.close(); } ); }


} // namespace sys

#endif // CD2KM3B0MVL3AROMJMJL2CGIFXOS2SKCJYMPJRVJ
