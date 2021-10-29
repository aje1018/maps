// A generic Map implemented with right-threaded BST
// BST is not balanced

#include <map> // helper container for thread copying



/*					 Public Functions					 */



template <typename KEY, typename T>
Map<KEY, T>::Map() {
	// create a dummy root node
	_root = new Elem;
	_root->left = _root;  // empty tree
	_root->right = 0;
	_root->rightThread = false;
	_size = 0;

}

// copy constructor 
template <typename KEY, typename T>
Map<KEY, T>::Map(const Map<KEY, T>& rhs) {
	// if empty tree
	if (rhs._root == rhs._root->left) {
		_root = new Elem;
		_root->left = _root;  // empty tree
		_root->right = 0;
		_size = 0;
	}
	else {
		_root = new Elem;
		_root->left = _root;
		_root->right = 0;
		copyCode(_root->left, rhs._root->left); // to deep copy the tree without dummy nodes
		copyThread(_root, rhs._root); // to copy the threading; must pass dummy nodes to complete the threads
		_size = rhs._size;
	}
}
template <typename KEY, typename T>
Map<KEY, T>::~Map() {
	if (_root->left != _root)
		destructCode(_root->left);
	delete _root;
}

// assignment operator
template <typename KEY, typename T>
Map<KEY, T>& Map<KEY, T>::operator=(const Map& rhs) {
	if (rhs._root == rhs._root->left) {
		_root = new Elem;
		_root->left = _root;
		_root->right = 0;
		_size = 0;
	}
	else {
		_root = new Elem;
		_root->left = _root;
		_root->right = 0;
		copyCode(_root->left, rhs._root->left);
		copyThread(_root, rhs._root);
		_size = rhs._size;
	}
	return *this;
}

//insert an element into the Map, return true if successful
template <typename KEY, typename T>
bool Map<KEY, T>::insert(KEY k, T t) {
	if (insert(_root->left, k, t, _root)) {
		_size++;
		/*calcHeights(1, _root->left);
		balanceTree(_root->left);*/
		return true;
	}
	/*else {
		this->operator[](k) = t;
	}*/
	return false;
}



// return size of the map
template <typename KEY, typename T>
int Map<KEY, T>::size() const {
	return _size;
}

// return an iterator pointing to the end if the element is not found,
// otherwise, return an iterator to the element
template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::find(KEY k) const {
	for (Iterator it = begin(); it != end(); it++) {
		if (it->key == k) {
			return it;
		}
	}
	return Iterator(_root);
}

//overloaded subscript operator
template <typename KEY, typename T>
T& Map<KEY, T>::operator[](KEY k) {
	insert(k, T());
	return find(k)->data;
}

// return the left most (smallest) tree node
template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::begin() const {
	if (_root->left == _root) {
		return Iterator(_root);
	}
	Elem* e = _root->left;
	while (e->left != nullptr) {
		e = e->left;
	}
	return Iterator(e);
}

// return the dummy root node
template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::end() const {
	return Iterator(_root);
}

// outputs information in tree in inorder traversal order
template <typename KEY, typename T>
ostream& Map<KEY, T>::dump(ostream& out) const {
	if (_root == _root->left) {// tree empty
		return out;
	}
	printTree(out, 0, _root->left);   // print tree structure
	return out;
}



/*					 Private Functions					 */



//helper method for inserting record into tree
template <typename KEY, typename T>
bool Map<KEY, T>::insert(Elem*& root, const KEY& key, const T& data, Elem* lastLeft) {

	if (root == _root || root == 0) {
		root = new Elem;
		root->key = key;
		root->data = data;
		root->left = 0;
		root->right = lastLeft;
		root->rightThread = true;
		root->height = 0;
		return true;
	}

	if (key == root->key) {
		return false;
	}

	if (key < root->key) {

		if (insert(root->left, key, data, root)) {

			if (!root->rightThread) {

				if (height(root->left) - height(root->right) == 2) {

					if (key < root->left->key) {
						rotateRight(root);
					}
					else {
						doubleRotateRight(root);
					}
				}

				if (height(root->left) - height(root->right) == -2) {

					if (key > root->right->key) {
						rotateLeft(root);
					}
					else {
						doubleRotateLeft(root);
					}
				}
			}
			else {
				if (height(root->left) + 1 == 2) {

					if (key < root->left->key) {
						rotateRight(root);
					}
					else {
						doubleRotateRight(root);
					}
				}

				if (height(root->left) + 1 == -2) {

					if (key > root->right->key) {
						rotateLeft(root);
					}
					else {
						doubleRotateLeft(root);
					}
				}
			}
			if (!root->rightThread) {
				root->height = max(height(root->left), height(root->right)) + 1;
			}
			else {
				root->height = height(root->left) + 1;
			}
			return true;
		}
		else {
			return false;
		}
	}

	if (!root->rightThread) {

		if (insert(root->right, key, data, lastLeft)) {

			if (!root->rightThread) {

				if (height(root->left) - height(root->right) == 2) {

					if (key < root->left->key) {
						rotateRight(root);
					}
					else {
						doubleRotateRight(root);
					}
				}

				if (height(root->left) - height(root->right) == -2) {

					if (key > root->right->key) {
						rotateLeft(root);
					}
					else {
						doubleRotateLeft(root);
					}
				}
				
			}
			else {
				if (height(root->left) + 1 == 2) {

					if (key < root->left->key) {
						rotateRight(root);
					}
					else {
						doubleRotateRight(root);
					}
				}

				if (height(root->left) + 1 == -2) {

					if (key > root->right->key) {
						rotateLeft(root);
					}
					else {
						doubleRotateLeft(root);
					}
				}
			}
			if (!root->rightThread) {
				root->height = max(height(root->left), height(root->right)) + 1;
			}
			else {
				root->height = height(root->left) + 1;
			}
			return true;
		}
		else {
			return false;
		}
	}
	else {
		root->rightThread = false;
		root->height = 1;
		root->right = new Elem;
		root->right->key = key;
		root->right->data = data;
		root->right->left = 0;
		root->right->right = lastLeft;
		root->right->height = 0;
		root->right->rightThread = true;
		return true;
	}
}

//// calculate the height of all nodes in the tree
//template <typename key, typename t>
//void map<key, t>::calcheights(int height, elem*& node) {
//	node->height = height;
//	if (node->left != nullptr) {
//		calcheights(height + 1, node->left);
//	}
//	if (!node->rightthread) {
//		calcheights(height + 1, node->right);
//	}
//}
//
//// balance the tree with proper rotate functions
//template <typename key, typename t>
//void map<key, t>::balancetree(elem*& node) {
//
//}

// output the structure of tree. The tree is output as "lying down"
// in which _root is the LEFT most Elem.
template <typename KEY, typename T>
void Map<KEY, T>::printTree(ostream& out, int level, Elem* p) const {
	int i;
	if (p) {
		if (p->right && !p->rightThread)
			printTree(out, level + 1, p->right);
		for (i = 0; i < level; i++) {
			out << "\t";
		}
		out << p->key << " " << p->data << "(" << p->height << ")" << '\n';
		printTree(out, level + 1, p->left);
	}
}

//common code for deallocation
template <typename KEY, typename T>
void Map<KEY, T>::destructCode(Elem*& p) {
	Elem* e = p;
	if (e->left != nullptr) {
		destructCode(e->left);
	}
	if (e->right != nullptr && !e->rightThread) {
		destructCode(e->right);
	}
	delete e;
}

// common copy code for deep copy a tree without copying threads
template <typename KEY, typename T>
void  Map<KEY, T>::copyCode(Elem*& newRoot, Elem* origRoot) {
	if (origRoot == 0) {
		newRoot = 0;
	}
	else {
		newRoot = new Elem;
		newRoot->key = origRoot->key;
		newRoot->data = origRoot->data;
		newRoot->height = origRoot->height;
		newRoot->rightThread = origRoot->rightThread;
		copyCode(newRoot->left, origRoot->left);
		if (!origRoot->rightThread)
			copyCode(newRoot->right, origRoot->right);
	}
}

// construct a key-element map to rethread the new tree
// the map contains all nodes key values and their corresonding elem node address 
// for furture lookup in setting up threads
template <typename KEY, typename T>
void Map<KEY, T>::addToMap(Elem* root, map<KEY, Elem*>& keyElemMap) {
	if (root) {
		keyElemMap[root->key] = root;
		addToMap(root->left, keyElemMap);
		if (!root->rightThread)
			addToMap(root->right, keyElemMap);
	}
}

// common copy code for thread copying
template <typename KEY, typename T>
void Map<KEY, T>::copyThread(Elem*& newRoot, Elem* origRoot) {
	// construct the key-element map for new and orig tree
	map<KEY, Elem*> newKeyElemMap;
	map<KEY, Elem*> origKeyElemMap;
	addToMap(newRoot->left, newKeyElemMap);
	addToMap(origRoot->left, origKeyElemMap);

	// start at the last element in the tree, which threads to root
	typename std::map<KEY, Elem*>::reverse_iterator it = origKeyElemMap.rbegin();
	newKeyElemMap[it->first]->rightThread = true;
	newKeyElemMap[it->first]->right = newRoot;

	// then thread the rest of the tree backwardly 
	it++;
	while (it != origKeyElemMap.rend()) {
		if (it->second->rightThread) {
			newKeyElemMap[it->first]->rightThread = true;
			newKeyElemMap[it->first]->right = newKeyElemMap[origKeyElemMap[it->first]->right->key];
		}
		it++;
	}
}

template<typename KEY, typename T>
int Map<KEY, T>::height(Elem* node) {
	return node == 0 ? -1 : node->height;
}

template<typename KEY, typename T>
void Map<KEY, T>::rotateRight(Elem*& node) {
	Elem* e1 = node->left;
	if (e1->rightThread) {
		e1->rightThread = false;
		node->left = nullptr;
	}
	else {
		node->left = e1->right;
	}
	e1->right = node;

	// update heights
	if (!node->rightThread) {
		node->height = max(height(node->left), height(node->right)) + 1;
	}
	else {
		node->height = height(node->left) + 1;
	}
	if (!e1->rightThread) {
		e1->height = max(height(e1->left), height(e1->right)) + 1;
	}
	else {
		e1->height = height(e1->left) + 1;
	}
	node = e1;
}

// single left rotation
template<typename KEY, typename T>
void Map<KEY, T>::rotateLeft(Elem*& node) {
	Elem* e1 = node->right;
	if (e1->left == nullptr) {
		node->rightThread = true;
		node->right = e1;
	}
	else {
		node->right = e1->left;
	}
	e1->left = node;

	// Update heights
	if (!node->rightThread) {
		node->height = max(height(node->left), height(node->right)) + 1;
	}
	else {
		node->height = height(node->left) + 1;
	}
	if (!e1->rightThread) {
		e1->height = max(height(e1->left), height(e1->right)) + 1;
	}
	else {
		e1->height = height(e1->left) + 1;
	}

	// Return new root
	node = e1;
}

// double right rotation
template<typename KEY, typename T>
void Map<KEY, T>::doubleRotateRight(Elem*& node) {
	rotateLeft(node->left);
	rotateRight(node);
}

// double left rotation
template<typename KEY, typename T>
void Map<KEY, T>::doubleRotateLeft(Elem*& node) {
	if (node->rightThread) {
		return;
	}
	rotateRight(node->right);
	rotateLeft(node);
}



/*						Iterator Functions					*/



template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::Iterator::operator++(int x) {
	Iterator* it = this;
	if (it->_cur->right && it->_cur->rightThread) {
		it->_cur = it->_cur->right;
	}
	else if (it->_cur->right && !it->_cur->rightThread) {
		it->_cur = it->_cur->right;
		while (it->_cur->left) {
			it->_cur = it->_cur->left;
		}
	}
	else {
		return Iterator(0);
	}
	return Iterator(it->_cur);
}

template <typename KEY, typename T>
typename Map<KEY, T>::Elem& Map<KEY, T>::Iterator::operator*() {
	return *_cur;
}

template <typename KEY, typename T>
typename Map<KEY, T>::Elem* Map<KEY, T>::Iterator::operator->() {
	return _cur;
}

template <typename KEY, typename T>
bool Map<KEY, T>::Iterator::operator==(Iterator it) {
	return this->_cur == it._cur;
}

template <typename KEY, typename T>
bool Map<KEY, T>::Iterator::operator!=(Iterator it) {
	return this->_cur != it._cur;
}


// outputs using overloaded << operator
template<typename KEY, typename T>
ostream& operator<< (ostream& out, const Map<KEY, T>& v) {
	v.dump(out);
	return out;
}
