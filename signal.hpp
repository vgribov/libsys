#include <signal.h>

#include <initializer_list>

#include "error.hpp"

namespace sys {

struct Sig_set : public sigset_t {
    Sig_set() { empty(); }
    Sig_set(std::initializer_list<int> sigs)
        : Sig_set{}
    {
        for (int sig : sigs) add(sig);
    }

    void empty() { ::sigemptyset(this); }
    void fill()  { ::sigfillset(this); }

    void add(int signum) { ::sigaddset(this, signum); }
    void del(int signum) { ::sigdelset(this, signum); }

    bool is_member(int signum) { return ::sigismember(this, signum); }
};

using Sig_handler = void (*)(int);

inline Sig_handler signal(int sig, Sig_handler handler) {
    auto res = ::signal(sig, handler);
    if (res == SIG_ERR) throw error(__func__);
    return res;
}

} // namespace sys
