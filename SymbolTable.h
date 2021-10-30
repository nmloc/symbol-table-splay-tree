#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Symbol {
public:
    string id;
    string paraList[10];
    string retType;
    int scopeLevel;
    Symbol *parent, *left, *right;
};

class SymbolTable
{
public:
    Symbol *root;
    int currentScope = 0;

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

	Symbol* lookupRecursive(Symbol *node, string id) {
		if (node == nullptr || id.compare(node->id) == 0) {
			return node;
		}

		if (id.compare(node->id) < 0) {
			return lookupRecursive(node->left, id);
		} 
		return lookupRecursive(node->right, id);
	}

	void preOrderTraversal(Symbol *node) {
		if (node != nullptr) {
			cout << node->id << endl;
			preOrderTraversal(node->left);
			preOrderTraversal(node->right);
		} 
	}

	void deleteNode(Symbol *node) {
		
	}
    
    SymbolTable() {}
	inline bool stringToBool(string str) {
		return true ? str == "true" : false;
	}
    inline void check(string cmd, string para1, string *paraList, string para, string isStatic);
	inline void insert(string id, string *paraList, string retType, bool isStatic);
	inline void assign(string id, string value);
	inline void begin();
	inline void end();
	inline void lookup(string id);
	inline void print();
	inline void run(string filename);
};
#endif