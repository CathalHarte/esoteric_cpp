/******************************************************************************/
/*!
 * @file  smart_tree.h
 * @brief Named a smart tree because it is an unsorted tree implemented using 
 *        smart pointers
 * 
 * @author Cathal Harte <cathal.harte@protonmail.com>
 */
#ifndef _SMART_TREE_H
#define _SMART_TREE_H

/*******************************************************************************
* Includes
******************************************************************************/

#include <string>
#include <memory>
#include <vector>

namespace smart_tree
{

/*! @defgroup smart_tree Smart_tree.
 *
 * @addtogroup smart_tree
 * @{
 * @brief 
 */
/*******************************************************************************
* Definitions and types
*******************************************************************************/

/*******************************************************************************
* Data
*******************************************************************************/

/*******************************************************************************
* Class prototypes
*******************************************************************************/

//  This class implements a node for an unsorted tree of strings
//  Design parameters :
//      A branch without a parent is a root.
//      A branch may have 1 parent and an unlimited number of children.
//      We use smart ptrs throughout
//          As long as a reference to the parent remains, the children will not expire
//          The corollary of this is that if a reference to a child is kept and no references to the parent are
//          then the parent and siblings will expire and the child becomes the new root
class WordBranch
{
public:
    WordBranch(std::string name = "") : name(name) {}
    std::string name;

    bool isRoot();
    std::shared_ptr<WordBranch> getParent();
    std::size_t getNumChildren();
    std::vector<std::shared_ptr<WordBranch>>::iterator childrenBegin() { children.begin(); }
    std::vector<std::shared_ptr<WordBranch>>::iterator childrenEnd() { children.end(); }

private:
    std::weak_ptr<WordBranch> parent;
    std::vector<std::shared_ptr<WordBranch>> children;
    friend void addChild(std::shared_ptr<WordBranch> parent, std::shared_ptr<WordBranch> child);
    friend void removeChild(std::shared_ptr<WordBranch> parent, std::shared_ptr<WordBranch> child);
};

/*******************************************************************************
* Function prototypes
*******************************************************************************/

// should I do the friend class pattern here?
void addChild(std::shared_ptr<WordBranch> parent, std::shared_ptr<WordBranch> child);
void removeChild(std::shared_ptr<WordBranch> parent, std::shared_ptr<WordBranch> child);

/*! @}
 */

} // namespace smart_tree

#endif // _SMART_TREE_H
