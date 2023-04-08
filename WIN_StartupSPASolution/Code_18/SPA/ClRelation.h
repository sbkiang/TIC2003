#pragma once

using namespace std;

class ClRelation {
private:
	string _relationship;
	string _input1;
	string _input2;

public:
	ClRelation();
	ClRelation(string relationship, string input1, string input2);
	string GetRelationship();
	string GetInput1(); // get input with quotes
	string GetInput2();
	string GetInput1Unquoted(); // get input without quotes
	string GetInput2Unquoted();
};