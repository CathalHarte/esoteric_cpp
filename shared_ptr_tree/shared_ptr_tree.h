/******************************************************************************/
/*!
 * @file  shared_ptr_tree.h
 * @brief 
 * 
 * @author Cathal Harte <cathal.harte@protonmail.com>
 */
#ifndef _SHARED_PTR_TREE_H
#define _SHARED_PTR_TREE_H

/*******************************************************************************
* Includes
******************************************************************************/

#include <string>
#include <memory>
#include <vector>

namespace tree
{

/*! @defgroup shared_ptr_tree Shared_ptr_tree.
 *
 * @addtogroup shared_ptr_tree
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

class WordBranch
{
public:

    WordBranch() : name("") {}
    WordBranch(std::string name) : name(name) {}
    std::string name;

    // we cannot expose either the parent of vector of children
    // else we could add children in an incorrect manner
    std::weak_ptr<WordBranch> parent;
    std::vector<std::shared_ptr<WordBranch>> children;

    bool isRoot();
};

/*******************************************************************************
* Function prototypes
*******************************************************************************/

/*! @}
 */

}

#endif // _SHARED_PTR_TREE_H
