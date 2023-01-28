#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Container.h"

using namespace std;

class While : Container {
public:
	// Constructor
	While();

	// Destructor
	~While();

	// method for processing the source program
	//virtual vector<string> getVectorStatements() = 0;
	//virtual int getStatementCount() = 0;
	string condition;
};