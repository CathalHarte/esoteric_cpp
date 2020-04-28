/**
* \file smart_tree_test.cpp
*
* \brief ... unit test
*
* \author Cathal Harte  <cathal.harte@protonmail.com>
*/

/*******************************************************************************
* Includes
*******************************************************************************/

#include <smart_tree.h>

#include <memory>

#include <gtest/gtest.h>

using namespace smart_tree;
using WordBranch = Branch<std::string>;

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

TEST(stdlib_test, delete_pointer_to_stack)
{
    int a = 11;
    ASSERT_DEATH(delete &a, "");

    WordBranch foo;
    ASSERT_DEATH(delete &foo, "");
}

TEST(stdlib_test, shared_ptr_comparison)
{
    // how do we quickly determine if two shared_ptr point to the same location?

    std::shared_ptr<int> foo = std::make_shared<int>(11);
    std::shared_ptr<int> bar(foo);
    *foo = 12;
    ASSERT_EQ(*bar, 12);
    ASSERT_EQ(foo.use_count(), 2);

    ASSERT_EQ(foo, bar);
}

TEST(word_branch, declaration_bare)
{
    WordBranch foo;
    ASSERT_EQ(foo.data, "");
    ASSERT_FALSE(foo.getParent());
    ASSERT_EQ(foo.getNumChildren(), 0);
}

TEST(word_branch, declaration_string)
{
    WordBranch foo("foo");
    ASSERT_EQ(foo.data, "foo");
    ASSERT_FALSE(foo.getParent());
    ASSERT_EQ(foo.getNumChildren(), 0);
}

TEST(word_branch, ptr_declarations)
{
    std::shared_ptr<WordBranch> bar = std::make_shared<WordBranch>("bar");
    std::shared_ptr<WordBranch> baz;

    ASSERT_EQ(baz.use_count(), 0);

    baz = bar;

    ASSERT_EQ(baz.use_count(), 2);
    ASSERT_EQ(baz->data, "bar");

    bar.reset();
    ASSERT_EQ(baz.use_count(), 1);
    ASSERT_EQ(baz->data, "bar");
}

// A WordBranch is a root if it has no parent, i.e. it has just been created, or if its own parent has been reset
TEST(word_branch, is_root)
{
    std::shared_ptr<WordBranch> root = std::make_shared<WordBranch>("I'm the root");
    ASSERT_TRUE(root->isRoot());
}

// we can't add by val, WordBranch members have to be shared_ptr to begin with
TEST(word_branch, add_children_ref)
{
    std::shared_ptr<WordBranch> foo = std::make_shared<WordBranch>("The root");
    {
        std::shared_ptr<WordBranch> bar = std::make_shared<WordBranch>("a branch");
        std::shared_ptr<WordBranch> baz = std::make_shared<WordBranch>("another branch");
        addChild(foo, bar);
        addChild(foo, baz);

        ASSERT_TRUE(bar->getParent());
    }
    ASSERT_EQ(foo->getNumChildren(), 2);
}

TEST(word_branch, remove_child)
{
    std::shared_ptr<WordBranch> foo = std::make_shared<WordBranch>("The root");
    {
        std::shared_ptr<WordBranch> bar = std::make_shared<WordBranch>("a branch");
        std::shared_ptr<WordBranch> baz = std::make_shared<WordBranch>("another branch");
        addChild(foo, bar);
        addChild(foo, baz);

        removeChild(foo, bar);

        ASSERT_FALSE(bar->getParent());
    }
    ASSERT_EQ(foo->getNumChildren(), 1);
}

// A child cannot have two parents, then it wouldn't exactly be a tree
TEST(word_branch, child_relocation_fail)
{
    std::shared_ptr<WordBranch> foo = std::make_shared<WordBranch>("a root");
    std::shared_ptr<WordBranch> bar = std::make_shared<WordBranch>("another root");
    std::shared_ptr<WordBranch> baz = std::make_shared<WordBranch>("child 2");

    addChild(foo, baz);

    ASSERT_DEATH(addChild(bar, baz), "");
}

TEST(word_branch, child_relocation_success)
{
    std::shared_ptr<WordBranch> foo = std::make_shared<WordBranch>("a root");
    std::shared_ptr<WordBranch> bar = std::make_shared<WordBranch>("another root");
    std::shared_ptr<WordBranch> baz = std::make_shared<WordBranch>("child 2");

    addChild(foo, baz);

    removeChild(foo, baz);
    addChild(bar, baz);

    ASSERT_EQ(foo->getNumChildren(), 0);
    ASSERT_EQ(bar->getNumChildren(), 1);
}

TEST(word_branch, get_parent)
{
    std::shared_ptr<WordBranch> foo = std::make_shared<WordBranch>("The root");
    {
        std::shared_ptr<WordBranch> bar = std::make_shared<WordBranch>("a branch");
        std::shared_ptr<WordBranch> baz = std::make_shared<WordBranch>("another branch");
        addChild(foo, bar);
        addChild(foo, baz);
        foo->data = "a parent";
        bar->data = "a child";

        ASSERT_FALSE(bar->isRoot());
        ASSERT_EQ(baz->getParent()->data, "a parent");
    }
}

TEST(word_branch, get_children_iter)
{

    std::shared_ptr<WordBranch> foo = std::make_shared<WordBranch>("A root");
    std::shared_ptr<WordBranch> bar = std::make_shared<WordBranch>("child 0");
    std::shared_ptr<WordBranch> baz = std::make_shared<WordBranch>("child 1");
    std::shared_ptr<WordBranch> qux = std::make_shared<WordBranch>("child 2");

    addChild(foo, bar);
    addChild(foo, baz);
    addChild(foo, qux);

    std::vector<std::string> rename = {"rename0", "rename1", "rename2"};
    bar->data = rename[0];
    baz->data = rename[1];
    qux->data = rename[2];

    int i = 0;
    auto iter = foo->childrenBegin();
    auto end = foo->childrenEnd();
    for (; iter != end; std::advance(iter, 1))
    {
        ASSERT_EQ(rename[i++], (**iter).data);
    }
}

// the whole design motivation behind this tree, self destructing children
TEST(word_branch, children_destroyed)
{
    std::weak_ptr<WordBranch> cant_keep_alive;
    {
        std::shared_ptr<WordBranch> layer0_0 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer1_0 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer1_1 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer2_0 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer3_0 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer3_1 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer4_0 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer4_1 = std::make_shared<WordBranch>();

        cant_keep_alive = layer4_1;

        addChild(layer0_0, layer1_0);
        addChild(layer0_0, layer1_1);
        addChild(layer1_0, layer2_0);
        addChild(layer2_0, layer3_0);
        addChild(layer2_0, layer3_1);
        addChild(layer3_1, layer4_0);
        addChild(layer3_1, layer4_1);
    }

    ASSERT_TRUE(cant_keep_alive.expired());
}

TEST(word_branch, children_kept_alive)
{
    std::shared_ptr<WordBranch> can_keep_alive3_1;
    std::weak_ptr<WordBranch>
        cant_keep_alive0_0,
        cant_keep_alive1_0,
        cant_keep_alive1_1,
        cant_keep_alive2_0,
        cant_keep_alive3_0,
        cant_keep_alive3_1;

    {
        std::shared_ptr<WordBranch> layer0_0 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer1_0 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer1_1 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer2_0 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer3_0 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer3_1 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer4_0 = std::make_shared<WordBranch>();
        std::shared_ptr<WordBranch> layer4_1 = std::make_shared<WordBranch>();

        cant_keep_alive0_0 = layer0_0;
        cant_keep_alive1_0 = layer1_0;
        cant_keep_alive1_1 = layer1_1;
        cant_keep_alive2_0 = layer2_0;
        cant_keep_alive3_0 = layer3_0;
        cant_keep_alive3_1 = layer3_1;

        can_keep_alive3_1 = layer3_1;

        addChild(layer0_0, layer1_0);
        addChild(layer0_0, layer1_1);
        addChild(layer1_0, layer2_0);
        addChild(layer2_0, layer3_0);
        addChild(layer2_0, layer3_1);
        addChild(layer3_1, layer4_0);
        addChild(layer3_1, layer4_1);
    }

    ASSERT_TRUE(cant_keep_alive0_0.expired());
    ASSERT_TRUE(cant_keep_alive1_0.expired());
    ASSERT_TRUE(cant_keep_alive1_1.expired());
    ASSERT_TRUE(cant_keep_alive2_0.expired());
    ASSERT_TRUE(cant_keep_alive3_0.expired()); // sibling is destroyed
    ASSERT_FALSE(cant_keep_alive3_1.expired());

    ASSERT_EQ(can_keep_alive3_1->getNumChildren(), 2);
}

} // namespace
