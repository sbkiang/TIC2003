#include "Container.h"

// constructor
Container::Container() {
}

// destructor
Container::~Container() {}

CFG* Container::linkStatements() {
	CFG* cfg = new CFG();
	CFGNode* head = new CFGNode;
	if (_statements.size() == 0) {
		return cfg;
	}
	head->_stmtPtr = _statements.at(0);
	head->_container = _type;
	cfg->_head = head;
	for (int i = 1; i < _statements.size(); i++) {
		CFGNode* node = new CFGNode;
		node->_stmtPtr = _statements.at(i);
		node->_container = _type;
		cfg->addSTailSJump(node);
	}
	if (_type == "while") { // if is a while container, the last node will need to link with the first node
		if (cfg->_sTail) { cfg->_sTail->_sJump = cfg->_head; } // check if _sTail is null. This might happen if we get a while container without any statements in it
		//else { cfg->_sTail = cfg->_head; } // if no _sTail = no statements, the tail is the head
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

void Container::printContainerTree(int tabs) {
	for (int i = 0; i < _childContainers.size(); i++) {
		Container* container = _childContainers.at(i);
		for (int i = 0; i < tabs; i++) {
			cout << "\t"; // tab for formatting
		}
		if (container->_type == "else") {
			cout << "else container for : " << getIfElseLinker(container)->_ifPtr->_statements.at(0)->_stmt;
		}
		else {
			cout << container->_type << " container : " << container->_statements.at(0)->_stmt;
		}
		cout << endl;
		_childContainers.at(i)->printContainerTree(tabs + 1);
	}
}

void Container::printStmt() {
	for (int i = 0; i < _statements.size(); i++) {
		cout << _statements.at(i)->_stmtNum << " : " << _statements.at(i)->_stmt << endl;
	}
}