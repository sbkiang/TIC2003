#pragma once
#include <sstream>
#include <vector>
#include <iostream>
#include "IContainer.h"

using namespace std;

class ContainerProcedure : public IContainer {
private:
	string _name;
	string _type = "procedure";
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
	ContainerProcedure(string name, int level);
	vector<string> _uses; // stores the variables that satisfy the assign's uses(s,v), print's uses(s,v), and container stmt s's uses(s,v)
	vector<string> _modifies; // stores the variables that satisfy the assign's uses(s,v), print's uses(s,v), and container stmt s's uses(s,v)
	vector<ContainerProcedure*> _calls; // stores the called procedures. Used to populate database for _uses, _modifies, _calls

	void _GetAllUses(ContainerProcedure* child, vector<string>& variable);
	void _GetAllModifies(ContainerProcedure* child, vector<string>& variable);
	void _GetAllCalls(ContainerProcedure* child, vector<ContainerProcedure*>& procedure);
	vector<string> GetAllUses(); // get all direct and indirect uses(s,v) for this procedure
	vector<string> GetAllModifies(); // get all direct and indirect modifies(s, v) for this procedure
	vector<ContainerProcedure*> GetAllCalls(); // get all direct and indirect calls for this procedure
	vector<IContainer*> GetChildContainer();
	void PrintContainerTree(int tab);
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