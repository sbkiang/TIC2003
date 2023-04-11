#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <assert.h>

using namespace std;

class Statement;

class Container {
private:
	vector<Statement*> _statements;
	vector<Container*> _childContainers;
	string _condition;
	string _type;
	int _level = 0;
	int _startStmtNum = 0;
	int _endStmtNum = 0;
	int _adjustedStartStmtNum = 0;
	int _adjustedEndStmtNum = 0;

public:
	// Constructor
	Container();

	void PrintContainerTree(int tab);
	vector<Container> GetAllContainers();
	void _GetAllContainers(Container container, vector<Container> &containers);
	void AddChildContainer(Container* cont);
	vector<Statement*> GetStatements();
	void AddStatements(Statement* ptrStmt);
	vector<Container*> GetChildContainer();
	Container* GetChildContainer(int idx);
	void SetCondition(string condition);
	void AddCondition(string add);
	string GetCondition();
	void SetType(string type);
	string GetType();
	void SetLevel(int level);
	int GetLevel();
	void SetStartStmtNum(int stmtNum);
	int GetStartStmtNum();
	void SetEndStmtNum(int stmtNum);
	int GetEndStmtNum();
	void SetAdjustedStartStmtNum(int adjustment);
	int GetAdjustedStartStmtNum();
	void SetAdjustedEndStmtNum(int adjustment);
	int GetAdjustedEndStmtNum();
	//void PrintStmt();
};