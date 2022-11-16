#pragma once

#include <sys/un.h>
#include <sys/socket.h>

#include "file.hpp"

namespace sys {

inline auto socket(int domain, int type, int protocol) {
    return File{ SYS_INV(socket, domain, type, protocol) };
}

template <typename A>
inline void bind(File_des sfd, const A* addr) {
    auto sa = reinterpret_cast<const ::sockaddr*>(addr);
    SYS_INV(bind, sfd.get(), sa, sizeof(A));
}

template <typename A>
inline void connect(File_des sfd, const A* addr) {
    auto sa = reinterpret_cast<const ::sockaddr*>(addr);
    SYS_INV(connect, sfd.get(), sa, sizeof(A));
}

inline void listen(File_des sfd, int backlog) {
    SYS_INV(listen, sfd.get(), backlog);
}

inline auto accept(File_des s) {
    return File{ SYS_INV(accept, s.get(), nullptr, nullptr) };
}

} // namespace sys
