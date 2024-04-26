# Red–black tree

In computer science, a red–black tree is a kind of self-balancing binary search tree. Each node stores a "color" ("red" or "black"), used to ensure that the tree remains balanced during insertions and deletions. The re-balancing guarantees searching in O (log ⁡n), where n is the number of entries. The insert and delete operations, along with the tree rearrangement and recoloring, are also performed in O (log ⁡n) time.

## struct RedBlackNode
Structure, that represents node of the Red-black tree and links left and right to children nodes.

### Members
value - data with user-defined type.

color - variable instantiated by enum Color

parent - pointer to parent of the current node

left - pointer to left child of the current node

right - pointer to right child of the current node

### Functions

RedBlackNode* max() - returns maximum of the subtree with given node as root

RedBlackNode* min() - returns minimum of the subtree with given node as root

RedBlackNode* successor() - returns successor of the given node

RedBlackNode* predecessor() - returns predecessor(previous node) of the given node

## class RedBlackTree
Class, that represents a Red-black self-balancing binary search tree.
### Members
root - pointer to root of the Red-Black tree

### Functions
bool empty() - returns true if tree is empty

iterator begin() - returns iterator to minimum node of the tree

iterator end() - returns empty iterator (end of the tree)

iterator max() - returns iterator to maximum node of the tree

iterator min() - returns iterator to minimum node of the tree

nodeType* find(value_type val) - returns pointer to a node with given value

void insert(value_type val) - inserts new node with given value to the tree

void erase(value_type val) - deletes node with given value from the tree

void merge(RedBlackTree<value_type> tree2) - join two Red-Black trees to one Red-Black tree

void print() - visualizes the Red-Black tree

## class RedBlackIterator
Implementation of an iterator for Red-black tree.
### Members
iterator
