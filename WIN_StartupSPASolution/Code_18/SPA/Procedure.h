#pragma once
#include <sstream>
#include <vector>
#include <iostream>
#include "Container.h"

using namespace std;

class Procedure : public Container {
public:
	// Default Constructor
	Procedure(string name);

	// Default Destructor
	~Procedure();
	string _name;
};