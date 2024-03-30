#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    virtual void rotateRight(AVLNode<Key, Value>* node);
    virtual void rotateLeft(AVLNode<Key, Value>* node);
    virtual void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    virtual void removeFix(AVLNode<Key, Value>* node, int8_t diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
  AVLNode<Key, Value>* temp = static_cast<AVLNode<Key,Value>*>(this->root_);
  AVLNode<Key, Value>* parent = nullptr;
    while(temp)
    {
        parent = temp;
        if(new_item.first == temp->getKey())
        {
            temp->setValue(new_item.second);
            return;
        }
        else if(new_item.first > temp->getKey())
        {
            temp = temp->getRight();
        }
        else
        {
            temp = temp->getLeft();
        }
    }
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if(parent == nullptr) this->root_ = newNode;
    else if(new_item.first > parent->getKey())
    {
        parent->setRight(newNode);
    }
    else
    {
        parent->setLeft(newNode);
    }

    if(parent && (parent->getBalance() == -1 || parent->getBalance() == 1))
    {
      parent->setBalance(0);
      return;
    }
    else if(parent)
    {
      if(newNode == parent->getLeft())
        parent->setBalance(-1);
      else 
        parent->setBalance(1);
      insertFix(parent, newNode);
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
  AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
  if(!curr) return;

  AVLNode<Key, Value>* parent = curr -> getParent();
  int8_t diff = 0;

  if(curr -> getLeft() != nullptr && curr -> getRight() != nullptr) //curr has two children
  {
    AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this -> predecessor(curr));
    this->nodeSwap(curr, pred);
    parent = curr -> getParent();
  }
  if(parent)
  {
    if(parent -> getLeft() == curr)
    {
      diff = 1;
    }
    else
    {
      diff = -1;
    } 
  }
  AVLNode<Key, Value>* child = nullptr;
  if(curr -> getLeft())
    child = curr -> getLeft();
  else
    child = curr -> getRight();

  if(!parent)
    this -> root_ = child;
  else if (parent -> getLeft() == curr)
    parent -> setLeft(child);
  else 
    parent -> setRight(child);
  
  if(child)
    child -> setParent(parent);

  delete curr;
  removeFix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
  AVLNode<Key, Value>* z = node;
  if(!node) return; //check if rotate is possible
  AVLNode<Key, Value>* x = node -> getLeft();
  if(!x) return;
  
  x -> setParent(z -> getParent());
  if(z -> getParent() != nullptr)
  {
    if(z == z -> getParent() -> getLeft())
    {
      z -> getParent() -> setLeft(x);
    }
    else
    {
      z -> getParent() -> setRight(x);
    }
  }
  else
  {
    this->root_ = x;
  }
  z -> setParent(x);
  if(x -> getRight() != nullptr)
  {
    AVLNode<Key, Value>* b = x -> getRight();
    b -> setParent(z);
    z -> setLeft(b);
  }
  else
  {
    z -> setLeft(nullptr);
  }
  x -> setRight(z);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
  if(!node || !node -> getRight()) return;
  AVLNode<Key, Value>* x = node -> getRight();
  AVLNode<Key, Value>* b = x -> getLeft();
  x -> setParent(node -> getParent());
  if(node -> getParent())
  {
    if(node == node -> getParent() -> getLeft())
    {
      node -> getParent() -> setLeft(x);
    }
    else
    {
      node -> getParent() -> setRight(x);
    }
  }
  else
  {
    this->root_ = x;
  }
  if(b) b -> setParent(node);
  x -> setLeft(node);
  node -> setParent(x);
  node -> setRight(b);
  
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node)
{
    if (!parent) return; // Check if parent or grandparent is nullptr
    
    AVLNode<Key, Value>* grandparent = parent->getParent();

    if(!grandparent) return;
    
    if (parent == grandparent->getLeft()) // Parent is left child of grandparent
    {
        grandparent->updateBalance(-1);
        if (grandparent->getBalance() == 0) return;
        else if (grandparent->getBalance() == -1) insertFix(grandparent, parent);
        else if (grandparent->getBalance() == -2)
        {
            if (parent->getLeft() == node) // Zig Zig
            {
                rotateRight(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            }
            else // Zig Zag
            {
                rotateLeft(parent);
                rotateRight(grandparent);
                if (node->getBalance() == -1)
                {
                    parent->setBalance(0);
                    grandparent->setBalance(1);
                    node->setBalance(0);
                }
                else if (node->getBalance() == 0)
                {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                    node->setBalance(0);
                }
                else if (node -> getBalance() == 1)
                {
                    parent->setBalance(-1);
                    grandparent->setBalance(0);
                    node->setBalance(0);
                }
            }
        }
    }
    else // Parent is right child of grandparent
    {
        grandparent->updateBalance(1);
        if (grandparent->getBalance() == 0) return;
        else if (grandparent->getBalance() == 1) insertFix(grandparent, parent);
        else if (grandparent->getBalance() == 2)
        {
            if (parent->getRight() == node) // Zig Zig
            {
                rotateLeft(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            }
            else // Zig Zag
            {
                rotateRight(parent);
                rotateLeft(grandparent);
                if (node->getBalance() == 1)
                {
                    parent->setBalance(0);
                    grandparent->setBalance(-1);
                    node->setBalance(0);
                }
                else if (node->getBalance() == 0)
                {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                    node->setBalance(0);
                }
                else
                {
                    parent->setBalance(1);
                    grandparent->setBalance(0);
                    node->setBalance(0);
                }
            }
        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff)
{
  if(!node) return;
  AVLNode<Key, Value>* parent = node -> getParent();
  int ndiff = 0;
  if(parent != nullptr)
  {
    if(parent -> getLeft() == node)
      ndiff = 1;
    else
      ndiff = -1;
  }

  if(diff == -1) //Left Subtree Taller
  {
    if(node -> getBalance() + diff == -2) //Case 1
    {
      AVLNode<Key, Value>* child = node -> getLeft();
      if(child -> getBalance() == -1) //Case 1a
      {
        rotateRight(node);
        node -> setBalance(0);
        child -> setBalance(0);
        removeFix(parent, ndiff);
      }
      else if (child -> getBalance() == 0) //Case 1b
      {
        rotateRight(node);
        node -> setBalance(-1);
        child -> setBalance(1);
      }
      else // Case 1c
      {
        AVLNode<Key, Value>* grandchild = child -> getRight();
        rotateLeft(child);
        rotateRight(node);
        if(grandchild -> getBalance() == 1)
        {
          node -> setBalance(0);
          child -> setBalance(-1);
          grandchild -> setBalance(0);
        }
        else if (grandchild -> getBalance() == 0)
        {
          node -> setBalance(0);
          child -> setBalance(0);
          grandchild -> setBalance(0);
        }
        else
        {
          node -> setBalance(1);
          child -> setBalance(0);
          grandchild -> setBalance(0);
        }
        removeFix(parent, ndiff);
      }
    }
    else if (node -> getBalance() + diff == -1) //Case 2
    {
      node -> setBalance(-1);
    }
    else if(node -> getBalance() + diff == 0) // Case 3
    {
      node -> setBalance(0);
      removeFix(parent, ndiff);
    }
  }
  if(diff == 1) //Right Subtree Taller
  {
    if(node -> getBalance() + diff == 2) //Case 1
    {
      AVLNode<Key, Value>* child = node -> getRight();
      if(child -> getBalance() == 1) //Case 1a
      {
        rotateLeft(node);
        node -> setBalance(0);
        child -> setBalance(0);
        removeFix(parent, ndiff);
      }
      else if (child -> getBalance() == 0) //Case 1b
      {
        rotateLeft(node);
        node -> setBalance(1);
        child -> setBalance(-1);
      }
      else // Case 1c
      {
        AVLNode<Key, Value>* grandchild = child -> getLeft();
        rotateRight(child);
        rotateLeft(node);
        if(grandchild -> getBalance() == -1)
        {
          node -> setBalance(0);
          child -> setBalance(1);
          grandchild -> setBalance(0);
        }
        else if (grandchild -> getBalance() == 0)
        {
          node -> setBalance(0);
          child -> setBalance(0);
          grandchild -> setBalance(0);
        }
        else
        {
          node -> setBalance(-1);
          child -> setBalance(0);
          grandchild -> setBalance(0);
        }
        removeFix(parent, ndiff);
      }
    }
    else if (node -> getBalance() + diff == 1) //Case 2
    {
      node -> setBalance(1);
    }
    else if(node -> getBalance() + diff == 0) // Case 3
    {
      node -> setBalance(0);
      removeFix(parent, ndiff);
    }
  }
}

#endif



