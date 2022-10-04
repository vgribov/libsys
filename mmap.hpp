#pragma once

#include <iostream>
#include <utility>
#include <sys/mman.h>

#include "file.hpp"
#include "utils.hpp"

namespace sys {

struct Mem_map {
    void*  data;
    size_t length;
};

inline utils::Ptr<Mem_map>
mmap(void* addr, size_t length, int prot, int flags, File_des fd, off_t offset)
{
    void* p = ::mmap(addr, length, prot, flags, fd.get(), offset);
    if (p == MAP_FAILED) throw error(__func__);
    auto deleter = [](Mem_map* mm) noexcept {
        ::munmap(mm->data, mm->length);
        delete mm;
    };
    return { new Mem_map{p, length}, deleter };
}

} // namespace sys
