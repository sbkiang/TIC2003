#include "Container.h"

// constructor
Container::Container() {
	_statementCount = 0;
}

// destructor
Container::~Container() {}

Statement::Statement(bool inAlt) {
	_altCondition = inAlt;
}