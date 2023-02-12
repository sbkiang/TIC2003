#include "Container.h"

// constructor
Container::Container() {}

// destructor
Container::~Container() {}

void Container::printContainerTree(int nestedLevel) {
	for (int i = 0; i < _childContainers.size(); i++) {
		Container* container = _childContainers.at(i);
		for (int i = 0; i < nestedLevel; i++) {
			cout << " "; // tab for formatting
		}
		if (container->_type == "else") {
			cout << "else container";
		}
		else {
			cout << container->_type << " container";
		}
		cout << "\t\tStmt start : " << container->_startStmtNum << "\tStmt end: " << container->_endStmtNum << "\t ElseCountSubtract: " << container->_elseCountSubtract;
		cout << endl;
		_childContainers.at(i)->printContainerTree(nestedLevel + 1);
	}
}
/*
void Container::printStmt() {
	for (int i = 0; i < _statements.size(); i++) {
		cout << _statements.at(i)->_stmtNum << " : " << _statements.at(i)->_stmt << endl;
	}
}
*/