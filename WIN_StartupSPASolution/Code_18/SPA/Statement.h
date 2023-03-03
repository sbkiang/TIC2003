#pragma once
#include <string>

using namespace std;

class Container;

class Statement{
public:
	// Default Constructor
	Statement(int stmtNum, string stmt, int stmtNumSubtract);
	Statement(int stmtNum, string stmt, Container* container, int stmtNumSubtract);
	Statement(int stmtNum, int level, Container* container, int stmtNumSubtract);
	Statement(int stmtNum, string stmt, int level, Container* container, int stmtNumSubtract);

	// Default Destructor
	~Statement();

	int getAdjustedStmtNum();

	string _stmt;
	bool _containerHead = false;
	bool _containerTail = false;
	Container* _container = nullptr;

private:
	int _stmtNum = 0;
	int _level = 0;
	int _stmtNumSubtract = 0;
		
};