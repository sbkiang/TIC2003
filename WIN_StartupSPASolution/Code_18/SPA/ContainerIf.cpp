#include "ContainerIf.h"

void ContainerIf::_GetAllChildContainer(IContainer& container, vector<IContainer*>& containers)
{
	for (int i = 0; i < container.GetChildContainer().size(); i++) {
		IContainer* child = container.GetChildContainer().at(i);
		containers.push_back(child);
		_GetAllChildContainer(*child, containers);
	}
}

ContainerIf::ContainerIf(int level)
{
	_level = level;
}

void ContainerIf::PrintContainerTree(int tab)
{
}

vector<IContainer*> ContainerIf::GetChildContainer()
{
	return _childContainer;
}

void ContainerIf::SetChildContainer(vector<IContainer*> childContainer)
{
	_childContainer = childContainer;
}

void ContainerIf::PushBackChildContainer(IContainer* childContainer)
{
	_childContainer.push_back(childContainer);
}

vector<IContainer*> ContainerIf::GetAllChildContainer()
{
	vector<IContainer*> containers;
	_GetAllChildContainer(*(this), containers);
	return containers;
}

vector<Statement*> ContainerIf::GetStatement()
{
	return _statement;
}

void ContainerIf::SetStatement(vector<Statement*> stmt)
{
	_statement = stmt;
}

void ContainerIf::PushBackStatement(Statement* stmt)
{
	_statement.push_back(stmt);
}

string ContainerIf::GetType()
{
	return _type;
}

int ContainerIf::GetStartStmtNum()
{
	return _startStmtNum;
}

void ContainerIf::SetStartStmtNum(int num)
{
	_startStmtNum = num;
}

int ContainerIf::GetEndStmtNum()
{
	return _endStmtNum;
}

void ContainerIf::SetEndStmtNum(int num)
{
	_endStmtNum = num;
}

int ContainerIf::GetLevel()
{
	return _level;
}

void ContainerIf::SetLevel(int level)
{
	_level = level;
}

int ContainerIf::GetAdjustedStartStmtNum()
{
	return _adjustedStartStmtNum;
}

void ContainerIf::SetAdjustedStartStmtNum(int num)
{
	_adjustedStartStmtNum = num;
}

int ContainerIf::GetAdjustedEndStmtNum()
{
	return _adjustedEndStmtNum;
}

void ContainerIf::SetAdjustedEndStmtNum(int num)
{
	_adjustedEndStmtNum = num;
}
