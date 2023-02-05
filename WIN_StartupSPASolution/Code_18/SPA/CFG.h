#pragma once
#include "Struct.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <set>

class CFG {
public:
	CFGNode* _head;
	CFGNode* _sTail;
	CFGNode* _fTail;

	CFG(CFGNode* head);
	CFG();
	~CFG();

	CFGNode* getNode(int stmtNum);
	CFGNode* _getNode(int stmtNum, CFGNode* CFGNode, set<int> visitsed);
	void addSTailSJump(CFGNode* CFGNode); // add node to success jump of CFG's success condition's tail
	void addSTailFJump(CFGNode* CFGNode); // add node to fail jump of CFG's success condition's tail
	void addFTailSJump(CFGNode* CFGNode); // add node to success jump of CFG's fail condition's tail
	void addFTailFJump(CFGNode* CFGNode); // add node to fail jump of CFG's fail condition's tail
	CFGNode* getSTail();
	CFGNode* _getSTail(CFGNode* node, set<int> visited);
	CFGNode* getFTail();
	CFGNode* _getFTail(CFGNode* node, set<int> visited);
	void printCFG();
	void _printCFG(CFGNode* node, set<int> visited, int spaces);
};