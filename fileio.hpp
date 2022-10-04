#ifndef Z0RQZMERFZSUQMN8YQ96X4F3BZYQS21E3W7POI1I
#define Z0RQZMERFZSUQMN8YQ96X4F3BZYQS21E3W7POI1I

#include <poll.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <unistd.h>

#include <initializer_list>
#include <string>

#include "config.h"
#include "file.hpp"

namespace sys {

template <typename T>
constexpr size_t __elements_left(size_t bytes)
{
    auto e = bytes / sizeof(T);
    return bytes % sizeof(T) ? e + 1 : e;
}

template <typename T>
size_t read(const File_des& f, T* buf, size_t len = 1)
{
    auto    cur   = reinterpret_cast<char*>(buf);
    size_t  bytes = len * sizeof(T);
    ssize_t ret;

    while ((ret = ::read(f.get(), cur, bytes)) != 0)
    {
        if (ret == -1) {
            if (errno == EINTR) continue;
            throw error(__func__);
        }

        bytes -= ret;
        if (bytes == 0) return len;

        cur += ret;
    }

    return len - __elements_left<T>(bytes);
}

template <typename T>
size_t write(const File_des& f, const T* buf, size_t len = 1)
{
    auto    cur   = reinterpret_cast<const char *>(buf);
    size_t  bytes = len * sizeof(T);
    ssize_t ret;

    while ((ret = ::write(f.get(), cur, bytes)) != 0)
    {
        if (ret == -1) {
            if (errno == EINTR) continue;
            throw error(__func__);
        }

        bytes -= ret;
        if (bytes == 0) return len;

        cur += ret;
    }

    return len - __elements_left<T>(bytes);
}

inline void fsync(const File_des& f) {
    SYS_INV(fsync, f.get());
}

#ifdef HAVE_FDATASYNC

inline void fdatasync(const File_des& f) {
    SYS_INV(fdatasync, f.get());
}

#endif // HAVE_FDATASYNC

inline off_t lseek(const File_des& f, off_t pos = 0, int origin = SEEK_SET) {
    return SYS_INV(lseek, f.get(), pos, origin);
}

#ifdef HAVE_PREADWRITE

template <typename T>
size_t pread(const File_des& f, T* buf, size_t count, off_t pos)
{
    auto    cur   = reinterpret_cast<char *>(buf);
    size_t  bytes = count * sizeof(T);
    ssize_t ret;

    while ((ret = ::pread(f.get(), cur, bytes, pos)) != 0)
    {
        if (ret == -1) {
            if (errno == EINTR) continue;
            throw error(__func__);
        }

        bytes -= ret;
        if (bytes == 0) return count;

        cur += ret;
        pos += ret * sizeof(T);
    }

    return count - __elements_left<T>(bytes);
}

template <typename T>
size_t pwrite(const File_des& f, const T* buf, size_t count, off_t pos)
{
    auto    cur   = reinterpret_cast<const char*>(buf);
    size_t  bytes = count * sizeof(T);
    ssize_t ret;

    while ((ret = ::pwrite(f.get(), cur, bytes, pos)) != 0)
    {
        if (ret == -1) {
            if (errno == EINTR) continue;
            throw error(__func__);
        }

        bytes -= ret;
        if (bytes == 0) return count;

        cur += ret;
        pos += ret * sizeof(T);
    }

    return count - __elements_left<T>(bytes);
}

#endif // HAVE_PREADWRITE

inline void ftruncate(const File_des& f, off_t len) {
    SYS_INV(ftruncate, f.get(), len);
}

inline void truncate(const char* path, off_t len) {
    SYS_INV(truncate, path, len);
}

struct File_set : public fd_set {
    File_set() { zero(); }
    File_set(std::initializer_list<File_des> l)
        : File_set()
        { for (const auto& f : l) set(f); }

    void set(const File_des& f)    { FD_SET(f.get(), this); }
    void clr(const File_des& f)    { FD_CLR(f.get(), this); }
    bool is_set(const File_des& f) { return FD_ISSET(f.get(), this) != 0; }
    void zero()                    { FD_ZERO(this); }
};

inline size_t select(size_t n,
                     File_set* readfds,
                     File_set* writefds,
                     File_set* exceptfds,
                     timeval*  timeout) {
    return SYS_INV(select, n, readfds, writefds, exceptfds, timeout);
}

#ifdef HAVE_PSELECT

inline size_t pselect(size_t    n,
                      File_set* readfds,
                      File_set* writefds,
                      File_set* exceptfds,
                      timespec* timeout,
                      sigset_t* sigmask = nullptr) {
    return SYS_INV(pselect, n, readfds, writefds, exceptfds,
                   timeout, sigmask);
}

#endif // HAVE_PSELECT

struct Poll_fd : public pollfd {
    Poll_fd(const File_des &f, short ev)
        : pollfd{f.get(), ev, 0}
        { };
};

inline size_t poll(Poll_fd* fds, size_t nfds, int timeout) {
    return SYS_INV(poll, fds, nfds, timeout);
}

#ifdef HAVE_PPOLL

inline size_t ppoll(Poll_fd* fds, size_t nfds, const timespec* timeout,
                    const sigset_t* sigmask = nullptr) {
    return SYS_INV(ppoll, fds, nfds, timeout, sigmask);
}

#endif // HAVE_PPOLL

} // namespace sys

#endif // Z0RQZMERFZSUQMN8YQ96X4F3BZYQS21E3W7POI1I
