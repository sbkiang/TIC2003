#include "Container.h"

// constructor
Container::Container() {
	_statementCount = 0;
}

// destructor
Container::~Container() {}

CFG* Container::linkStatements() {
	if (_statements.size() == 0) { return nullptr; }
	CFGNode* head = new CFGNode;
	head->_stmtPtr = _statements.at(0);
	CFG* cfg = new CFG(head);
	for (int i = 1; i < _statements.size(); i++) {
		CFGNode* node = new CFGNode;
		node->_stmtPtr = _statements.at(i);
		cfg->addSTailSJump(node);
	}
	if (_type == "while") { // if is a while container, the last node will need to link with the first node
		cfg->_fTail->_sJump = cfg->_head;
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