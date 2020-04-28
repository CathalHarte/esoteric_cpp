/******************************************************************************/
/*!
 * @file  shared_ptr_tree.cpp
 * @brief
 * 
 * @author Cathal Harte <cathal.harte@protonmail.com>
 */

/*******************************************************************************
* Includes
******************************************************************************/

#include "shared_ptr_tree.h"
#include <cassert>

namespace tree
{

/*******************************************************************************
* Definitions
*******************************************************************************/


/*******************************************************************************
* Types
*******************************************************************************/


/*******************************************************************************
* Internal function prototypes
*******************************************************************************/


/*******************************************************************************
* Classes
*******************************************************************************/


/*******************************************************************************
* Functions
*******************************************************************************/

bool WordBranch::isRoot()
{
    return parent.expired();
}

std::shared_ptr<WordBranch> WordBranch::getParent()
{
    return parent.lock();
}

std::size_t WordBranch::getNumChildren()
{
    return children.size();
}

void addChild(std::shared_ptr<WordBranch> parent, std::shared_ptr<WordBranch> child)
{
    assert(("Prospective child is parentless", child->parent.expired() == true));
    parent->children.emplace_back(child);
    child->parent = parent;
}

void removeChild(std::shared_ptr<WordBranch> parent, std::shared_ptr<WordBranch> child)
{
    auto iter = parent->children.begin();
    auto end = parent->children.end();
    for(; iter != end; advance(iter,1))
    {
        if(child == *iter)
        {
            parent->children.erase(iter);
            child->parent.reset();
            return;
        }
    }
    throw "is not a child of parent";
}

}
