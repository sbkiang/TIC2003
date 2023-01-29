#include "Container.h"

CFG::CFG(CFGNode* head){
	_head = head;
	_sTail = NULL;
	_fTail = NULL;
}

CFG::~CFG() {}

CFGNode* CFG::getNode(int stmtNum) {
	return _getNode(stmtNum, _head);
}

CFGNode* CFG::_getNode(int stmtNum, CFGNode* node) {
	CFGNode* found = NULL;
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
	if (!_sTail) { // we want to maintain _sTail as NULL if there's no statement for it
		_head->_sJump = node;
		return;
	}
	_sTail->_sJump = node;
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

// constructor
Container::Container() {
	_statementCount = 0;
}

// destructor
Container::~Container() {}

CFG* Container::linkStatements() { // Create a CFGNode for each statement. Then, link them together in a CFG, and following the if-else and while CFG rule
	if (_statements.size() == 0) { return NULL; }
	CFGNode* head = new CFGNode;
	head->_stmtPtr = _statements.at(0);
	CFG* cfg = new CFG(head);
	for (int i = 1; i < _statements.size(); i++) {
		CFGNode* node = new CFGNode;
		node->_stmtPtr = _statements.at(i);
		if (!(_statements.at(i)->_failCondition)) { // if statement resides in the success condition block
			cfg->addSTailSJump(node);
			continue;
		}
		if (_statements.at(i)->_failCondition) { // statement reside in the fail condition block. Only applicable for if-else
			cfg->addFTailSJump(node);
			continue;
		}
		/*
		if (!(_statements.at(i)->_altCondition)) { // if statement reside in the success condition block
			while (traverse->_sJump) { traverse = traverse->_sJump; } // traverse node until the last node
			traverse->_sJump = node; // add current statement's CFGNode to the 
			continue;
		}
		if (_statements.at(i)->_altCondition) { // statement reside in the fail condition block. Only applicable for if-else
			while (traverse->_fJump) { traverse = traverse->_fJump; } // traverse node until last node for "else" block
			traverse->_fJump = node; // add current statement to the linked list
			continue;
		}
		*/
	}
	return cfg;
}

Statement::Statement(int stmtNum, bool inAlt) {
	_stmtNum = stmtNum;
	_failCondition = inAlt;
}