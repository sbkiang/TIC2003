#pragma once
#include "Struct.h"

class CFG {
public:
	CFGNode* _head;
	CFGNode* _sTail;
	CFGNode* _fTail;

	CFG(CFGNode* head);
	~CFG();

	CFGNode* getNode(int stmtNum);
	CFGNode* _getNode(int stmtNum, CFGNode* CFGNode);
	void addSTailSJump(CFGNode* CFGNode); // add node to success jump of CFG's success condition's tail
	void addSTailFJump(CFGNode* CFGNode); // add node to fail jump of CFG's success condition's tail
	void addFTailSJump(CFGNode* CFGNode); // add node to success jump of CFG's fail condition's tail
	void addFTailFJump(CFGNode* CFGNode); // add node to fail jump of CFG's fail condition's tail
};