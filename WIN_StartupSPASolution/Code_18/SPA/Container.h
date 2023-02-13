#pragma once
#include <sstream>
#include <fstream>
#include <vector>
//#include "Struct.h"
//#include "CFG.h"
#include <iostream>
#include <assert.h>

using namespace std;

struct Statement;

class Container {
public:
	// Constructor
	Container();

	// Destructor
	~Container();

	vector<Statement*> _statements;
	vector<Container*> _childContainers;
	string _condition;
	string _type;
	int _level;
	int _startStmtNum;
	int _endStmtNum;

	void printContainerTree(int tab);
	vector<Container*> getAllContainers();
	void _getAllContainers(Container* container, vector<Container*> &containers);
	//void printStmt();
};