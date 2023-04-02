#pragma once

#include "Container.h"
#include "CFG.h"
#include "Struct.h"
#include <stack>
#include <map>

class CFGBuilder {
public:
	//static CFG* BuildCFG(Container* procedure);
	static CFG BuildCFG(Container* procedure);

private:
	static void _CreateStmtMap(Container* container, map<int, CFGNode*> &stmtMap);
	static CFGNode* _FindNextStmt(stack<Container*> parentStack, int startStmtNum, map<int, CFGNode*> stmts);
	static void _PrintStmt(map<int, CFGNode*> stmts);
};