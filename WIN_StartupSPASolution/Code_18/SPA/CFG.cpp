#include "CFG.h"

CFG::CFG(CFGNode* head) {
	_head = head;
	_sTail = nullptr;
	_fTail = nullptr;
}

CFG::~CFG() {}

CFGNode* CFG::getNode(int stmtNum) {
	return _getNode(stmtNum, _head);
}

CFGNode* CFG::_getNode(int stmtNum, CFGNode* node) {
	CFGNode* found = nullptr;
	if (node) {
		if (node->_stmtPtr->_stmtNum == stmtNum) {
			return node;
		}
		CFGNode* found = _getNode(stmtNum, node->_sJump);
		if (!found) {
			found = _getNode(stmtNum, node->_fJump);
		}
	}
	return found;
}

void CFG::addFTailSJump(CFGNode* node) { 
	if (!_fTail) { _head->_sJump = node; } // we want to maintain _fTail as NULL if there's no statement for it instead of setting _head as the tail
	else { _fTail->_sJump = node; } // if _fTail is not NULL, add the new CFG node to it
	_fTail = node;
}

void CFG::addFTailFJump(CFGNode* node) { 
	if (!_fTail) { _head->_fJump = node; } // we want to maintain _fTail as NULL if there's no statement for it
	else { _fTail->_fJump = node; }
	_fTail = node;
}

void CFG::addSTailSJump(CFGNode* node) { 
	if (!_sTail) { _head->_sJump = node; } // we want to maintain _sTail as NULL if there's no statement for it
	else { _sTail->_sJump = node; } // if _sTail is not NULL, add the new CFG node to it
	_sTail = node;
}

void CFG::addSTailFJump(CFGNode* node) {  
	if (!_sTail) { _head->_fJump = node; } // we want to maintain _sTail as NULL if there's no statement for it
	else { _sTail->_fJump = node; } // if _sTail is not NULL, add the new CFG node to it
	_sTail = node;
}