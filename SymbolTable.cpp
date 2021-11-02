#include "SymbolTable.h"

void SymbolTable::insert(string id, string *paraList, int sizeOfParaList, string retType, bool isStatic, bool isFunction) {
	// Check Invalid Declaration
	if (isFunction && currentScope != 0) {
		string subError;
		if (paraList != nullptr) {
			subError = '(' + paraList[0];
			for (int i = 1; i < sizeOfParaList; i++) {
				subError = subError + ',' + paraList[i];
			}
			subError += ")->";
		}

		string error = "INSERT " + id + " " + subError + retType + " " + boolToString(isStatic);
		throw InvalidDeclaration(error);
	}

	// Check Redeclare
	if (root) {
		int compareNum_forCheck = 0;
		Symbol *foundID = lookupRecursive(root, id, compareNum_forCheck);
		if (foundID) {
			if (foundID->id == id && foundID->scopeLevel == currentScope) {
				string subError;
				if (paraList != nullptr) {
					subError = '(' + paraList[0];
					for (int i = 1; i < sizeOfParaList; i++) {
						subError = subError + ',' + paraList[i];
					}
					subError += ")->";
				}

				string error = "INSERT " + id + " " + subError + retType + " " + boolToString(isStatic);
				throw Redeclared(error);
			}
		}
	}
	
	int compareNum = 0;
	int splayNum = 0;
	Symbol *node = new Symbol();
    node->id = id;
	node->isFunction = isFunction;
	if (sizeOfParaList > 1) {
		for(int i = 0; i < sizeOfParaList; i++) {
            node->paraList[i] = paraList[i];
        }
	}

	node->retType = retType;

	if (isStatic) node->scopeLevel = 0;
	else node->scopeLevel = this->currentScope;

	node->parent = nullptr;
    node->left = nullptr;
    node->right = nullptr;

    Symbol *y = nullptr;
    Symbol *x = this->root;
	// find appropriate position for node
    while (x != nullptr) {
        y = x;
		if (node->scopeLevel < x->scopeLevel) {
			x = x->left;
			compareNum++;
		}
		else if (node->scopeLevel > x->scopeLevel) {
			x = x->right;
			compareNum++;
		}
		else {
			if (node->id.compare(x->id) < 0) {
				x = x->right;
			} else {
				x = x->left;
			}
			compareNum++;
		}
    }
	
    // choose left or right of parent to add node
	if (y == nullptr) {
      root = node;
    }
    else if(node->scopeLevel < y->scopeLevel){
		y->left = node;
    }
    else if(node->scopeLevel > y->scopeLevel){
		y->right = node;
    }
    else
    {
		if(node->id.compare(y->id) < 0) {
			//y->left = node;
			y->right = node;
		} else {
			//y->right = node;
			y->left = node;
		}
    }


    // splay the node
	if (node != root) {
		splay(node);
		splayNum++;
	}

	// reply from INSERT cmd
	cout << compareNum << " " << splayNum << endl;
}

void SymbolTable::assign(string id, string value, string *paraList, int sizeOfParaList, bool isFunction) {
    regex rNum("[0-9]+");
	regex rString("['][ a-zA-Z0-9]*[']");
	regex rId("[a-z][_a-zA-Z0-9]*");

	string subError = "";
	if (paraList != nullptr) {
		subError = '(' + paraList[0];
		for (int i = 1; i < sizeOfParaList; i++) {
			subError = subError + ',' + paraList[i];
		}
		subError += ')';
	}
	string error = "ASSIGN " + id + " " + value + subError;

	int compareNum_id = 0;
	Symbol *foundID = lookupRecursive(root, id, compareNum_id);
	if (regex_match(value, rNum)) {
		if (foundID == nullptr) throw Undeclared(error);
		else {
			int splayNum_id = 0;
			splay(foundID);
			splayNum_id++;
			if (foundID->retType == "number")
				cout << compareNum_id << " " << splayNum_id << endl;
			else throw TypeMismatch(error);
		}
	}
	else if (regex_match(value, rString)) {
		if (foundID == nullptr) throw Undeclared(error);
		else {
			int splayNum_id = 0;
			splay(foundID);
			splayNum_id++;
			if (foundID->retType == "string") 
				cout << compareNum_id << " " << splayNum_id << endl;
			else throw TypeMismatch(error);
		}
	}
	else if (regex_match(value, rId) && isFunction == false) {
		if (foundID == nullptr) throw Undeclared(error);
		else {
			int splayNum_id = 0;
			splay(foundID);
			splayNum_id++;
			int compareNum_value = 0;
			Symbol *foundValue = lookupRecursive(root, value, compareNum_value);
			if (foundValue) {
				if (foundValue->retType == foundID->retType) {
					cout << compareNum_id + compareNum_value << " " << splayNum_id << endl;
				}
				else throw TypeMismatch(error);
			}
			else throw Undeclared(error);
		}
	}
	else if (regex_match(value, rId) && isFunction == true) {
		int compareNum_value = 0;
		Symbol *foundValue = lookupRecursive(root, value, compareNum_value);
		if (foundValue) {
			for (int i = 0; i < sizeOfParaList; i++) {
				if (regex_match(paraList[i], rNum) && foundValue->paraList[i] == "number")
					continue;
				else if (regex_match(paraList[i], rString) && foundValue->paraList[i] == "string")
					continue;
				else throw TypeMismatch(error);
			}
			if (foundID == nullptr) throw Undeclared(error);
			else if (foundValue->retType == foundID->retType) {
				int splayNum_id = 0;
				splay(foundID);
				splayNum_id++;
				cout << compareNum_id + compareNum_value << " " << splayNum_id << endl;
			}
			else throw TypeMismatch(error);
		}
		else throw Undeclared(error);
	}
	else throw TypeMismatch(error);
}
	
void SymbolTable::begin() {
    this->currentScope++;

}

void SymbolTable::end() {
	if (currentScope <= 0) {
		throw UnknownBlock();
	}
	else {
		deleteInPreOrder(root);
		this->currentScope--;
	}
}

void SymbolTable::lookup(string id) {
	int compareNum = 0;
	Symbol *foundID = lookupRecursive(this->root, id, compareNum);
    if (foundID) {
		splay(foundID);
		cout << foundID->scopeLevel << endl;
	}
	else {
		string error = "LOOKUP " + id;
		throw Undeclared(error);
	}
}

void SymbolTable::print() {
    preOrderTraversal(root);
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
			int length = inputString[2].length();
			// INSERT cmd
			if (inputString[0] == "INSERT") {
				int eleNum_funcType = 1;
				// INSERT function type
				if (inputString[2][0] == '(' && inputString[2][1] != ')') {
					eleNum_funcType++;
					// split value of function type
					for (int i = 0; i < length; i++) {
						if (inputString[2][i] == ',') eleNum_funcType++;
					}
					string funcType[10];
					int i = 0;
					for (int j = 0; j < length; j++) {
						if (inputString[2][j] == '(' || inputString[2][j] == ')' || inputString[2][j] == '-')
							continue;
						else if (inputString[2][j] == ',' || inputString[2][j] == '>') {
							i++;
							continue;
						}
						else funcType[i] += inputString[2][j];
					}
					string args_funcType[eleNum_funcType - 1];
					string retType = funcType[eleNum_funcType - 1];
					for (int i = 0; i < eleNum_funcType - 1; i++) {
						args_funcType[i] = funcType[i];
					}
					if (inputString[2][0] == '(' && inputString[2][1] != ')') {
					// INSERT function type with at least 1 para
						insert(inputString[1], args_funcType, eleNum_funcType - 1, retType, stringToBool(inputString[3]), true);
					}
					else
					// INSERT function type with no para
						insert(inputString[1], nullptr, 0, funcType[0], stringToBool(inputString[3]), true);
				}
				// INSERT with type "string" or "number"
				else insert(inputString[1], nullptr, 0, inputString[2], stringToBool(inputString[3]), false);
			}
			// ASSIGN cmd
			else if (inputString[0] == "ASSIGN") {
				// split value of function call
				if (inputString[2][length - 1] == ')') {
					string name_funcCall;
					string para_funcCall[10];
					int dataPos = 0;
					int commaCount = 0;
					while (inputString[2][dataPos] != '(') {
						name_funcCall += inputString[2][dataPos];
						dataPos++;
					}
					// ASSIGN function call with at lease 1 para
					if (inputString[2][dataPos + 1] != ')') {
						while (inputString[2][dataPos + 1] != ')') {
							if (inputString[2][dataPos + 1] == ',') {
								commaCount++;
								dataPos++;
								continue;
							}
							para_funcCall[commaCount] += inputString[2][dataPos + 1];
							dataPos++;
						}
						assign(inputString[1], name_funcCall, para_funcCall, commaCount + 1, true);
					}
					// ASSIGN function call with no para
					else assign(inputString[1], name_funcCall, nullptr, 0, true);
				}
				// ASSIGN with type "string" or "number"
				else assign(inputString[1], inputString[2], nullptr, 0, false);
			}
			else if (inputString[0] == "BEGIN")
				begin();
			else if (inputString[0] == "END")
				end();
			else if (inputString[0] == "LOOKUP")
				lookup(inputString[1]);
			else if (inputString[0] == "PRINT")
				print();
			else {
				string error = inputString[0] + " " + inputString[1] + " " + inputString[2] + " " + inputString[3];
				throw InvalidInstruction(error);
			}
		}
		if (currentScope > 0)
			throw UnclosedBlock(currentScope);
		inputFile.close();
	}
}