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
	static void _createStmtMap(Container* container, map<int, CFGNode*> &stmtMap);
	static CFGNode* _findNextStmt(stack<Container*> parentStack, int startStmtNum, map<int, CFGNode*> stmts);
	static void _printStmt(map<int, CFGNode*> stmts);
};