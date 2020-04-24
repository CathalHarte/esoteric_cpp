/**
* \file shared_ptr_tree_test.cpp
*
* \brief ... unit test
*
* \author Cathal Harte  <cathal.harte@protonmail.com>
*/

/*******************************************************************************
* Includes
*******************************************************************************/

#include <memory>

#include <gtest/gtest.h>

#include <shared_ptr_tree.h>

namespace
{

/*******************************************************************************
* Definitions and types
*******************************************************************************/

#define GTEST_COUT std::cerr << "[          ] [ INFO ] "

/*******************************************************************************
* Local Function prototypes
*******************************************************************************/

/*******************************************************************************
* Data
*******************************************************************************/

/*******************************************************************************
* Functions
*******************************************************************************/

TEST(stdlib_test, shared_out_of_scope)
{
    // testing of the basics of memory management can happen in two ways
    // 1, we use an overload of new and delete in our test suite
    // 2, we use a weak_ptr - as we can ask this ptr if the underlying data is still valid
    //                      - and it doesn't keep the memory of a shared ptr alive
    // 3, assert_death when trying to access the newly created shared handle - although I think this only dereferences a
    //    NULL ptr

    int *raw_ptr;
    std::weak_ptr<int> top_level_weak;
    {
        std::shared_ptr<int> shared = std::make_shared<int>();
        *shared = 14;
        raw_ptr = shared.get();
        top_level_weak = shared;
    }

    ASSERT_EQ(*raw_ptr, 14);
    ASSERT_TRUE(top_level_weak.expired());
    auto shared_handle = top_level_weak.lock();
    ASSERT_FALSE(shared_handle);
    ASSERT_DEATH(GTEST_COUT << *shared_handle << std::endl;, "");
}

TEST(stdlib_test, unique_ptr_1)
{
    std::unique_ptr<int> unique = std::make_unique<int>();
    *unique = 165;

    // std::weak_ptr<int> weak = unique;
    int *raw_ptr = unique.get();
    ASSERT_EQ(*raw_ptr, 165);
}

} // namespace
