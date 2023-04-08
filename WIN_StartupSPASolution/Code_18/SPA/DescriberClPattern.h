#pragma once
#include "IDescriberClObject.h"

class DescriberClPattern : IDescriberClObject {
private:
	bool _input1IsSyn = false;
	bool _input1IsWildcard = false;

public:
	DescriberClPattern(ClPattern clPattern, map<string,string> synEntMap);
	bool Input1IsSynonym();
	bool Input1IsAny();
};