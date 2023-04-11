#pragma once
#include <string>
#include <vector>

using namespace std;

class Container;

class Statement{
private:
	string _stmt;
	string _entity;
	vector<string> _tokens;
	int _stmtNum = 0;
	int _level = 0;
	int _stmtNumSubtract = 0;

public:
	// Default Constructor
	Statement(int stmtNum, string stmt, int stmtNumSubtract);
	Statement(int stmtNum, string stmt, Container* container, int stmtNumSubtract);
	Statement(int stmtNum, int level, Container* container, int stmtNumSubtract);
	Statement(int stmtNum, string stmt, int level, Container* container, int stmtNumSubtract);

	bool _containerHead = false;
	bool _containerTail = false;
	Container* _container = nullptr;

	void AddToken(string tk);
	vector<string> GetToken();
	void SetEntity(string entity);
	string GetEntity();
	int GetAdjustedStmtNum();
	int GetStmtNum();
	void AddStmt(string s);
	string GetStmt();
	int GetLevel();
};