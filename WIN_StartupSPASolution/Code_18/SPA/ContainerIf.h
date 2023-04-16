#pragma once
#include <sstream>
#include <vector>
#include <iostream>
#include "IContainer.h"

using namespace std;

class ContainerIf : public IContainer {
private:
	string _type = "if";
	vector<IContainer*> _childContainer;
	vector<Statement> _statement;
	IContainer* _parent;
	int _level = 0;
	int _startStmtNum = 0;
	int _endStmtNum = 0;
	int _adjustedStartStmtNum = 0;
	int _adjustedEndStmtNum = 0;

	void _GetAllChildContainer(IContainer& container, vector<IContainer*>& containers);

public:
	// Default Constructor
	ContainerIf(int level);
	void PrintContainerTree(int tab);
	vector<IContainer*> GetChildContainer();
	void SetChildContainer(vector<IContainer*> childContainer);
	void PushBackChildContainer(IContainer* childContainer);
	vector<IContainer*> GetAllChildContainer();
	vector<Statement> GetStatement();
	void SetStatement(vector<Statement> stmt);
	void PushBackStatement(Statement stmt);
	string GetType();
	int GetStartStmtNum();
	void SetStartStmtNum(int num);
	int GetEndStmtNum();
	void SetEndStmtNum(int num);
	int GetLevel();
	void SetLevel(int level);
	int GetAdjustedStartStmtNum();
	void SetAdjustedStartStmtNum(int num);
	int GetAdjustedEndStmtNum();
	void SetAdjustedEndStmtNum(int num);
};