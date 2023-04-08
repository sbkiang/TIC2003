#pragma once
#include "ClPattern.h"
#include "IDescriberClPattern.h"

class DescriberClPattern : public IDescriberClPattern {
private:
	bool _input1IsSyn = false;
	bool _input1IsWildcard = false;

public:
	DescriberClPattern(ClPattern clPattern, map<string,string> synEntMap);
	bool Input1IsSyn();
	bool Input1IsAny();

};