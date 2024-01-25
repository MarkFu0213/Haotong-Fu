#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"
#include <string>

struct KeyError { };

// AVLNode class
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int getHeight () const;
    void setHeight (int height);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. 
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int height_;


};

template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), height_(1)
{

}

template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

template<class Key, class Value>
int AVLNode<Key, Value>::getHeight() const
{
    return height_;
}

template<class Key, class Value>
void AVLNode<Key, Value>::setHeight(int height)
{
    height_ = height;
}

template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}


template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}

// AVLTree class
template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    
    // helper functions for insert and remove
    AVLNode<Key, Value>* helper_insert_avl(AVLNode<Key, Value>* node, const std::pair<const Key, Value> &keyValuePair);
    AVLNode<Key, Value>* remove_helper_avl(AVLNode<Key, Value>* node, const Key& key);

    // helper for rotations and height updates
    void left_rotation(AVLNode<Key,Value>* z);
    void right_rotation(AVLNode<Key,Value>* z);
    void left_right_rotation(AVLNode<Key,Value>* z);
    void right_left_rotation(AVLNode<Key,Value>* z);
    void updateHeight(AVLNode<Key,Value>* node);
    void balance_tree(AVLNode<Key,Value>* node);
    int getHeightDifference(AVLNode<Key,Value>* node);
};

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key, Value>* node = helper_insert_avl(static_cast<AVLNode<Key, Value>*>(this->root_), new_item);

    //If the tree was empty, set the new node as the root
    if (this->root_ == nullptr) {
        this->root_ = node;
    }


    // Balance the tree starting from the inserted node up to the root
    while (node != nullptr) {
        updateHeight(node);
        balance_tree(node);
        node = node->getParent();
    }

    // making sure that root_ is pointing to the actual root
    while (this->root_ != nullptr && this->root_->getParent() != nullptr) {
        this->root_ = static_cast<AVLNode<Key, Value>*>(this->root_->getParent());
    }
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::helper_insert_avl(AVLNode<Key, Value>* node, const std::pair<const Key, Value> &keyValuePair)
{
    //if the node is null, just create the new node and return with null height
    if (node == nullptr) {
        return new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, nullptr);
    }

    //Again, I recurively call this function to find the key, it is found, set the child/parent pointers accordingly
    if (keyValuePair.first < node->getKey()) {
        //place found
        if (node->getLeft() == nullptr) {
            node->setLeft(new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, node));
            return node->getLeft();
        } else {
            //recursively call the left child
            return helper_insert_avl(node->getLeft(), keyValuePair);
        }
    } else if (keyValuePair.first > node->getKey()) {
        //place found
        if (node->getRight() == nullptr) {
            node->setRight(new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, node));
            return node->getRight();
        } else {
             //recursively call the right child
            return helper_insert_avl(node->getRight(), keyValuePair);
        }
    } else {
        // this is the place where when Key already exists, update the value
        node->setValue(keyValuePair.second);
        return node;
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    // Call the removal helper to remove the node and get the node to start rebalancing
    AVLNode<Key, Value>* startNode = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::remove_helper(this->root_, key));

    //if it is null, just stop
    if(startNode == nullptr)
    {
        return;
    }
    // if its parent is null, then it would be a root
    if(startNode->getParent() == nullptr)
    {
        this->root_ = startNode;
    }
    
    while (startNode != nullptr) {
        // Update the height of the start node
        updateHeight(startNode);
        // Balance the tree at the start node
        balance_tree(startNode);
        startNode = startNode->getParent();
    }
}

//nodeswap 
template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int tempH = n1->getHeight();
    n1->setHeight(n2->getHeight());
    n2->setHeight(tempH);
}


//this function does a left rotation, 
template<class Key, class Value>
void AVLTree<Key, Value>::left_rotation(AVLNode<Key,Value>* z)
{
    //it first does the left side, changing all the pointers
    AVLNode<Key,Value>* y = z->getRight(); 
    z->setRight(y->getLeft());

    if (y->getLeft() != nullptr) {
        y->getLeft()->setParent(z); 
    }

    y->setParent(z->getParent()); 

    //it then does the right side, changing all the pointers
    if (z->getParent() == nullptr) { 
        this->root_ = y; 
    } else if (z == z->getParent()->getLeft()) { 
        z->getParent()->setLeft(y);
    } else { 
        z->getParent()->setRight(y);
    }

    y->setLeft(z); 
    z->setParent(y); 

    //updating heights of z and y (only for single rotation)
    updateHeight(z);
    updateHeight(y);
   

}


//this function does a right rotation, 
template<class Key, class Value>
void AVLTree<Key, Value>::right_rotation(AVLNode<Key,Value>* z)
{

    //it first does the right side, changing all the pointers
    AVLNode<Key,Value>* y = z->getLeft(); 
    z->setLeft(y->getRight());

    if (y->getRight() != nullptr) {
        y->getRight()->setParent(z);
    }

    y->setParent(z->getParent()); 

    //it then does the left side, changing all the pointers
    if (z->getParent() == nullptr) {
        this->root_ = y; 
    } else if (z == z->getParent()->getLeft()) { 
        z->getParent()->setLeft(y); 
    } else { 
        z->getParent()->setRight(y); 
    }

    y->setRight(z); 
    z->setParent(y); 

    //updating heights of z and y
    updateHeight(z);
    updateHeight(y);

}

//this function does a right and then left, double rotation
template<class Key, class Value>
void AVLTree<Key, Value>::right_left_rotation(AVLNode<Key,Value>* z)
{
    AVLNode<Key,Value>* y = z->getRight(); 
    AVLNode<Key,Value>* x = y->getLeft(); 
    
    //do the right hand side first
    y->setLeft(x->getRight());
    if (x->getRight() != nullptr) {
        x->getRight()->setParent(y);
    }
    x->setRight(y);
    y->setParent(x);

    // then do the left hand side
    z->setRight(x->getLeft());
    if (x->getLeft() != nullptr) {
        x->getLeft()->setParent(z);
    }

    //connect x to the original parent, and x becomes the root
    x->setParent(z->getParent());
    if (z->getParent() == nullptr) { 
        this->root_ = x; 
    } else if (z == z->getParent()->getLeft()) { 
        z->getParent()->setLeft(x); 
    } else { 
        z->getParent()->setRight(x);
    }
    // connect x and z
    x->setLeft(z);
    z->setParent(x);

    // Update heights of xyz
    updateHeight(z);
    updateHeight(y);
    updateHeight(x);

}


//this function does a left and then right rotation, double rotation
template<class Key, class Value>
void AVLTree<Key, Value>::left_right_rotation(AVLNode<Key,Value>* z)
{
    AVLNode<Key,Value>* y = z->getLeft(); 
    AVLNode<Key,Value>* x = y->getRight(); 
    
    //left hand side first
    y->setRight(x->getLeft());
    if (x->getLeft() != nullptr) {
        x->getLeft()->setParent(y);
    }
    x->setLeft(y);
    y->setParent(x);

    //right hand side 
    z->setLeft(x->getRight());
    if (x->getRight() != nullptr) {
        x->getRight()->setParent(z);
    }

    //connect x to the original parent, x becomes new root
    x->setParent(z->getParent());
    if (z->getParent() == nullptr) { 
        this->root_ = x; 
    } else if (z == z->getParent()->getLeft()) { 
        z->getParent()->setLeft(x); 
    } else { 
        z->getParent()->setRight(x);
    }
    
    //then connect z and x, and then update the heights of these xyz
    x->setRight(z);
    z->setParent(x);

    updateHeight(z);
    updateHeight(y);
    updateHeight(x);
}

//this helper function updates the height of a node
template<class Key, class Value>
void AVLTree<Key, Value>::updateHeight(AVLNode<Key,Value>* node)
{

    if (node == nullptr) 
    {
        return;
    }

    int leftH = 0; 
    int rightH = 0;
    //it check the left child and right child's height, and then add 1 to the max of them
    if (node->getLeft() != nullptr) {
        leftH = node->getLeft()->getHeight();
    }
    if (node->getRight() != nullptr) {
        rightH = node->getRight()->getHeight();
    }
    //set to max + 1
    node->setHeight(1 + std::max(leftH, rightH));
}

//this functoion calculates the height difference of its left and right child
template<class Key, class Value>
int AVLTree<Key, Value>::getHeightDifference(AVLNode<Key,Value>* node)
{
    if(node == nullptr)
    {
        return 0;
    }
    int leftH = 0;
    int rightH = 0;
    //if one side is null, just set the height to 0
    if(node -> getLeft() != nullptr)
    {
        leftH = node->getLeft()->getHeight();
    }
    else{
        leftH = 0;
    }
    if(node -> getRight() != nullptr)
    {
        rightH = node->getRight()->getHeight();
    }
    else{
        rightH = 0;
    }
    //it could be both pos and neg number
    return leftH - rightH;
}

//this function uses the height difference of a node and perform rotations accordinly
template<class Key, class Value>
void AVLTree<Key, Value>::balance_tree(AVLNode<Key,Value>* node)
{
    if (node == nullptr) 
    {
        return;
    }

    int difference = getHeightDifference(node);

    // if the difference is larger than 1, then it is a left heavy subtree
    if (difference > 1)
    {
        // If left child is left heavey or same height, a single right rotation is needed. 
        if (getHeightDifference(node->getLeft()) >= 0)
        {
            right_rotation(node);
        }
        // do a double rotation: left right rotation
        else
        {
            left_right_rotation(node);
        }
    }
    // if the difference is smaller than 1, then it is a right heavy subtree
    else if (difference < -1)
    {
        // If right child is right heavy or same height. a single left rotation is needed.
        if (getHeightDifference(node->getRight()) <= 0)
        {
            left_rotation(node);
        }
        // If right child is left heavy, do a right left rotation.
        else
        {
            right_left_rotation(node);
        }
    }
}



#endif
