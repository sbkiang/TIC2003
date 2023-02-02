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
	void addSTailSJump(CFGNode* CFGNode);
	void addSTailFJump(CFGNode* CFGNode);
	void addFTailSJump(CFGNode* CFGNode);
	void addFTailFJump(CFGNode* CFGNode);
};