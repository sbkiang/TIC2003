#pragma once
#include <vector>
#include "Statement.h"

using namespace std;

class IContainer {
public:
	virtual void PrintContainerTree(int tab) = 0;
	virtual vector<IContainer*> GetChildContainer() = 0;
	virtual void SetChildContainer(vector<IContainer*> childContainer) = 0;
	virtual void PushBackChildContainer(IContainer* childContainer) = 0;
	virtual vector<IContainer*> GetAllChildContainer() = 0;
	virtual vector<Statement> GetStatement() = 0;
	virtual void PushBackStatement(Statement stmt) = 0;
	virtual void SetStatement(vector<Statement> stmt) = 0;
	virtual string GetType() = 0;
	virtual int GetStartStmtNum() = 0;
	virtual void SetStartStmtNum(int num) = 0;
	virtual int GetEndStmtNum() = 0;
	virtual void SetEndStmtNum(int num) = 0;
	virtual int GetLevel() = 0;
	virtual void SetLevel(int level) = 0;
	virtual int GetAdjustedStartStmtNum() = 0;
	virtual void SetAdjustedStartStmtNum(int num) = 0;
	virtual int GetAdjustedEndStmtNum() = 0;
	virtual void SetAdjustedEndStmtNum(int num) = 0;

};