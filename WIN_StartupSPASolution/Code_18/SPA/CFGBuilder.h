#pragma once

#include "Container.h"
#include "CFG.h"
#include "Struct.h"
#include <stack>
#include <map>

class CFGBuilder {
public:
	static CFG* buildCFG(Container* procedure);

private:
	static map<int, CFGNode*> _joinStmts(Container* container);
	static CFGNode* _findNextStmt(stack<Container*> parentStack, int startStmtNum, map<int, CFGNode*> stmts);
	static void _printStmt(map<int, CFGNode*> stmts);
};