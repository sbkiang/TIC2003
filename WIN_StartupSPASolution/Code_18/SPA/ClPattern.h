#pragma once
#include "IClConstrain.h"
#include "HelperFunction.h"

using namespace std;

class ClPattern : public IClConstrain{
private:
	string _synonym;
	string _input1;
	string _input2;

public:
	ClPattern();
	ClPattern(string syn, string input1, string input2);
	string GetPatternSynonym();
	string GetRelRefRelation();
	string GetInput1();
	string GetInput2();
	string GetInput1Unquoted();
	string GetInput2Unquoted();
};