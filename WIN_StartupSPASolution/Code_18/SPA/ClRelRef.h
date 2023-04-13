#pragma once
#include "IClConstrain.h"
#include "HelperFunction.h"

using namespace std;

class ClRelRef : public IClConstrain{
private:
	string _relationship;
	string _input1;
	string _input2;
public:
	ClRelRef();
	ClRelRef(string relationship, string input1, string input2);
	string GetRelRefRelation();
	string GetPatternSynonym();
	string GetInput1(); // get input with quotes
	string GetInput2();
	string GetInput1Unquoted(); // get input without quotes
	string GetInput2Unquoted();
};