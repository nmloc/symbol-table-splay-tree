#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Symbol {

};

class Table {
    
};

class SymbolTable
{
public:
    SymbolTable() {}
    inline void check(string cmd, string param1, string param2);
	inline void insert(string id, string type, bool isStatic);
	inline void assign(string id, string value);
	inline void begin();
	inline void end();
	inline void lookup(string id);
	inline void print();
	inline void run(string filename);
};
#endif