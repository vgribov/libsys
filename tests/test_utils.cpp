#include "utils.hpp"
#include "gtest/gtest.h"

using namespace sys::utils;

static int obj_count;

struct A {
    int val;
    A(int v = 0) : val{v} { ++obj_count; }
    ~A() { --obj_count; }
};

TEST(Ptr, constuction)
{
    Ptr<A> empty_ptr;
    ASSERT_EQ(empty_ptr, nullptr);

    Ptr<A> a_ptr{ new A{10} };
    ASSERT_NE(a_ptr, nullptr);
    ASSERT_EQ(a_ptr->val, 10);

    a_ptr = make_ptr<A>(5);
    ASSERT_NE(a_ptr, nullptr);
    ASSERT_EQ(a_ptr->val, 5);

    a_ptr = make_ptr<A>();
    ASSERT_NE(a_ptr, nullptr);
    ASSERT_EQ(a_ptr->val, 0);
}

TEST(Ptr, destruction)
{
    {
        obj_count = 0;
        Ptr<A> a_ptr{ new A{} };
        ASSERT_EQ(obj_count, 1);
    }

    ASSERT_EQ(obj_count, 0);

    A a{};
    {
        auto deleter = [](A*) noexcept { --obj_count; };
        Ptr<A> a_ptr{ &a, deleter };
        ASSERT_EQ(obj_count, 1);
    }

    ASSERT_EQ(obj_count, 0);
}
