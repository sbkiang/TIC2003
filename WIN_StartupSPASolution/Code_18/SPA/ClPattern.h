#pragma once
#include <string>
#include "HelperFunction.h"

using namespace std;

class ClPattern {
private:
	string _synonym;
	string _input1;
	string _input2;

public:
	ClPattern();
	ClPattern(string syn, string input1, string input2);
	string GetSynonym();
	string GetInput1();
	string GetInput2();
	string GetInput1Unquoted();
	string GetInput2Unquoted();
};