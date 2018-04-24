#include "btNode.h"
#include <iostream> ///////////// remove when finished testing

void dumpToArrayInOrder(btNode* bst_root, int* dumpArray)
{
   if (bst_root == 0) return;
   int dumpIndex = 0;
   dumpToArrayInOrderAux(bst_root, dumpArray, dumpIndex);
}

void dumpToArrayInOrderAux(btNode* bst_root, int* dumpArray, int& dumpIndex)
{
   if (bst_root == 0) return;
   dumpToArrayInOrderAux(bst_root->left, dumpArray, dumpIndex);
   dumpArray[dumpIndex++] = bst_root->data;
   dumpToArrayInOrderAux(bst_root->right, dumpArray, dumpIndex);
}

void tree_clear(btNode*& root)
{
   if (root == 0) return;
   tree_clear(root->left);
   tree_clear(root->right);
   delete root;
   root = 0;
}

int bst_size(btNode* bst_root)
{
   if (bst_root == 0) return 0;
   return 1 + bst_size(bst_root->left) + bst_size(bst_root->right);
}

// write definition for bst_insert here
bool bst_insert(btNode*& bst_root, int anInt)
{
    // if the bst is empty, create a new node for the value to be
    // added and make it the root, then return true
    if (bst_root == 0)
    {
        bst_root = new btNode;
        bst_root->data = anInt;
        bst_root->left = 0;
        bst_root->right = 0;
        return true;
    }

    // create a cursor to traverse the bst
    btNode* cursor = bst_root;
    // traverse the bst
    while (cursor != 0)
    {
        // don't add anInt to the bst if it already exists in the bst
        if (cursor->data == anInt)
            return false;
        // if anInt is less than the current value, look to the
        // left subtree
        if (anInt < cursor->data)
        {
            // if there is no left child, make a new node with anInt
            // as the left child
            if (cursor->left == 0)
            {
                cursor->left = new btNode;
                cursor->left->data = anInt;
                cursor->left->left = 0;
                cursor->left->right = 0;
                return true;
            }
            // otherwise, mover the cursor to the left subtree
            else
                cursor = cursor->left;
        }
        // if anInt is greater than the current value, look to the
        // right subtree
        if (anInt > cursor->data)
        {
            // if there is no right child, make a new node with anInt
            // as the right child
            if (cursor->right == 0)
            {
                cursor->right = new btNode;
                cursor->right->data = anInt;
                cursor->right->left = 0;
                cursor->right->right = 0;
                return true;
            }
            // otherwise move the cursor to the right subtree
            else
                cursor = cursor->right;
        }
    }
    // this should never be reached
    return false;
}

// write definition for bst_remove here
bool bst_remove(btNode*& bst_root, int anInt)
{
    // if the bst is empty, nothing can be removed
    if (bst_root == 0)
        return false;
    // if anInt is less than the current value, search the left subtree
    if (anInt < bst_root->data)
        return bst_remove(bst_root->left, anInt);
    // if anInt is greater than the current value, search the right subtree
    if (anInt > bst_root->data)
        return bst_remove(bst_root->right, anInt);
    // if anInt is found, remove it from the bst
    if (anInt == bst_root->data)
    {
        // if there are 0 or 1 children, use the following to remove the node
        if (bst_root->left == 0 || bst_root->right == 0)
        {
            // store the old root for deleting later
            btNode* old_bst_root = bst_root;

            // if there are no left and right children, simply remove the node
            if (bst_root->left == 0 && bst_root->right == 0)
            {
                bst_root = 0;
                delete old_bst_root;
                return true;
            }
            // if there is only a left child, set the root to the left child
            else if (bst_root->left != 0 && bst_root->right == 0)
            {
                bst_root = bst_root->left;
            }
            // if there is only a right child, set the root to the right child
            else if (bst_root->left == 0 && bst_root->right != 0)
            {
                bst_root = bst_root->right;
            }
            // delete the old root once it has been removed in one of the
            // above ways
            delete old_bst_root;
        }
        // if there is a left and right child, bring the max value of the
        // left subtree to replace the value of the root
        else
        {
            bst_remove_max(bst_root->left, bst_root->data);
        }
        // reaching this point means the node containing anInt has been
        // successfully removed
        return true;
    }
    // this should never be reached
    return false;
}

// write definition for bst_remove_max here
void bst_remove_max(btNode*& bst_root, int& data)
{
    // if there is no right child, then there is nothing bigger
    // than the current value and we have found the max to be
    // swapped and its node deleted
    if (bst_root->right == 0)
    {
        data = bst_root->data;
        btNode* old_bst_root = bst_root;
        bst_root = bst_root->left;
        delete old_bst_root;
    }
    // if the max has not yet been found, look to the right subtree
    // until it is found
    else
    {
        bst_remove_max(bst_root->right, data);
    }
}
