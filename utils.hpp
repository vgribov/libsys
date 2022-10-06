#pragma once

#include <memory>

namespace sys::utils {

struct No_copy {
    No_copy() {}
    No_copy(No_copy&)            = delete;
    No_copy& operator=(No_copy&) = delete;
};

template <typename T>
using Dtor = void(*)(T*) noexcept;

template <typename T>
class Ptr : public std::unique_ptr<T, Dtor<T>> {
    using Base = std::unique_ptr<T, Dtor<T>>;

public:
    Ptr(T* obj = nullptr, Dtor<T> dtor = def_dtor)
        : Base{obj, dtor}
        { }
    operator T*() { return this->get(); }

private:
    static void def_dtor(T* t) noexcept {
        try { delete t; }
        catch (...) {}
    }
};

template <typename T, typename... Args>
auto make_ptr(Args&&... args) {
    return Ptr<T>{ new T{std::forward<Args>(args)...} };
}

} // namespace sys
