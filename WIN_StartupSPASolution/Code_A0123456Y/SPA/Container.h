#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "IfElseLinker.h"
#include "Struct.h"

using namespace std;

class CFG {
public:
	CFGNode* _head;
	CFGNode* _sTail;
	CFGNode* _fTail;

	CFG(CFGNode* head);
	~CFG();

	CFGNode* getNode(int stmtNum);
	CFGNode* _getNode(int stmtNum, CFGNode* CFGNode);
	void addSTailSJump(CFGNode* CFGNode);
	void addSTailFJump(CFGNode* CFGNode);
	void addFTailSJump(CFGNode* CFGNode);
	void addFTailFJump(CFGNode* CFGNode);
};

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