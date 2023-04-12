#pragma once
#include "ClPattern.h"
#include "IDescriberClConstrain.h"

class DescriberClPattern : public IDescriberClConstrain {
private:
	bool _input1IsSyn = false;
	bool _input1IsWildcard = false;
	string _entityInput1;

public:
	DescriberClPattern(ClPattern clPattern, map<string,string> synEntMap);
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