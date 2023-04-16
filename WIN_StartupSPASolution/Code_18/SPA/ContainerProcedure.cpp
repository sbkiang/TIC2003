#include "ContainerProcedure.h"

void ContainerProcedure::_GetAllChildContainer(IContainer& container, vector<IContainer*>& containers)
{
	for (int i = 0; i < container.GetChildContainer().size(); i++) {
		IContainer* child = container.GetChildContainer().at(i);
		containers.push_back(child);
		_GetAllChildContainer(*child, containers);
	}
}

ContainerProcedure::ContainerProcedure(int level)
{
	_level = level;
}

vector<IContainer*> ContainerProcedure::GetChildContainer()
{
	return _childContainer;
}

void ContainerProcedure::PrintContainerTree(int tab)
{
}

void ContainerProcedure::SetChildContainer(vector<IContainer*> childContainer)
{
	_childContainer = childContainer;
}

void ContainerProcedure::PushBackChildContainer(IContainer* childContainer)
{
	_childContainer.push_back(childContainer);
}

vector<IContainer*> ContainerProcedure::GetAllChildContainer()
{
	vector<IContainer*> containers;
	_GetAllChildContainer(*(this), containers);
	return containers;
}

vector<Statement*> ContainerProcedure::GetStatement()
{
	return _statement;
}

void ContainerProcedure::SetStatement(vector<Statement*> stmt)
{
	_statement = stmt;
}

void ContainerProcedure::PushBackStatement(Statement* stmt)
{
	_statement.push_back(stmt);
}

string ContainerProcedure::GetType()
{
	return _type;
}

int ContainerProcedure::GetStartStmtNum()
{
	return _startStmtNum;
}

void ContainerProcedure::SetStartStmtNum(int num)
{
	_startStmtNum = num;
}

int ContainerProcedure::GetEndStmtNum()
{
	return _endStmtNum;
}

void ContainerProcedure::SetEndStmtNum(int num)
{
	_endStmtNum = num;
}

int ContainerProcedure::GetLevel()
{
	return _level;
}

void ContainerProcedure::SetLevel(int level)
{
	_level = level;
}

int ContainerProcedure::GetAdjustedStartStmtNum()
{
	return _adjustedStartStmtNum;
}

void ContainerProcedure::SetAdjustedStartStmtNum(int num)
{
	_adjustedStartStmtNum = num;
}

int ContainerProcedure::GetAdjustedEndStmtNum()
{
	return _adjustedEndStmtNum;
}

void ContainerProcedure::SetAdjustedEndStmtNum(int num)
{
	_adjustedEndStmtNum = num;
}
