#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Container.h"
#include <iostream>
#include <queue>

using namespace std;

class Procedure : public Container {
public:
	// Default Constructor
	Procedure();

	// Default Destructor
	~Procedure();

	// method for processing the source program
	//virtual vector<string> getVectorStatements() = 0;
	//virtual int getStatementCount() = 0;
	string _name;

	void print();
	void _print();
};