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
		cout << "\t\tStmt start : " << container->_startStmtNum << "\tStmt end: " << container->_endStmtNum;
		cout << endl;
		_childContainers.at(i)->printContainerTree(nestedLevel + 1);
	}
}

vector<Container*> Container::getAllContainers() {
	vector<Container*> containers;
	_getAllContainers(this, containers);
	return containers;
}

void Container::_getAllContainers(Container* container, vector<Container*> &containers) {
	if (!container) { return; }
	if ((container->_type != "procedure" && container->_type != "else")) { // don't add procedure and else container to the result
		containers.push_back(container);
	}
	for (int i = 0; i < container->_childContainers.size(); i++) {
		if (container->_childContainers.at(i)->_type == "if") { // if childContainer is "if" container
			container->_childContainers.at(i)->_adjustedEndStmtNum = container->_childContainers.at(i + 1)->_adjustedEndStmtNum;
			container->_childContainers.at(i)->_endStmtNum = container->_childContainers.at(i + 1)->_endStmtNum;
		}
		_getAllContainers(container->_childContainers.at(i), containers);
	}
}

/*
void Container::printStmt() {
	for (int i = 0; i < _statements.size(); i++) {
		cout << _statements.at(i)->_stmtNum << " : " << _statements.at(i)->_stmt << endl;
	}
}
*/