#pragma once
#include "ClPattern.h"

class DescriberClPattern {
private:
	bool _input1IsSyn = false;
	bool _input1IsWildcard = false;

public:
	DescriberClPattern(ClPattern clPattern, map<string,string> synEntMap);
	bool Input1IsSyn();
	bool Input1IsAny();

};