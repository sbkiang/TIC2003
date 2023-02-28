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

	vector<string> _uses; // stores the variables that satisfy the assign's uses(s,v), print's uses(s,v), and container stmt s's uses(s,v)
	vector<string> _modifies; // stores the variables that satisfy the assign's uses(s,v), print's uses(s,v), and container stmt s's uses(s,v)
	vector<Procedure*> _calls; // stores the called procedures. Used to populate database for _uses, _modifies, _calls

	void _GetAllUses(Procedure* child, vector<string>& variable);
	void _GetAllModifies(Procedure* child, vector<string>& variable);
	void _GetAllCalls(Procedure* child, vector<Procedure*>& procedure);
	vector<string> GetAllUses(); // get all direct and indirect uses(s,v) for this procedure
	vector<string> GetAllModifies(); // get all direct and indirect modifies(s, v) for this procedure
	vector<Procedure*> GetAllCalls(); // get all direct and indirect calls for this procedure
};