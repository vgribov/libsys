#pragma once

#include <memory>

namespace sys::utils {

template <typename T, typename F = void(*)(T*)>
class Ptr : public std::unique_ptr<T, void(*)(T*)> {
    using Base = std::unique_ptr<T, void(*)(T*)>;
public:
    using Base::Base;
    using Base::operator=;

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
