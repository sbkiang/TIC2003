#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "IfElseLinker.h"

using namespace std;

struct Statement {
	string _stmt;
	int _stmtNum;
	bool _failCondition;
	Statement(int stmtNum, bool inAlt);
};

struct CFGNode {
	Statement* _stmtPtr = NULL;
	CFGNode* _sJump = NULL; // For non-condition statement, _sJump points to next statement. For condition statement, _sJump points to success condition block
	CFGNode* _fJump = NULL; //For non-condition statement, _fJump points to NULL. For condition statement, _fJump points to fail condition block
};

/*
struct IfElseLinker {
	Container* _ifPtr;
	Container* _elsePtr;
	IfElseLinker(Container* ifPtr, Container* elsePtr);
};
*/

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