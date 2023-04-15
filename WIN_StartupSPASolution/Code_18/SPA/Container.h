#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <assert.h>

using namespace std;

class Statement;

class Container {
public:
	// Constructor
	Container();

	// Destructor
	~Container();

	vector<Statement*> _statements;
	vector<Container*> _childContainers;
	Container* _parent;
	string _condition;
	string _type;
	int _level = 0;
	int _startStmtNum = 0;
	int _endStmtNum = 0;
	int _adjustedStartStmtNum = 0;
	int _adjustedEndStmtNum = 0;

	void printContainerTree(int tab);
	vector<Container> getAllContainers();
	void _getAllContainers(Container container, vector<Container> &containers);
	//void printStmt();
};