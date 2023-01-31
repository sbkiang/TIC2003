#pragma once
#include <string>

using namespace std;

struct Statement {
	string _stmt;
	int _stmtNum;
	Statement(int stmtNum);
	Statement(int stmtNum, string stmt);
};

struct CFGNode {
	Statement* _stmtPtr = NULL;
	CFGNode* _sJump = NULL; // For non-condition statement, _sJump points to next statement. For condition statement, _sJump points to first statement in the success condition block
	CFGNode* _fJump = NULL; //For non-condition statement, _fJump points to NULL. For condition statement, _fJump points to first statement in the fail condition block
};