#include "Container.h"

// constructor
Container::Container() {
	_statementCount = 0;
}

// destructor
Container::~Container() {}

CFG* Container::linkStatements() { // Create a CFGNode for each statement. Then, link them together in a CFG, and following the if-else and while CFG rule
	if (_statements.size() == 0) { return nullptr; }
	CFGNode* head = new CFGNode;
	head->_stmtPtr = _statements.at(0);
	CFG* cfg = new CFG(head);
	for (int i = 1; i < _statements.size(); i++) {
		CFGNode* node = new CFGNode;
		node->_stmtPtr = _statements.at(i);
		cfg->addSTailSJump(node);
		/*
		if (_statements.at(i)->_failCondition) { // statement reside in the fail condition block. Only applicable for if-else
			cfg->addFTailSJump(node);
			continue;
		}
		*/
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

IfElseLinker* Container::getIfElseLinker(Container* ptr) {
	for (int i = 0; i < _ifElseLinker.size(); i++) {
		if (_ifElseLinker.at(i)->_ifPtr == ptr) { return _ifElseLinker.at(i); }
		if (_ifElseLinker.at(i)->_elsePtr == ptr) { return _ifElseLinker.at(i); }
	}
	return nullptr;
}