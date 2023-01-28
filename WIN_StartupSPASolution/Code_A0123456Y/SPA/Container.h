#pragma once
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

struct Statement{
	string _stmt;
	int _stmtNum;
	bool _altCondition;
	Statement(bool inAlt);
};


class Container {
public:
	// Constructor
	Container();

	// Destructor
	~Container();

	// method for processing the source program
	//virtual vector<string> getVectorStatements();
	//virtual int getStatementCount() = 0;
	vector<Statement*> _statements;
	vector<Container*> _childContainers;
	string _condition;
	string _type;
	int _statementCount;
};