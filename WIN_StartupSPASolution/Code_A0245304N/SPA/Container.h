#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "IfElseLinker.h"
#include "Struct.h"
#include "CFG.h"

using namespace std;

class Container {
public:
	// Constructor
	Container();

	// Destructor
	~Container();

	CFG* linkStatements();

	// method for processing the source program
	//virtual vector<string> getVectorStatements();
	//virtual int getStatementCount() = 0;
	vector<Statement*> _statements;
	vector<Container*> _childContainers;
	string _condition;
	string _type;
	int _statementCount;
	vector<IfElseLinker*> _ifElseLinker;

	IfElseLinker* getIfElseLinker(Container* ptr);
};