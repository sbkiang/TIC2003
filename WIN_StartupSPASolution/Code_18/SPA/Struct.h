#pragma once
#include <string>
#include "Container.h"

using namespace std;

struct Statement {
	string _stmt;
	int _stmtNum;
	int _level;
	bool _containerHead;
	bool _containerTail;
	Container* _container;
	Statement(int stmtNum, string stmt);
	Statement(int stmtNum, string stmt, Container* container);
	Statement(int stmtNum, int level, Container* container);
	Statement(int stmtNum, string stmt, int level, Container* container);
	Statement(int stmtNum, int level, bool conditionHead, Container* container);
};

struct CFGNode {
	Statement* _stmtPtr = NULL;
	CFGNode* _sJump = NULL; // For non-condition statement, _sJump points to next statement. For condition statement, _sJump points to first statement in the success condition block
	CFGNode* _fJump = NULL; //For non-condition statement, _fJump points to NULL. For condition statement, _fJump points to first statement in the fail condition block
	string _container;
};