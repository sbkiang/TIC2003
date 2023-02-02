#pragma once

using namespace std;

class Container; // IfElseLinker class doesn't need to know anything about Container, other than it's a class. So, we forward declare it here instead of using include which causes error

class IfElseLinker{
public:
	// Default Constructor
	IfElseLinker(Container* _ifPtr, Container* _elsePtr);
	IfElseLinker();

	// Default Destructor
	~IfElseLinker();
	Container* _ifPtr;
	Container* _elsePtr;
};