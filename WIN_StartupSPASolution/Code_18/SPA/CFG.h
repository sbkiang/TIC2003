#pragma once
#include "Struct.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <set>

struct CFGNode;

class CFG {
public:
	CFGNode* _head;

	CFG(CFGNode* head);
	CFG();
	~CFG();

	CFGNode* getNode(int stmtNum);
	CFGNode* _getNode(int stmtNum, CFGNode* CFGNode, set<int> visitsed);
	void printCFG();
	void _printCFG(CFGNode* node, set<int> visited, int spaces);
};