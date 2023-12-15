#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
#include <algorithm>
#include <string>


/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed within the deleteAll() helper method in the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree(); //TODO
    virtual ~BinarySearchTree(); //TODO
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); //TODO
    virtual void remove(const Key& key); //TODO
    void clear(); //TODO
    bool isBalanced() const; //TODO
    void print() const;
    bool empty() const;
    
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator  // TODO
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const; // TODO
    Node<Key, Value> *getSmallestNode() const;  // TODO
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); // TODO
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2);

    // Add helper functions here
    Node<Key, Value>* helper_insert(Node<Key, Value>* the_root, const std::pair<const Key, Value> &keyValuePair);
    Node<Key, Value>* remove_helper(Node<Key, Value>* node, const Key& key);
    bool isBalanced_helper(Node<Key, Value>* n, int& height) const;
    void clearHelper(Node<Key, Value>* node);


protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr)
{
    current_ = ptr;
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator()
{
    current_ = nullptr;
}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool 
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return current_ == rhs.current_;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return current_ != rhs.current_;
}


/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    //if current is nullptr then we just return the this as it is
    if(current_ == nullptr) {
        return *this;
    }
    //go to the leftmost child of its right child if there is one
    if(current_->getRight() != nullptr)
    {
        current_ = current_ -> getRight();
        while(current_->getLeft() != nullptr)
        {
            current_ = current_->getLeft();
        }
    }
    else{
        //traverse until the node is not the right child
        Node<Key, Value> *temp = current_->getParent();
        while(temp != nullptr && current_ == temp->getRight())
        {
            current_ = temp;
            temp = temp->getParent();
        }
        current_ = temp;
    }
    return *this;
}


/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() 
{
    root_ = nullptr;
}

//call clear function that delete the entire tree
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    clear();
}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    // TODO
    if (!root_) {
        // If the tree is empty, create a new root node.
        root_ = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, nullptr);
    } else {
        // Otherwise, just call the helper insert function.
        helper_insert(root_, keyValuePair);
    }
    
}

template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::helper_insert(Node<Key, Value>* the_root, const std::pair<const Key, Value> &keyValuePair)
{
    // Check if the key is less than the node's key.
    if(keyValuePair.first < the_root->getKey()) {
        if (the_root->getLeft() == nullptr) {
            // Insert here if there is no leftchild, and set it as the original node's left child
            the_root->setLeft(new Node<Key, Value>(keyValuePair.first, keyValuePair.second, the_root));
        } else {
            // Otherwise, recurse on the left subtree until we found it
            helper_insert(the_root->getLeft(), keyValuePair);
        }
    } 
    // Check if the key is larger than the node's key
    else if(keyValuePair.first > the_root->getKey()) {
        // Insert here if there is no rightchild, and set it as the original node's right child
        if (the_root->getRight() == nullptr) {
            the_root->setRight(new Node<Key, Value>(keyValuePair.first, keyValuePair.second, the_root));
        } else {
            //otherwise, recursively call it
            helper_insert(the_root->getRight(), keyValuePair);
        }
    } else {
        // If the key is same, update the value.
        the_root->setValue(keyValuePair.second);
    }
    return the_root;
}

/**
* A remove method to remove a specific key from a Binary Search Tree.
* The tree may not remain balanced after removal.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    remove_helper(root_, key);
}




template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::remove_helper(Node<Key, Value>* node, const Key& key) {

    // if the node removed is null, just return null, do nothing
    if (node == nullptr) {
        return nullptr;
    }

    // we find the key using internalfind function
    node = internalFind(key);

    if (node != nullptr) {
        // found the node to be deleted
        if (node->getLeft() == nullptr && node->getRight() == nullptr) {
            // Case 1: Node is a leaf without children, just delete it
            if (node == root_) {
                root_ = nullptr;
                delete node;
                return nullptr;
            }

            //reset the delete node's parent's child pointers to nullptr
            Node<Key, Value>* temp = node->getParent();
            if(temp->getLeft() == node)
            {
                temp->setLeft(nullptr);
            }
            if(temp->getRight() == node)
            {
                temp->setRight(nullptr);
            }
            
            //return the parent of the deleted node
            temp = node->getParent();
            delete node;
            return temp;
        } 

        //if the node has a left child and no right child
        else if (node->getLeft() != nullptr && node->getRight() == nullptr) {
            // Case 2: Node has only a left child, reset their parent/children pointers
            Node<Key, Value>* temp = node->getLeft();
            
            //swap it and then promote the delete node's child, and connect it to the deleted node's parent
            nodeSwap(node, temp);
            if(node->getRight()!=NULL){
                temp->setRight(node->getRight());
                node->getRight()->setParent(temp);
            }
            temp->setLeft(NULL);
            if(node->getLeft()!=NULL){
                temp->setLeft(node->getLeft());
                node->getLeft()->setParent(temp);
            }
            temp = node->getParent();
            delete node;
            return temp;
        } 
        
        else if (node->getLeft() == nullptr && node->getRight() != nullptr) {
            // Case 3: Node has only a right child, reset their parent/children pointers
            Node<Key, Value>* temp = node->getRight();
            nodeSwap(node, temp);
            //swap it and then promote the delete node's child, and connect it to the deleted node's parent
            if(node->getLeft()!=NULL){
                temp->setLeft(node->getLeft());
                node->getLeft()->setParent(temp);
            }
            temp->setRight(NULL);
            if(node->getRight()!=NULL){
                temp->setRight(node->getRight());
                node->getRight()->setParent(temp);
            }
            temp = node->getParent();
            delete node;
            return temp;
        } else {
            // Case 4: Node has two children
            Node<Key, Value>* succ = node->getRight();
            //traverse until the very left of the right child, to find the successor
            while (succ->getLeft() != nullptr) {
                succ = succ->getLeft();
            }
            //swap with the successor, and then we perform check on the deleted node (in successor's original position)
            //then I just copied the above code to check how many child it has and do the pointer swapping
            nodeSwap(node, succ);
            if (node->getLeft() == nullptr && node->getRight() == nullptr) {
            // Case 4-1: Node is a leaf without children, just delete it
            if (node == root_) {
                root_ = nullptr;
                return nullptr;
            }
            Node<Key, Value>* temp = node->getParent();
            //reset the delete node's parent's child pointers to nullptr
            if(temp->getLeft() == node)
            {
                temp->setLeft(nullptr);
            }
            if(temp->getRight() == node)
            {
                temp->setRight(nullptr);
            }
            
            delete node;
            return temp;
        } 
        //if the node has a left child and no right child
            else if (node->getLeft() != nullptr && node->getRight() == nullptr) {
                // Case 4-2: Node has only a left child, reset their parent/children pointers
                Node<Key, Value>* temp = node->getLeft();
                 //swap it and then promote the delete node's child, and connect it to the deleted node's parent
                nodeSwap(node, temp);
                if(node->getRight()!=NULL){
                    temp->setRight(node->getRight());
                    node->getRight()->setParent(temp);
                }
                temp->setLeft(NULL);
                if(node->getLeft()!=NULL){
                    temp->setLeft(node->getLeft());
                    node->getLeft()->setParent(temp);
                }
                temp = node->getParent();
                delete node;
                return temp;
            } 
            else if (node->getLeft() == nullptr && node->getRight() != nullptr) {
                // Case 4-3: Node has only a right child, reset their parent/children pointers
                Node<Key, Value>* temp = node->getRight();
                 //swap it and then promote the delete node's child, and connect it to the deleted node's parent
                nodeSwap(node, temp);
                if(node->getLeft()!=NULL){
                    temp->setLeft(node->getLeft());
                    node->getLeft()->setParent(temp);
                }
                temp->setRight(NULL);
                if(node->getRight()!=NULL){
                    temp->setRight(node->getRight());
                    node->getRight()->setParent(temp);
                }
                //return its parent
                temp = node->getParent();
                delete node;
                return temp;
            }          
        }
    }
    return node;
}


template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    // if this node has a left child, then we traverse it to the right most of its left child
    if(current->getLeft() != nullptr)
    {
        Node<Key, Value>* temp = current->getLeft();
        while(temp->getRight() != nullptr)
        {
            temp = temp->getRight();
        }
        return temp;
    }
    else{
        //if the node does not have a left child, we first check if it is a right child of a parent
        //if yes, then just return its parent
        if(current->getParent()->getRight() == current)
        {
            return current->getParent();
        }
        else{
            //if not, we traverse it upward until we found the node that is the right child of a parent
            //then we just return that parent
            Node<Key, Value>* temp = current;
            Node<Key, Value>* parent = temp->getParent();

            while (parent != nullptr && temp == parent->getLeft()) {
                temp = parent;
                parent = parent->getParent();
            }
            // This could be nullptr if the current node is the minimum node in the tree
            return parent; 

        }
    }
    return current;
}


/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear() {
    clearHelper(root_);
    root_ = nullptr;
}

//recursively call the left child and right child to delete the entire tree
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearHelper(Node<Key, Value>* n) {
    if(n != nullptr) {
        clearHelper(n->getLeft());
        clearHelper(n->getRight());
        delete n;
    }
}


/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    //traverse until the very left and return it
    Node<Key, Value>* temp = root_;
    while(temp && temp->getLeft() != nullptr)
    {
        temp = temp->getLeft();
    }
    return temp;
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    // this function return the node with the passed in key with while loop, if not found return null
    Node<Key, Value>* temp = root_;
    while (temp) {
        if (key < temp->getKey()) {
            temp = temp->getLeft();
        } else if (key > temp->getKey()) {
            temp = temp->getRight();
        } else {
            return temp;
        }
    }
    return nullptr;

}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    // TODO
    int height = 0;
    return isBalanced_helper(root_, height);
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced_helper(Node<Key, Value>* n, int& height) const
{
    if(n == nullptr)
    {
        height = -1;
        return true;
    }
    //I recursively call the left hand right and right hand to see if every node is balance
    int leftH = 0;
    int rightH = 0;
    bool leftBalanced = isBalanced_helper(n->getLeft(), leftH);
    bool rightBalanced = isBalanced_helper(n->getRight(), rightH);
    //specifically it calculates the max of its left and right child and add 1
    height = std::max(leftH, rightH) +1;
    // if the absolute value of the height difference is larger than 1 or if either of the above bool
    //variable return false, then return false
    if(abs(leftH - rightH) > 1)
    {
        return false;
    }
    if(!leftBalanced || !rightBalanced)
    {
        return false;
    }

    return true;
}


template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif