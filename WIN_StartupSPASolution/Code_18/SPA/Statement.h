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

	string _entity = "";
	bool _containerHead = false;
	bool _containerTail = false;
	Container* _container = nullptr;

	void setEntity(string entity);
	string getEntity();
	int getAdjustedStmtNum();
	int getStmtNum();
	void appendStmt(string s);
	string getStmt();
	int getLevel();

private:
	string _stmt = "";
	int _stmtNum = 0;
	int _level = 0;
	int _stmtNumSubtract = 0;
};