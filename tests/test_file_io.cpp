#include <array>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#include "gtest/gtest.h"

#include "file_io.hpp"

template <typename T, size_t N>
constexpr size_t array_size(T (&)[N]) noexcept { return N; }

namespace {

#ifdef __APPLE__
    constexpr const char* fd_dir = "/dev/fd";
#else
    constexpr const char* fd_dir = "/proc/self/fd";
#endif

auto count_open_fds()
{
    auto* dp = ::opendir(fd_dir);
    if (!dp) return -1;

    auto count = 0;
    while (::readdir(dp)) ++count;

    ::closedir(dp);
    return count - 3 - 3;
}

} // namespace


TEST(File, default_constructor)
{
    sys::File f{};
}

TEST(File, open_close)
{
    sys::File f{};
    f = sys::open("/etc/hosts", O_RDONLY);
    EXPECT_EQ(count_open_fds(), 1);
}

TEST(File, move_assignment)
{
    auto f = sys::open("/etc/passwd", O_RDONLY);
    EXPECT_EQ(count_open_fds(), 1);

    f = sys::open("/etc/group", O_RDONLY);
    EXPECT_EQ(count_open_fds(), 1);
}


TEST(File, explicit_close)
{
    auto f = sys::open("/etc/hosts", O_RDONLY);
    EXPECT_EQ(count_open_fds(), 1);

    sys::close(f);
    EXPECT_EQ(count_open_fds(), 0);
}

TEST(File, file_not_exist)
{
    auto cought = false;

    try {
        unlink("/tmp/pearl");
        auto f = sys::open("/tmp/pearl", O_WRONLY | O_TRUNC);
    } catch (const sys::Error& ex) {
        std::cerr << ex.what() << "\n";
        cought = true;
    }

    EXPECT_TRUE(cought);
    EXPECT_EQ(count_open_fds(), 0);
}

TEST(File, creat)
{
    auto f = sys::creat("/tmp/pearl", 0644);
    EXPECT_EQ(count_open_fds(), 1);
}

TEST(File, read)
{
    auto f = sys::open("/dev/zero", O_RDONLY);

    unsigned long word;
    auto nr = sys::read(f, &word);
    EXPECT_EQ(nr, 1);
    EXPECT_EQ(word, 0);

    unsigned long words[] = { 1, 2, 3, 4 };
    nr = sys::read(f, words, array_size(words));
    EXPECT_EQ(nr, array_size(words));
    for (size_t i = 0; i < array_size(words); ++i)
        EXPECT_EQ(words[i], 0);

    std::array<int, 6> buf = { 1, 2, 3, 4, 5, 6 };
    nr = sys::read(f, buf.data(), buf.size());
    EXPECT_EQ(nr, buf.size());
    for (auto x : buf)EXPECT_EQ(x, 0);

    // File is no readable exception
    auto cought = false;
    f = sys::open("/dev/null", O_WRONLY);
    try {
        static_cast<void>(sys::read(f, &word));
    } catch (const sys::Error& ex) {
        cought = true;
        EXPECT_EQ(ex.errnum(), EBADF);
    }
    EXPECT_TRUE(cought);
}

TEST(File, write)
{
    auto f = sys::open("/dev/null", O_WRONLY);

    unsigned long word;
    auto nw = sys::write(f, &word);
    EXPECT_EQ(nw, 1);

    // File is no writable exception
    auto cought = false;
    f = sys::open("/dev/zero", O_RDONLY);
    try {
        static_cast<void>(sys::write(f, &word));
    } catch (const sys::Error& ex) {
        cought = true;
        EXPECT_EQ(ex.errnum(), EBADF);
    }
    EXPECT_TRUE(cought);
}

TEST(File, read_write)
{
    auto f = sys::open("/tmp/pearl", O_WRONLY | O_TRUNC);

    std::array<int, 6> obuf = { 1, 2, 3, 4, 5, 6 };
    auto nw = sys::write(f, obuf.data(), obuf.size());
    EXPECT_EQ(nw, obuf.size());

    f = sys::open("/tmp/pearl", O_RDONLY);

    std::array<int, obuf.size()> ibuf = {};
    auto nr = sys::read(f, ibuf.data(), ibuf.size());
    EXPECT_EQ(nr, ibuf.size());
    for (size_t i = 0; i < ibuf.size(); ++i)
        EXPECT_EQ(ibuf[i], obuf[i]);

    // EOF
    unsigned long word;
    nr = sys::read(f, &word);
    EXPECT_EQ(nr, 0);
}

TEST(File, fsync)
{
    char tmpl[] = "/tmp/fileXXXXXX";
    auto f = sys::mkstemp(tmpl);

    std::array<int, 6> obuf = { 1, 2, 3, 4, 5, 6 };
    auto nw = sys::write(f, obuf.data(), obuf.size());
    EXPECT_EQ(nw, obuf.size());

    sys::fsync(f);
}

#ifdef HAVE_FDATASYNC
TEST(File, fdatasync)
{
    auto f = sys::open("/tmp", O_WRONLY | O_TMPFILE, 0644);

    std::array<int, 6> obuf = { 1, 2, 3, 4, 5, 6 };
    auto nw = sys::write(f, obuf.data(), obuf.size());
    EXPECT_EQ(nw, obuf.size());

    sys::fdatasync(f);
}
#endif // HAVE_FDATASYNC

TEST(File, lseek)
{
    constexpr off_t size = 1000000;

    auto f = sys::open("/tmp/sparse", O_CREAT | O_WRONLY | O_TRUNC, 0644);

    auto ret = sys::lseek(f, size-1, SEEK_END);
    EXPECT_EQ(ret, size-1);

    char c = 0;
    sys::write(f, &c);

    ret = sys::lseek(f, 0, SEEK_CUR);
    EXPECT_EQ(ret, size);

    int ex_cnt{0};
    try {
        sys::lseek(f, -1, -1);
    } catch (sys::Error &e) {
        EXPECT_EQ(e.errnum(), EINVAL);
        ++ex_cnt;
    }
    EXPECT_EQ(ex_cnt, 1);
}

#ifdef HAVE_PREADWRITE

TEST(File, pread_pwrite)
{
    auto f = sys::open("/tmp/pearl", O_RDWR | O_TRUNC);

    std::array<int, 6> obuf = { 1, 2, 3, 4, 5, 6 };
    auto nw = sys::pwrite(f, obuf.data(), obuf.size(), 0);
    EXPECT_EQ(nw, obuf.size());

    std::array<int, obuf.size()> ibuf = {};
    auto nr = sys::pread(f, ibuf.data(), ibuf.size(), 0);
    EXPECT_EQ(nr, ibuf.size());
    for (size_t i = 0; i < ibuf.size(); ++i)
        EXPECT_EQ(ibuf[i], obuf[i]);

    // EOF
    unsigned long word;
    nr = sys::pread(f, &word, sizeof(word), obuf.size() * sizeof(int));
    EXPECT_EQ(nr, 0);

    // some bytes left
    nr = sys::pread(f, ibuf.data(), ibuf.size(), 1);
    EXPECT_EQ(nr, ibuf.size() - 1);
}

#endif // HAVE_PREADWRITE

TEST(File, ftruncate)
{
    std::string txt = "Edward Teach was a notorious English pirate.\n"
                      "He was nicknamed Blackbeard.";

    auto f = sys::open("/tmp/pirate.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    sys::write(f, txt.data(), txt.size());
    sys::lseek(f);

    std::array<char, 45> ibuf;
    sys::ftruncate(f, ibuf.size());
    auto n = sys::read(f, ibuf.data(), ibuf.size());
    EXPECT_EQ(n, ibuf.size());

    for (size_t i = 0; i < ibuf.size(); ++i)
        EXPECT_EQ(txt[i], ibuf[i]);

    unsigned long word;
    n = sys::read(f, &word);
    EXPECT_EQ(n, 0);
}

TEST(File, truncate)
{
    std::string txt = "Edward Teach was a notorious English pirate.\n"
                      "He was nicknamed Blackbeard.";

    const char *fname = "/tmp/pirate.txt";
    auto f = sys::open(fname, O_RDWR | O_CREAT | O_TRUNC, 0644);
    sys::write(f, txt.data(), txt.size());

    std::array<char, 45> ibuf;
    sys::truncate(fname, ibuf.size());
    auto pos = sys::lseek(f, 0, SEEK_END);
    EXPECT_EQ(pos, ibuf.size());

    sys::lseek(f);
    auto n = sys::read(f, ibuf.data(), ibuf.size());
    EXPECT_EQ(n, ibuf.size());

    for (size_t i = 0; i < ibuf.size(); ++i)
        EXPECT_EQ(txt[i], ibuf[i]);
}

TEST(File, select)
{
    sys::File_set readfds;
    readfds.set(sys::STDIN);

    sys::File_set writefds;
    writefds.set(sys::STDOUT);

    timeval tv;
    tv.tv_sec  = 5;
    tv.tv_usec = 0;

    auto ret = sys::select(sys::STDOUT.get() + 1,
                           &readfds, &writefds, nullptr, &tv);
    EXPECT_NE(ret, 0);
    EXPECT_EQ(readfds.is_set(sys::STDIN), false);
    EXPECT_EQ(writefds.is_set(sys::STDOUT), true);
}

#ifdef HAVE_PSELECT

TEST(File, pselect)
{
    sys::File_set readfds  = {sys::STDIN};
    sys::File_set writefds = {sys::STDOUT};
    timespec      ts       = {5, 0};

    auto ret = sys::pselect(sys::STDOUT.get() + 1,
                            &readfds, &writefds, nullptr, &ts);
    EXPECT_NE(ret, 0);
    EXPECT_EQ(readfds.is_set(sys::STDIN), false);
    EXPECT_EQ(writefds.is_set(sys::STDOUT), true);
}

#endif // HAVE_PSELECT

TEST(File, poll)
{
    std::array<sys::Poll_fd, 2> fds = {
        sys::Poll_fd{ sys::STDIN,  POLLIN  },
        sys::Poll_fd{ sys::STDOUT, POLLOUT }
    };

    auto ret = sys::poll(fds.data(), fds.size(), 5000);
    EXPECT_NE(ret, 0);
    EXPECT_FALSE(fds[0].revents & POLLIN);
    EXPECT_TRUE(fds[1].revents & POLLOUT);
}

#ifdef HAVE_PPOLL

TEST(File, ppoll)
{
    std::array<sys::Poll_fd, 2> fds = {
        sys::Poll_fd{ sys::STDIN,  POLLIN  },
        sys::Poll_fd{ sys::STDOUT, POLLOUT }
    };

    const timespec ts = {5, 0};

    auto ret = sys::ppoll(fds.data(), fds.size(), &ts);
    EXPECT_NE(ret, 0);
    EXPECT_FALSE(fds[0].revents & POLLIN);
    EXPECT_TRUE(fds[1].revents & POLLOUT);
}

#endif // HAVE_PPOLL
