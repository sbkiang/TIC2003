#include "ContainerElse.h"

void ContainerElse::_GetAllChildContainer(IContainer& container, vector<IContainer*>& containers)
{
	for (int i = 0; i < container.GetChildContainer().size(); i++) {
		IContainer* child = container.GetChildContainer().at(i);
		containers.push_back(child);
		_GetAllChildContainer(*child, containers);
	}
}

ContainerElse::ContainerElse(int level)
{
	_level = level;
}

void ContainerElse::PrintContainerTree(int tab)
{
}

vector<IContainer*> ContainerElse::GetChildContainer()
{
	return _childContainer;
}

void ContainerElse::SetChildContainer(vector<IContainer*> childContainer)
{
	_childContainer = childContainer;
}

void ContainerElse::PushBackChildContainer(IContainer* childContainer)
{
	_childContainer.push_back(childContainer);
}

vector<IContainer*> ContainerElse::GetAllChildContainer()
{
	vector<IContainer*> containers;
	_GetAllChildContainer(*(this), containers);
	return containers;
}

vector<Statement*> ContainerElse::GetStatement()
{
	return _statement;
}

void ContainerElse::SetStatement(vector<Statement*> stmt)
{
	_statement = stmt;
}

void ContainerElse::PushBackStatement(Statement* stmt)
{
	_statement.push_back(stmt);
}

string ContainerElse::GetType()
{
	return _type;
}

int ContainerElse::GetStartStmtNum()
{
	return _startStmtNum;
}

void ContainerElse::SetStartStmtNum(int num)
{
	_startStmtNum = num;
}

int ContainerElse::GetEndStmtNum()
{
	return _endStmtNum;
}

void ContainerElse::SetEndStmtNum(int num)
{
	_endStmtNum = num;
}

int ContainerElse::GetLevel()
{
	return _level;
}

void ContainerElse::SetLevel(int level)
{
	_level = level;
}

int ContainerElse::GetAdjustedStartStmtNum()
{
	return _adjustedStartStmtNum;
}

void ContainerElse::SetAdjustedStartStmtNum(int num)
{
	_adjustedStartStmtNum = num;
}

int ContainerElse::GetAdjustedEndStmtNum()
{
	return _adjustedEndStmtNum;
}

void ContainerElse::SetAdjustedEndStmtNum(int num)
{
	_adjustedEndStmtNum = num;
}
