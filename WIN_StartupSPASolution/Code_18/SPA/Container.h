#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "IfElseLinker.h"
#include "Struct.h"
#include "CFG.h"
#include <iostream>

using namespace std;

class Container {
public:
	// Constructor
	Container();

	// Destructor
	~Container();

	vector<Statement*> _statements;
	vector<Container*> _childContainers;
	string _condition;
	string _type;
	int _statementCount;
	vector<IfElseLinker*> _ifElseLinker;

	CFG* linkStatements();  // Create a CFGNode for each statement. Then, link them together in a CFG, and following the if-else and while CFG rule
	IfElseLinker* getIfElseLinker(Container* ptr);
	void printContainerTree(int tab);
	void printStmt();
};