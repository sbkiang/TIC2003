#pragma once
#include <string>
#include "HelperFunction.h"

using namespace std;

class Pattern {
private:
	string _synonym = "";
	string _input1 = "";
	string _input2 = "";

public:
	Pattern(string syn, string input1, string input2);
	string GetSynonym();
	string GetInput1();
	string GetInput2();
	string GetInput1Unquoted();
	bool input1Quoted = false;
};