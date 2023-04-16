#pragma once

#include "Container.h"
#include "CFG.h"
#include "Struct.h"
#include <stack>
#include <map>
#include "IContainer.h"

class CFGBuilder {
public:
	//static CFG* BuildCFG(Container* procedure);
	static CFG BuildCFG(IContainer* procedure);

private:
	static void _CreateStmtMap(IContainer* container, map<int, CFGNode*> &stmtMap);
	static CFGNode* _FindNextStmt(stack<IContainer*> parentStack, int startStmtNum, map<int, CFGNode*> stmts);
	static void _PrintStmt(map<int, CFGNode*> stmts);
};