#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Struct.h"
#include "CFG.h"
#include <iostream>
#include <assert.h>

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
	int _level;
	int _startStmtNum;
	int _endStmtNum;

	//CFG* linkStatements();  // Create a CFGNode for each statement. Then, link them together in a CFG, and following the if-else and while CFG rule
	void printContainerTree(int tab);
	void printStmt();
};