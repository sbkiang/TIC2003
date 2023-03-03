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

	CFGNode* getNode(int stmtNum);
	vector<CFGNode*> getAllCFGNodes();
	CFGNode* _getNode(int stmtNum, CFGNode* CFGNode, set<int> visitsed);
	void _getAllCFGNodes(CFGNode* node, set<int> visited, int spaces, vector<CFGNode*> &nodes);
	
};