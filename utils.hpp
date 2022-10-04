#pragma once

#include <memory>

namespace sys::utils {

struct No_copy {
    No_copy() {}
    No_copy(No_copy&)            = delete;
    No_copy& operator=(No_copy&) = delete;
};

    Ptr()       : Base{nullptr, default_deleter} {}
    Ptr(T* obj) : Base{obj,     default_deleter} {}

private:
    static void default_deleter(T*t) { delete t; }
};

template <typename T, typename... Args>
auto make_ptr(Args&&... args) {
    return Ptr<T>{ new T{std::forward<Args>(args)...} };
}

} // namespace sys
