#include "Container.h"

Container::Container() {}

void Container::PrintContainerTree(int nestedLevel) {
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
		_childContainers.at(i)->PrintContainerTree(nestedLevel + 1);
	}
}

vector<Container> Container::GetAllContainers() {
	vector<Container> containers;
	_GetAllContainers(*(this), containers);
	return containers;
}

void Container::_GetAllContainers(Container container, vector<Container> &containers) {
	//if (!container) { return; }
	/*
	if ((container._type != "procedure" && container._type != "else")) { // don't add procedure and else container to the result
		containers.push_back(container);
	}
	*/
	for (int i = 0; i < container._childContainers.size(); i++) {
		Container child = *(container._childContainers.at(i));
		if (child._type == "if") { // if childContainer is "if" container
			Container nextChild = *(container._childContainers.at(i + 1));
			child._adjustedEndStmtNum = nextChild._adjustedEndStmtNum;
			child._endStmtNum = nextChild._endStmtNum;
			containers.push_back(child);
		}
		else if (child._type == "while") {
			containers.push_back(child);
		}
		_GetAllContainers(child, containers);
	}
}

void Container::SetCondition(string condition)
{
	_condition = condition;
}

void Container::AddCondition(string add)
{
	_condition += add;
}

string Container::GetCondition()
{
	return _condition;
}

void Container::AddChildContainer(Container* cont)
{
	_childContainers.push_back(cont);
}

vector<Container*> Container::GetChildContainer()
{
	return _childContainers;
}

vector<Statement*> Container::GetStatements()
{
	return _statements;
}

void Container::AddStatements(Statement* ptrStmt)
{
	_statements.push_back(ptrStmt);
}


Container* Container::GetChildContainer(int idx)
{
	return _childContainers.at(idx);
}

void Container::SetType(string type)
{
	_type = type;
}

string Container::GetType()
{
	return _type;
}

/*
void Container::PrintStmt() {
	for (int i = 0; i < _statements.size(); i++) {
		cout << _statements.at(i)->_stmtNum << " : " << _statements.at(i)->_stmt << endl;
	}
}
*/

void Container::SetLevel(int level)
{
	_level = level;
}

int Container::GetLevel()
{
	return _level;
}

void Container::SetStartStmtNum(int stmtNum)
{
	_startStmtNum = stmtNum;
}

int Container::GetStartStmtNum()
{
	return _startStmtNum;
}

void Container::SetEndStmtNum(int stmtNum)
{
	_endStmtNum = stmtNum;
}

int Container::GetEndStmtNum()
{
	return _endStmtNum;
}

void Container::SetAdjustedStartStmtNum(int adjustment)
{
	_adjustedStartStmtNum = _startStmtNum + adjustment;
}

int Container::GetAdjustedStartStmtNum()
{
	return _adjustedStartStmtNum;
}

void Container::SetAdjustedEndStmtNum(int adjustment)
{
	_adjustedEndStmtNum = _endStmtNum + adjustment;
}

int Container::GetAdjustedEndStmtNum()
{
	return _adjustedEndStmtNum;
}
