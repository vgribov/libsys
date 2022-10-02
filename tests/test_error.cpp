#include <sstream>

#include "error.hpp"
#include "gtest/gtest.h"

using namespace sys;

TEST(Error, error)
{
    using namespace std::literals;
    try {
        throw error("Hello", " world");
    } catch (std::runtime_error& err) {
        ASSERT_EQ("Hello world: Success"s, err.what());
    }
}

