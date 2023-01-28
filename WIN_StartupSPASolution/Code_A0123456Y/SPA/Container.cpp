#include "Container.h"

// constructor
Container::Container() {
	_statementCount = 0;
}

// destructor
Container::~Container() {}

CFGNode* Container::linkStatements() {
	CFGNode* head = new CFGNode;
	head->_stmtPtr = _statements.at(0);
	for (int i = 1; i < _statements.size(); i++) {
		CFGNode* traverse = head;
		CFGNode* node = new CFGNode;
		node->_stmtPtr = _statements.at(i);
		if (!(_statements.at(i)->_altCondition)) { // if statement reside in the success condition block
			while (traverse->_sJump) { traverse = traverse->_sJump; } // traverse node until the last node
			traverse->_sJump = node; // add current statement to the linked list
			continue;
		}
		if (_statements.at(i)->_altCondition) { // statement reside in the fail condition block. Only applicable for if-else
			while (traverse->_fJump) { traverse = traverse->_fJump; } // traverse node until last node for "else" block
			traverse->_fJump = node; // add current statement to the linked list
			continue;
		}
	}
	return head;
}

Statement::Statement(int stmtNum, bool inAlt) {
	_stmtNum = stmtNum;
	_altCondition = inAlt;
}