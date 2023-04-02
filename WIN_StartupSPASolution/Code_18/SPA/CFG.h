#pragma once
#include "Struct.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <set>
#include "Statement.h"

//struct CFGNode;

class CFG {
public:
	CFGNode* _head;

	CFG(CFGNode* head);
	CFG();
	~CFG();

	CFGNode* GetNode(int stmtNum);
	vector<CFGNode*> GetAllCFGNodes();
	CFGNode* _GetNode(int stmtNum, CFGNode* CFGNode, set<int> visitsed);
	void _GetAllCFGNodes(CFGNode* node, set<int> visited, int spaces, vector<CFGNode*> &nodes);
	
};