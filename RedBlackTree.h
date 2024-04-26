#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <iterator>
#include <string>
using std::iterator;
using std::bidirectional_iterator_tag;

enum Color { RED, BLACK };

template <typename T>
struct RedBlackNode {

	typedef typename T value_type;
	value_type value;
	Color color;
	RedBlackNode* parent, * left, * right;

	RedBlackNode(value_type val) : value(val), color(RED), parent(NULL), left(NULL), right(NULL) {}

	value_type& operator=(const RedBlackNode& node) {
		value = node.val;
		color = node.color;
		parent = node.parent;
		left = node.left;
		right = node.right;
	}

	RedBlackNode* max() {
		if (right == NULL) {
			return this;
		}
		else {
			return right->max();
		}
	}

	RedBlackNode* min() {
		if (left == NULL) {
			return this;
		}
		else {
			return left->min();
		}
	}

	RedBlackNode* successor() {

		if (right != NULL) {
			return right->min();
		}
		else if (parent != NULL && this == parent->left) {
			return parent;
		}
		else {
			RedBlackNode* s = this;
			while (true) {
				s = s->parent;
				if (s == NULL) {
					return NULL;
				}
				else if (s->parent != NULL && s == s->parent->left) {
					return s->parent;
				}
			}
		}
		return NULL;
	}

	RedBlackNode* predecessor() {

		if (left != NULL) {
			return left->max();
		}
		else if (parent != NULL && this == parent->right) {
			return parent;
		}
		else {
			RedBlackNode* s = this;
			while (true) {
				s = s->parent;
				if (s == NULL) {
					return NULL;
				}
				else if (s->parent != NULL && s == s->parent->right) {
					return s->parent;
				}
			}
		}
		return NULL;
	}

};




template< typename T >
class RedBlackIterator {
public:
	RedBlackNode<T>* iterator;

	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = typename T;
	using difference_type = std::ptrdiff_t;
	using pointer = typename T*;
	using reference = typename T&;

	RedBlackIterator() : iterator(NULL) { };
	RedBlackIterator(RedBlackNode<T>* ptr) : iterator(ptr) { }
	RedBlackIterator(const RedBlackIterator& that) : iterator(that.iterator) { }

	RedBlackIterator operator++() {
		iterator = iterator->successor();
		return *this;
	}

	RedBlackIterator operator++(int) {
		RedBlackIterator result = *this;
		iterator = iterator->successor();
		return result;
	}

	RedBlackIterator operator--() {
		iterator = iterator->predecessor();
		return *this;
	}

	RedBlackIterator operator--(int) {
		RedBlackIterator result = *this;
		iterator = iterator->predecessor();
		return result;
	}

	RedBlackIterator& operator=(const RedBlackIterator& that) {
		iterator->value = that;
		return *this;
	}

	bool operator==(const RedBlackIterator& that) const {
		return (iterator == that.iterator);
	}

	bool operator!=(const RedBlackIterator& that) const {
		return (iterator != that.iterator);
	}

	operator RedBlackNode<T>& () {
		return *iterator;
	}

	operator const RedBlackNode<T>& () const {
		return *iterator;
	}

	T operator* () {
		return iterator->value;
	}

	pointer operator->() {
		return iterator;
	}
};




template< typename T >
class RedBlackTree {
public:
	//type definitions
	typedef typename RedBlackNode <T> nodeType;
	typedef typename T value_type;
	typedef RedBlackIterator <value_type> iterator;

	//constructor
	RedBlackTree() : root(NULL) {}

	//~RedBlackTree() {
	//	destroyRecursive(root);
	//}

	//void destroyRecursive(nodeType* node) {
	//	if (node == NULL) {
	//		return;
	//	}
	//	destroyRecursive(node->left);
	//	destroyRecursive(node->right);
	//	delete node;
	//}

	bool empty() const {
		return (root == NULL);
	}

	iterator begin() {
		return min();
	}
	iterator end() {
		return iterator();
	}
	iterator max() {
		if (empty()) {
			return end();
		}
		else {
			return iterator(root->max());
		}
	}
	iterator min() {
		if (empty()) {
			return end();
		}
		else {
			return iterator(root->min());
		}
	}

	nodeType* find(value_type val) {

		if (empty()) {
			return NULL;
		}

		nodeType* found = NULL;
		nodeType* current = root;

		while (current != NULL) {
			if (current->value == val) {
				found = current;
				break;
			}

			if (current->value < val) {
				current = current->right;
			}
			else {
				current = current->left;
			}
		}

		return found;
	}

	void insert(value_type val) {
		nodeType* node = new nodeType(val);
		insertHelp(node);
		//balancing after insertion
		insertFix(node);
	}

	void erase(value_type val) {

		//find node to delete
		nodeType* found = find(val);

		//delete node if it exists
		if (found != NULL) {
			eraseHelp(found);
		}
	}

	void print() {
		if (!empty()) {
			printHelp(this->root, "", true);
		}
	}

	void merge(RedBlackTree<value_type> tree2) {
		// we can merge trees iff all the nodes belonging to tree1 <= all nodes of tree2 (due to algorithm)

		value_type current_value;
		nodeType* current_node;
		nodeType* current_ptr = NULL;
		nodeType* root1 = root;
		nodeType* root2 = tree2.root;

		if (root1->max()->value > root2->min()->value) {
			std::cout << "All the nodes belonging to tree1 should be less than (in value) all nodes of tree2!" << std::endl;
			return;
		}

		int height1 = blackHeight(root1);
		int height2 = blackHeight(root2);

		//find current node and then delete it
		if (height1 > height2) {
			current_node = root1->max();
			current_value = current_node->value;
			erase(current_value);
			root1 = root;
		}
		else if (height2 > height1) {
			current_node = root2->min();
			current_value = current_node->value;
			tree2.erase(current_value);
			root2 = tree2.root;
		}
		else {
			current_node = root2->min();
			current_value = current_node->value;
			tree2.erase(current_value);
			root2 = tree2.root;

			if (height1 != blackHeight(root2)) {
				tree2.insert(current_value);
				root2 = tree2.root;
				current_node = root1->max();
				current_value = current_node->value;
				erase(current_value);
				root1 = root;
			}
		}

		setColor(current_node, RED);
		height1 = blackHeight(root1);
		height2 = blackHeight(root2);

		if (height1 == height2) {
			//set current node as root and tree1 its left subtree, tree2 its right subtree
			current_node->left = root1;
			root1->parent = current_node;
			current_node->right = root2;
			root2->parent = current_node;
			//root must be black
			setColor(current_node, BLACK);
			current_node->value = current_value;
			root = current_node;
		}
		else if (height2 > height1) {
			nodeType* current_ptr2 = root2;
			//find node, which black height will be same as height of tree1
			while (height1 != blackHeight(current_ptr2)) {
				current_ptr = current_ptr2;
				current_ptr2 = current_ptr2->left;
			}

			//find parent of this node
			nodeType* parent_ptr;
			if (current_ptr2 == NULL) {
				parent_ptr = current_ptr;
			}
			else {
				parent_ptr = current_ptr2->parent;
			}

			//set tree1 as left subtree of current_node
			current_node->left = root1;
			if (root1 != NULL) {
				root1->parent = current_node;
			}

			//set subtree which starts from current_ptr2 as right subtree of current node
			current_node->right = current_ptr2;
			if (current_ptr2 != NULL) {
				current_ptr2->parent = current_node;
			}

			//set sibling of current_ptr2 as current node
			parent_ptr->left = current_node;
			current_node->parent = parent_ptr;

			//balance tree
			if (getColor(parent_ptr) == RED) {
				insertFix(current_node);
			}
			else if (getColor(current_ptr2) == RED) {
				insertFix(current_ptr2);
			}

			current_node->value = current_value;
			root = root2;
		}
		else {
			nodeType* current_ptr2 = root1;
			//find node, which black height will be same as height of tree2
			while (height2 != blackHeight(current_ptr2)) {
				current_ptr2 = current_ptr2->right;
			}
			//find parent of this node
			nodeType* parent_ptr = current_ptr2->parent;
			//set right child of current node as root2 and left child as current_ptr2
			current_node->right = root2;
			root2->parent = current_node;
			current_node->left = current_ptr2;
			current_ptr2->parent = current_node;
			//set sibling of current_ptr2 as current node
			parent_ptr->right = current_node;
			current_node->parent = parent_ptr;
			//balance tree
			if (getColor(parent_ptr) == RED) {
				insertFix(current_node);
			}
			else if (getColor(current_ptr2) == RED) {
				insertFix(current_ptr2);
			}
			current_node->value = current_value;
			root = root1;
		}
		return;
	}

	nodeType* root;

private:

	Color getColor(nodeType* node) {
		if (node == NULL) {
			return BLACK;
		}
		return node->color;
	}

	void setColor(nodeType* node, Color color) {
		if (node == NULL) {
			return;
		}
		node->color = color;
	}

	void changeValues(nodeType* node1, nodeType* node2) {
		value_type temp;
		temp = node1->value;
		node1->value = node2->value;
		node2->value = temp;
	}

	nodeType* changeFind(nodeType* node) {
		if (node->left != NULL && node->right != NULL) {
			return (node->right)->min();
		}

		if (node->left == NULL && node->right == NULL) {
			return NULL;
		}

		if (node->left != NULL) {
			return node->left;
		}
		else {
			return node->right;
		}
	}

	int blackHeight(nodeType* node) {
		//number of black nodes under the given node
		int height = 0;
		while (node != NULL) {
			if (getColor(node) == BLACK) {
				height++;
			}
			node = node->left;
		}
		return height;
	}

	void leftRotate(nodeType* node) {
		//make node left child of its right child
		nodeType* helper = node->right;
		node->right = helper->left;

		if (node->right != NULL) {
			node->right->parent = node;
		}

		helper->parent = node->parent;

		if (node->parent == NULL) {
			root = helper;
		}
		else if (node == node->parent->left) {
			node->parent->left = helper;
		}
		else {
			node->parent->right = helper;
		}

		helper->left = node;
		node->parent = helper;
	}

	void rightRotate(nodeType* node) {
		//make node right child of its left child
		nodeType* helper = node->left;
		node->left = helper->right;

		if (node->left != NULL) {
			node->left->parent = node;
		}

		helper->parent = node->parent;

		if (node->parent == NULL) {
			root = helper;
		}
		else if (node == node->parent->left) {
			node->parent->left = helper;
		}
		else {
			node->parent->right = helper;
		}

		helper->right = node;
		node->parent = helper;
	}

	void insertHelp(nodeType* node) {

		if (empty()) {
			setColor(node, BLACK);
			root = node;
			return;
		}

		nodeType* parent = NULL;
		nodeType* current = root;

		while (current != NULL) {
			parent = current;
			if (node->value < current->value) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}

		node->parent = parent;

		if (node->value < parent->value) {
			parent->left = node;
		}
		else {
			parent->right = node;
		}

		return;
	}

	void insertFix(nodeType* node) {
		nodeType* uncle;

		if (node == root) {
			return;
		}

		//check the colour of the parent node
		//if its colour is black then dont change the colour
		//if its colour is red then check the colour of the nodes uncle
		while (getColor(node->parent) == RED) {
			//if parent is right child of grandparent -> uncle is left child
			if (node->parent == node->parent->parent->right) {
				uncle = node->parent->parent->left;
				//if uncle has a red colour (same as parent) 
				//then change the colour of uncle and parent to black and the colour of grandfather to red 
				//and repeat the same process for grandfather
				if (getColor(uncle) == RED) {
					setColor(uncle, BLACK);
					setColor(node->parent, BLACK);
					setColor(node->parent->parent, RED);
					node = node->parent->parent;
				}
				//if uncle has a black colour
				else {
					//right-left case
					if (node == node->parent->left) {
						node = node->parent;
						rightRotate(node);
					}
					//right-right case
					setColor(node->parent, BLACK);
					setColor(node->parent->parent, RED);
					leftRotate(node->parent->parent);
				}
			}
			//if parent is left child of grandparent -> uncle is right child
			else {
				uncle = node->parent->parent->right;
				//if uncle has a red colour (same as parent) 
				//then change the colour of uncle and parent to black and the colour of grandfather to red 
				//and repeat the same process for grandfather
				if (getColor(uncle) == RED) {
					setColor(uncle, BLACK);
					setColor(node->parent, BLACK);
					setColor(node->parent->parent, RED);
					node = node->parent->parent;
				}
				//if uncle has a black colour
				else {
					//left-right case
					if (node == node->parent->right) {
						node = node->parent;
						leftRotate(node);
					}
					//left-left case
					setColor(node->parent, BLACK);
					setColor(node->parent->parent, RED);
					rightRotate(node->parent->parent);
				}
			}
		}
		setColor(root, BLACK);
	}

	void eraseHelp(nodeType* node) {
		//we delete only a node which is a leaf or has only one child
		//node2 - the child that replace node
		nodeType* node2 = changeFind(node);
		nodeType* parent = node->parent;
		bool bothBlack = ((node2 == NULL || getColor(node2) == BLACK) && (getColor(node) == BLACK));

		//node is a leaf
		if (node2 == NULL) {
			if (node == root) {
				root = NULL;
			}
			else {
				if (bothBlack) {
					//node and node2 are black
					//balance the tree if node is a leaf
					eraseFix(node);
				}
				else {
					//node or node2 is red
					//if sibling is not NULL, set its color to red
					if (node == parent->left) {
						if (parent->right != NULL) {
							setColor(parent->right, RED);
						}
					}
					else {
						if (parent->left != NULL) {
							setColor(parent->left, RED);
						}
					}
				}

				//delete node from the tree
				if (node == parent->left) {
					parent->left = NULL;
				}
				else {
					parent->right = NULL;
				}
			}
			delete node;
			return;
		}

		//node has 1 child
		if (node->left == NULL || node->right == NULL) {
			if (node == root) {
				//assign the value of node2 to node and delete node2
				node->value = node2->value;
				node->left = node->right = NULL;
				delete node2;
			}
			else {
				//replace node with node2 and delete node
				if (node == parent->left) {
					parent->left = node2;
				}
				else {
					parent->right = node2;
				}
				delete node;
				node2->parent = parent;

				if (bothBlack) {
					//balance the tree if node and node2 are black
					eraseFix(node2);
				}
				else {
					//if node or node2 ais red, set node2 color to black
					setColor(node2, BLACK);
				}
			}
			return;
		}
		// id node has 2 children, change values with node2 and recurse
		changeValues(node2, node);
		eraseHelp(node2);
	}

	void eraseFix(nodeType* node) {
		if (node == root) {
			return;
		}

		nodeType* sibling;
		if (node == node->parent->left) {
			sibling = node->parent->right;
		}
		else {
			sibling = node->parent->left;
		}

		if (sibling == NULL) {
			//if not sibling, recurse to parent
			eraseFix(node->parent);
		}
		else {
			//sibling is red -> change its color to black and parent's color to black
			if (getColor(sibling) == RED) {
				setColor(node->parent, RED);
				setColor(sibling, BLACK);
				//sibling is left child
				if (sibling == sibling->parent->left) {
					rightRotate(node->parent);
				}
				//sibling is right child
				else {
					leftRotate(node->parent);
				}
				eraseFix(node);
			}
			//sibling is black
			else {
				//if sibling nas at least 1 red child
				if (getColor(sibling->left) == RED || getColor(sibling->right) == RED) {
					if (sibling->left != NULL and getColor(sibling->left) == RED) {
						//left-left case
						if (sibling == sibling->parent->left) {
							setColor(sibling->left, getColor(sibling));
							setColor(sibling, getColor(node->parent));
							rightRotate(node->parent);
						}
						//right-left case
						else {
							setColor(sibling->left, getColor(node->parent));
							rightRotate(sibling);
							leftRotate(node->parent);
						}
					}
					else {
						//left-right case
						if (sibling == sibling->parent->left) {
							setColor(sibling->right, getColor(node->parent));
							leftRotate(sibling);
							rightRotate(node->parent);
						}
						//right-right case
						else {
							setColor(sibling->right, getColor(sibling));
							setColor(sibling, getColor(node->parent));
							leftRotate(node->parent);
						}
					}
					setColor(node->parent, BLACK);
				}
				//if all childrens of sibling are black
				else {
					setColor(sibling, RED);
					if (getColor(node->parent) == BLACK) {
						eraseFix(node->parent);
					}
					else {
						setColor(node->parent, BLACK);
					}
				}
			}
		}
	}

	void printHelp(nodeType* node, std::string separator, bool last) {
		if (node != NULL) {
			std::cout << separator;
			if (last) {
				std::cout << "R--->";
			}
			else {
				std::cout << "L---->";
			}
			separator += "   ";

			std::string nodeColor = "BLACK";
			if (node->color == RED) {
				nodeColor = "RED";
			}
			std::cout << node->value << "(" << nodeColor << ")" << std::endl;
			printHelp(node->left, separator, false);
			printHelp(node->right, separator, true);
		}
	}

};

#endif