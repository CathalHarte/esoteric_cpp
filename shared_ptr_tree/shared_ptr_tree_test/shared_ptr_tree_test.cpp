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

using namespace tree;
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
        // raw_ptr = *shared;
        raw_ptr = shared.get();
        top_level_weak = shared;
    }

    ASSERT_EQ(*raw_ptr, 14);
    ASSERT_TRUE(top_level_weak.expired());
    auto shared_handle = top_level_weak.lock();
    ASSERT_FALSE(shared_handle);
    ASSERT_DEATH(GTEST_COUT << *shared_handle << std::endl;, "");
}

TEST(stdlib_test, shared_string_ptr)
{
    std::string foo = "foo";
    foo = "bar bar";

    std::shared_ptr<char[]> shared;
    std::shared_ptr<std::string> shared_2;

    // shared_2 = "dge";
    ASSERT_EQ(foo, "bar bar");
    ASSERT_DEATH(shared[0] = 'a', "");
    ASSERT_DEATH(*shared_2 = "baz", "");

    std::shared_ptr<std::string> shared_3 = std::make_shared<std::string>("baz");

    ASSERT_EQ(*shared_3, "baz");

    *shared_3 = "qux qux";
    ASSERT_EQ(*shared_3, "qux qux");

    std::string string_2;
    string_2 = "qux"; // this works, but not the equivalent on the shared pointer
    // maybe that's a false equivalent

    std::shared_ptr<std::string> shared_4 = std::make_shared<std::string>();
    *shared_4 = "what's after qux?";
    // ah, I see

    // so to revise that
    shared_2 = std::make_shared<std::string>("foo");
    ASSERT_EQ(*shared_2, "foo");
}

TEST(stdlib_test, unique_ptr_1)
{
    std::unique_ptr<int> unique = std::make_unique<int>();
    *unique = 165;

    // std::weak_ptr<int> weak = unique;
    int *raw_ptr = unique.get();
    ASSERT_EQ(*raw_ptr, 165);
}

TEST(word_branch, declaration_bare)
{
    WordBranch foo;
    ASSERT_EQ(foo.name, "");
}

TEST(word_branch, declaration_string)
{
    WordBranch foo("foo");
    ASSERT_EQ(foo.name, "foo");
}

TEST(word_branch, ptr_declarations)
{
    std::shared_ptr<WordBranch> foo = std::make_shared<WordBranch>();
    std::shared_ptr<WordBranch> bar = std::make_shared<WordBranch>("bar");
    std::shared_ptr<WordBranch> baz;

    ASSERT_EQ(baz.use_count(),0);

    baz = bar;

    ASSERT_EQ(baz.use_count(), 2);
    ASSERT_EQ(baz->name, "bar");

    bar.reset();
    ASSERT_EQ(baz.use_count(), 1);
    ASSERT_EQ(baz->name, "bar");
}

TEST(word_branch, out_of_scope)
{
    std::weak_ptr<WordBranch> on_a_branch_1, on_a_branch_2;
    {
        std::shared_ptr<WordBranch> trunk = std::make_shared<WordBranch>();
        trunk->name = "I am the trunk";

        {

            std::shared_ptr<WordBranch> branch_1 = std::make_shared<WordBranch>();
            branch_1->name = "I am the first branch";
            branch_1->parent = trunk;
            trunk->children.push_back(branch_1);

            on_a_branch_1 = branch_1;

            std::shared_ptr<WordBranch> branch_2 = std::make_shared<WordBranch>();

            on_a_branch_2 = branch_2;
        }

        {
            std::shared_ptr<WordBranch> temp = on_a_branch_1.lock();
            ASSERT_EQ(temp->name, "I am the first branch");
            ASSERT_FALSE(on_a_branch_2.lock());
        }
    }
    ASSERT_FALSE(on_a_branch_1.lock());
}



// A WordBranch is a root if it has no parent, i.e. it has just been created, or if its own parent has been reset
TEST(word_branch, is_root)
{
    std::shared_ptr<WordBranch> root = std::make_shared<WordBranch>("I'm the root");
    ASSERT_TRUE(root->isRoot());
}
} // namespace
