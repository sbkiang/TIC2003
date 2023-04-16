#include "ContainerWhile.h"

void ContainerWhile::_GetAllChildContainer(IContainer& container, vector<IContainer*>& containers)
{
	for (int i = 0; i < container.GetChildContainer().size(); i++) {
		IContainer* child = container.GetChildContainer().at(i);
		containers.push_back(child);
		_GetAllChildContainer(*child, containers);
	}
}

ContainerWhile::ContainerWhile(int level)
{
	_level = level;
}

void ContainerWhile::PrintContainerTree(int tab)
{
}

vector<IContainer*> ContainerWhile::GetChildContainer()
{
	return _childContainer;
}

void ContainerWhile::SetChildContainer(vector<IContainer*> childContainer)
{
	_childContainer = childContainer;
}

void ContainerWhile::PushBackChildContainer(IContainer* childContainer)
{
	_childContainer.push_back(childContainer);
}

vector<IContainer*> ContainerWhile::GetAllChildContainer()
{
	vector<IContainer*> containers;
	_GetAllChildContainer(*(this), containers);
	return containers;
}

vector<Statement> ContainerWhile::GetStatement()
{
	return _statement;
}

void ContainerWhile::SetStatement(vector<Statement> stmt)
{
	_statement = stmt;
}

void ContainerWhile::PushBackStatement(Statement stmt)
{
	_statement.push_back(stmt);
}

string ContainerWhile::GetType()
{
	return _type;
}

int ContainerWhile::GetStartStmtNum()
{
	return _startStmtNum;
}

void ContainerWhile::SetStartStmtNum(int num)
{
	_startStmtNum = num;
}

int ContainerWhile::GetEndStmtNum()
{
	return _endStmtNum;
}

void ContainerWhile::SetEndStmtNum(int num)
{
	_endStmtNum = num;
}

int ContainerWhile::GetLevel()
{
	return _level;
}

void ContainerWhile::SetLevel(int level)
{
	_level = level;
}

int ContainerWhile::GetAdjustedStartStmtNum()
{
	return _adjustedStartStmtNum;
}

void ContainerWhile::SetAdjustedStartStmtNum(int num)
{
	_adjustedStartStmtNum = num;
}

int ContainerWhile::GetAdjustedEndStmtNum()
{
	return _adjustedEndStmtNum;
}

void ContainerWhile::SetAdjustedEndStmtNum(int num)
{
	_adjustedEndStmtNum = num;
}
