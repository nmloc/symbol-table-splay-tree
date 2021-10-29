#include "SymbolTable.h"

void SymbolTable::insert(string id, string type, bool isStatic) {

}

void SymbolTable::assign(string id, string value) {
    
}

void SymbolTable::begin() {
    
}

void SymbolTable::end() {
    
}

void SymbolTable::lookup(string id) {
    
}

void SymbolTable::print() {
    
}

void SymbolTable::check(string cmd, string para1, string para2) {
    
}

void SymbolTable::run(string filename)
{
    ifstream openFile(filename);
	if (openFile.is_open())
	{
		string buffer;
		while (getline(openFile, buffer)) {
			string inputString[3] = {"", "", ""};
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
		if (scope > 0)
			throw UnclosedBlock(scope);
		openFile.close();
	}
}