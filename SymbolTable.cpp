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

void SymbolTable::check(string cmd, string id, string *paraList, string para, string isStatic) { // para is retType or value
    if (cmd == "INSERT")
		insert(id, paraList, para, stringToBool(isStatic));
	else if (cmd == "ASSIGN")
		assign(id, para);
	else if (cmd == "BEGIN")
		begin();
	else if (cmd == "END")
		end();
	else if (cmd == "LOOKUP")
		lookup(id);
	else if (cmd == "PRINT")
		print();
	else {
		string subError;

		if (paraList != NULL) {
			subError = '(' + paraList[0] + ',';
			for (int i = 1; i < (sizeof(paraList) / sizeof(paraList[0])); i++) {
				subError = subError + ',' + paraList[i];
			}
			subError += ")-> ";
		}

		string error = cmd + " " + id + " " + subError + para + " " + isStatic;
		throw InvalidInstruction(error);
	}
}

void SymbolTable::run(string filename)
{
    ifstream inputFile(filename);
	if (inputFile.is_open())
	{
		string line;
		while (getline(inputFile, line)) {
			string inputString[4] = {"", "", "", ""};
			int beginPos = 0, currPos = 0, numOfChar = 0, paramNum = 0, strLen = line.length();
			while (true) {
				if (currPos >= strLen) {
					inputString[paramNum++] = line.substr(beginPos, numOfChar);
					break;
				}
				if (line[currPos] == ' ') {
					inputString[paramNum++] = line.substr(beginPos, numOfChar);
					beginPos = currPos + 1;
					numOfChar = 0;
				}
				else {
					numOfChar++;
				}
				currPos++;
			}

			int paraNumInFuncType;
			if (inputString[2][0] == ' ') {
				paraNumInFuncType = 0;
			}
			else {
				paraNumInFuncType = 1;
			}
			for (int i = 0; i < inputString[2].length(); i++) {
				if (inputString[2][i] == ',') paraNumInFuncType++;
			}
			string funcType[paraNumInFuncType];
			int i = 0;
			for (int j = 0; j < inputString[2].length(); j++) {
				if (inputString[2][i] == '(' 
				|| inputString[2][i] == ')'
				|| inputString[2][i] == '-') {
					continue;
				}
				else if (inputString[2][i] == ',' 
				|| inputString[2][i] == '>') {
					i++;
					continue;
				}
				else funcType[i] += inputString[j];
			}
			if (inputString[0] != "")	{
				check(inputString[0], inputString[1], funcType, funcType[paraNumInFuncType - 1], inputString[3]);
			}
		}
		if (currentScope > 0)
			throw UnclosedBlock(currentScope);
		inputFile.close();
	}
}