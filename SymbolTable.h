#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Symbol {
public:
    string id;
	bool isFunction;
    string paraList[10];
    string retType;
    int scopeLevel;
    Symbol *parent, *left, *right;
};

class SymbolTable
{
public:
    Symbol *root;
    int currentScope;

	SymbolTable() {
		root = nullptr;
		currentScope = 0;
	}

	void leftRotate(Symbol *x) {
        Symbol *y = x->right;
		x->right = y->left;
		if (y->left != nullptr) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	void rightRotate(Symbol *x) {
		Symbol *y = x->left;
		x->left = y->right;
		if (y->right != nullptr) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}
    
	void splay(Symbol *x) {
		while (x->parent) {
			if (!x->parent->parent) {
				if (x == x->parent->left) {
					// zig rotation
					rightRotate(x->parent);
				} else {
					// zag rotation
					leftRotate(x->parent);
				}
			} else if (x == x->parent->left && x->parent == x->parent->parent->left) {
				// zig-zig rotation
				rightRotate(x->parent->parent);
				rightRotate(x->parent);
			} else if (x == x->parent->right && x->parent == x->parent->parent->right) {
				// zag-zag rotation
				leftRotate(x->parent->parent);
				leftRotate(x->parent);
			} else if (x == x->parent->right && x->parent == x->parent->parent->left) {
				// zig-zag rotation
				leftRotate(x->parent);
				rightRotate(x->parent);
			} else {
				// zag-zig rotation
				rightRotate(x->parent);
				leftRotate(x->parent);
			}
		}
	}

	// joins two trees s and t
	Symbol *join(Symbol *s, Symbol *t){
		if (!s) {
			return t;
		}

		if (!t) {
			return s;
		}
		Symbol *x = maximum(s);
		splay(x);
		x->right = t;
		t->parent = x;
		return x;
	}

	// splits the tree into s and t
	void split(Symbol *&x, Symbol *&s, Symbol *&t) {
		splay(x);
		if (x->right) {
			t = x->right;
			t->parent = nullptr;
		} else {
			t = nullptr;
		}
		s = x;
		s->right = nullptr;
		x = nullptr;
	}

	Symbol* minimum(Symbol *node) {
		while (node->left != nullptr) {
			node = node->left;
		}
		return node;
	}

	Symbol* maximum(Symbol *node) {
		while (node->right != nullptr) {
			node = node->right;
		}
		return node;
	}

	Symbol* lookupRecursive(Symbol *node, string id, int &compareNum) {
		if (node == nullptr) {
			return node;
		}

		if (id.compare(node->id) == 0) {
			compareNum++;
			return node;
		}

		if (id.compare(node->id) < 0) {
			compareNum++;
			return lookupRecursive(node->left, id, compareNum);
		}
		compareNum++;
		return lookupRecursive(node->right, id, compareNum);
	}

	void preOrderTraversal(Symbol *node) {
		if (node != nullptr) {
			cout << node->id << endl;
			preOrderTraversal(node->left);
			preOrderTraversal(node->right);
		} 
	}

	void deleteRecursive(Symbol *node, string id) {
		Symbol *x = nullptr;
		Symbol *t, *s;
		while (node != nullptr){
			if (node->id.compare(id) == 0) {
				x = node;
			}

			if (node->id.compare(id) <= 0) {
				node = node->right;
			} else {
				node = node->left;
			}
		}

		// Couldn't find key in the tree
		if (x == nullptr) {
			return;
		}
		split(x, s, t); // split the tree
		if (s->left){ // remove x
			s->left->parent = nullptr;
		}
		root = join(s->left, t);
		delete(s);
		s = nullptr;
	}

	void deleteNode(string id) {
		deleteRecursive(root, id);
	}

	void deleteInPreOrder(Symbol *node) {
		if (node != nullptr) {
			cout << node->id << endl;
			preOrderTraversal(node->left);
			preOrderTraversal(node->right);
			if (node->scopeLevel == currentScope && node->scopeLevel != 0) {
				deleteNode(node->id);
			}
		} 
	}
    
	inline bool stringToBool(string str) {
		return true ? str == "true" : false;
	}
	inline string boolToString(bool b)
	{
		return b ? "true" : "false";
	}
	inline void insert(string id, string *paraList, int sizeOfParaList, string retType, bool isStatic, bool isFunction);
	inline void assign(string id, string value, string *paraList, int sizeOfParaList, bool isFunction);
	inline void begin();
	inline void end();
	inline void lookup(string id);
	inline void print();
	inline void run(string filename);
};
#endif