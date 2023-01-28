#pragma once
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

struct Statement{
	string _stmt;
	int _stmtNum;
	bool _altCondition;
	Statement(int stmtNum, bool inAlt);
};

struct CFGNode {
	Statement* _stmtPtr = NULL;
	CFGNode* _sJump = NULL;
	CFGNode* _fJump = NULL;
};


class Container {
public:
	// Constructor
	Container();

	// Destructor
	~Container();

	CFGNode* linkStatements();

	// method for processing the source program
	//virtual vector<string> getVectorStatements();
	//virtual int getStatementCount() = 0;
	vector<Statement*> _statements;
	vector<Container*> _childContainers;
	string _condition;
	string _type;
	int _statementCount;
};