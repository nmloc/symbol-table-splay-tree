#include "SymbolTable.h"

void SymbolTable::insert(string id, string *paraList, string retType, bool isStatic) {
	Symbol *node = new Symbol();
    node->parent = nullptr;
    node->left = nullptr;
    node->right = nullptr;
    node->id = id;

	if (paraList) {
		for(int i = 0; i<(sizeof(paraList)/sizeof(paraList[0])); i++) {
            node->paraList[i] = paraList[i];
        }
	}

	if (isStatic) node->scopeLevel = 0;
	else node->scopeLevel = this->currentScope;

    Symbol *y = nullptr;
    Symbol *x = this->root;

    while (x != nullptr) {
        y = x;
        if (node->id.compare(x->id) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    // y is parent of x
    node->parent = y;
    if (y == nullptr) {
        root = node;
    } else if (node->id.compare(y->id) < 0) {
        y->left = node;
    } else {
        y->right = node;
    }

    // splay the node
    splay(node);
}

void SymbolTable::assign(string id, string value) {
    regex rNum("[0-9]+");
	regex rString("['][ a-zA-Z0-9]*[']");
	regex rId("[a-z][_a-zA-Z0-9]*");
	string error = "ASSIGN " + id + " " + value;
}

void SymbolTable::begin() {
    this->currentScope++;
}

void SymbolTable::end() {
    this->currentScope--;
}

void SymbolTable::lookup(string id) {
	Symbol *x = lookupRecursive(this->root, id);
    if (x) {
		splay(x);
	}
	cout << x->scopeLevel << endl;
}

void SymbolTable::print() {
    preOrderTraversal(root);
}

void SymbolTable::check(string cmd, string para1, string para2) {
    if (cmd == "INSERT")
		insert(para1, para2);
	else if (cmd == "ASSIGN")
		assign(para1, para2);
	else if (cmd == "BEGIN")
		begin();
	else if (cmd == "END")
		end();
	else if (cmd == "LOOKUP")
		lookup(para1);
	else if (cmd == "PRINT")
		print();
	else {
		string error = cmd + " " + para1 + " " + para2;
		throw InvalidInstruction(error);
	}
}

void SymbolTable::run(string filename)
{
    ifstream openFile(filename);
	if (openFile.is_open())
	{
		string buffer;
		while (getline(openFile, buffer)) {
			string inputString[4] = {"", "", "", ""};
			int beginPos = 0, currPos = 0, numOfChar = 0, paramNum = 0, strLen = buffer.length();
			while (true) {
				if (currPos >= strLen) {
					inputString[paramNum++] = buffer.substr(beginPos, numOfChar);
					break;
				}
				if (buffer[currPos] == ' ') {
					inputString[paramNum++] = buffer.substr(beginPos, numOfChar);
					beginPos = currPos + 1;
					numOfChar = 0;
				}
				else {
					numOfChar++;
				}
				currPos++;
			}
			if (inputString[0] != "")	{
				check(inputString[0], inputString[1], inputString[2]);
			}
		}
		if (currentScope > 0)
			throw UnclosedBlock(currentScope);
		openFile.close();
	}
}