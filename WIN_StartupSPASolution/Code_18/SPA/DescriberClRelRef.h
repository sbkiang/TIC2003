#pragma once
#include <string>
#include "define.h"
#include "ClRelRef.h"

using namespace std;

// describes whether the inputs in ClRelRef is synonym, is wildcard
class DescriberClRelRef {
private:
	bool _input1IsSyn = false;
	bool _input2IsSyn = false;
	bool _input1IsWildcard = false;
	bool _input2IsWildcard = false;
	bool _input1IsStmtOrWildcard = false;
	bool _input2IsStmtOrWildcard = false;
	string _entityInput1;
	string _entityInput2;

public:
	DescriberClRelRef(ClRelRef re, map<string, string> synEntMap);

	bool Input1IsSyn();
	bool Input2IsSyn();
	bool Input1IsWildcard();
	bool Input2IsWildcard();
	
	//return (_input1IsSyn || _input1IsWildcard);
	bool Input1IsAny();
	
	//return (_input2IsSyn || _input2IsWildcard);
	bool Input2IsAny();

	//return (_entityInput1 == "stmt" || _input1IsWildcard)
	bool Input1IsStmtOrWildcard();

	//return (_entityInput2 == "stmt" || _input2IsWildcard)
	bool Input2IsStmtOrWildcard();

	string EntityInput1();
	string EntityInput2();
};