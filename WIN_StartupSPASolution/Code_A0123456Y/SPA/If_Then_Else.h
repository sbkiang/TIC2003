#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Container.h"

using namespace std;

class If_Then_Else : Container {
public:

	// Constructor
	If_Then_Else();

	// Destructor
	~If_Then_Else();

	// method for processing the source program
	//virtual vector<string> getVectorStatements() = 0;
	//virtual int getStatementCount() = 0;
};