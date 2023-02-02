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

void CFG::addFTailSJump(CFGNode* node) { // add node to success jump of CFG's fail condition's tail
	if (!_fTail) { // we want to maintain _fTail as NULL if there's no statement for it
		_head->_sJump = node;
		return;
	}
	_fTail->_sJump = node;
	_fTail = node;
}

void CFG::addFTailFJump(CFGNode* node) { // add node to fail jump of CFG's fail condition's tail
	if (!_fTail) { // we want to maintain _fTail as NULL if there's no statement for it
		_head->_fJump = node;
		return;
	}
	_fTail->_fJump = node;
	_fTail = node;
}

void CFG::addSTailSJump(CFGNode* node) { // add node to success jump of CFG's success condition's tail
	if (!_sTail) { _head->_sJump = node; }
	else { _sTail->_sJump = node; }
	_sTail = node;
}

void CFG::addSTailFJump(CFGNode* node) {  // add node to fail jump of CFG's success condition's tail
	if (!_sTail) { // we want to maintain _sTail as NULL if there's no statement for it
		_head->_fJump = node;
		return;
	}
	_sTail->_fJump = node;
	_sTail = node;
}