#pragma once
#include <string>

using namespace std;

class IClConstrain {
public:
	virtual string GetPatternSynonym() = 0;
	virtual string GetRelRefRelation() = 0;
	virtual string GetInput1() = 0; // get input with quotes
	virtual string GetInput2() = 0;
	virtual string GetInput1Unquoted() = 0; // get input without quotes
	virtual string GetInput2Unquoted() = 0;
};