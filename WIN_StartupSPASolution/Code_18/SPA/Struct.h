#pragma once
#include <string>
#include "Container.h"

using namespace std;

struct Statement {
	string _stmt;
	int _stmtNum = 0;
	int _level = 0;
	int _stmtNumSubtract = 0;
	bool _containerHead = false;
	bool _containerTail = false;
	Container* _container = nullptr;
	Statement(int stmtNum, string stmt, int stmtNumSubtract);
	Statement(int stmtNum, string stmt, Container* container, int stmtNumSubtract);
	Statement(int stmtNum, int level, Container* container, int stmtNumSubtract);
	Statement(int stmtNum, string stmt, int level, Container* container, int stmtNumSubtract);
	int getAdjustedStmtNum();
};

struct CFGNode {
	Statement* _stmtPtr = NULL;
	CFGNode* _sJump = NULL; // For non-condition statement, _sJump points to next statement. For condition statement, _sJump points to first statement in the success condition block
	CFGNode* _fJump = NULL; //For non-condition statement, _fJump points to NULL. For condition statement, _fJump points to first statement in the fail condition block
};