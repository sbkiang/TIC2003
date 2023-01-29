#include "IfElseLinker.h"

// constructor
IfElseLinker::IfElseLinker(Container* ifPtr, Container* elsePtr) {
	_ifPtr = ifPtr;
	_elsePtr = elsePtr;
}

IfElseLinker::IfElseLinker() {
	_ifPtr = nullptr;
	_elsePtr = nullptr;
}

// destructor
IfElseLinker::~IfElseLinker() {}