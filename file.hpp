#ifndef CD2KM3B0MVL3AROMJMJL2CGIFXOS2SKCJYMPJRVJ
#define CD2KM3B0MVL3AROMJMJL2CGIFXOS2SKCJYMPJRVJ

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <utility>

#include "error.hpp"
#include "utils.hpp"

namespace sys {

/*
 * File descriptor
 */

class File_des {
public:
    constexpr explicit File_des(int fd = -1) noexcept : des_{fd} {}

    constexpr int get()     const noexcept { return des_; }
    constexpr int isValid() const noexcept { return (des_ >= 0); }

    void swap(File_des& rhs) noexcept {
        std::swap(des_, rhs.des_);
    }

    friend void close(File_des&);

private:
    int des_;
};

inline void swap(File_des& lhs, File_des& rhs) noexcept {
    lhs.swap(rhs);
}

inline void close(File_des& f) {
    SYS_INV(close, f.get());
    f.des_ = -1;
}

constexpr sys::File_des STDIN {STDIN_FILENO},
                        STDOUT{STDOUT_FILENO},
                        STDERR{STDERR_FILENO};

/*
 * File
 */

class File : public File_des, utils::No_copy {
public:
    explicit File()       noexcept : File_des{}   {}
    explicit File(int fd) noexcept : File_des{fd} {}

    File(File&& rhs) noexcept : File{} { swap(rhs); }

    File& operator=(File&& rhs) noexcept {
        File tmp{};
        rhs.swap(tmp);
        swap(tmp);
        return *this;
    }

    ~File() { if (isValid()) ::close(get()); }
};

inline auto open(const char* name, int flags) {
    return File{ SYS_INV(open, name, flags) };
}

inline auto open(const char* name, int flags, mode_t mode) {
    return File{ SYS_INV(open, name, flags, mode) };
}

inline auto creat(const char* name, mode_t mode) {
    return File{ SYS_INV(creat, name, mode) };
}

inline auto mkstemp(char *tmpl) {
    return File{ SYS_INV(mkstemp, tmpl) };
}

inline auto mkostemp(char *tmpl, int flags) {
    return File{ SYS_INV(mkostemp, tmpl, flags) };
}

inline auto mkstemps(char *tmpl, int suffixlen) {
    return File{ SYS_INV(mkstemps, tmpl, suffixlen) };
}

inline auto mkostemps(char *tmpl, int suffixlen, int flags) {
    return File{ SYS_INV(mkostemps, tmpl, suffixlen, flags) };
}

} // namespace sys

#endif // CD2KM3B0MVL3AROMJMJL2CGIFXOS2SKCJYMPJRVJ
