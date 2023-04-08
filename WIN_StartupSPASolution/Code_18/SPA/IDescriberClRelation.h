#pragma once
#include "IDescriberCl.h"

class IDescriberClRelation : public IDescriberCl {
	virtual bool Input1IsSyn() = 0;
	virtual bool Input2IsSyn() = 0;
	virtual bool Input1IsWildcard() = 0;
	virtual bool Input2IsWildcard() = 0;

	//return (_input1IsSyn || _input1IsWildcard);
	virtual bool Input1IsAny() = 0;

	//return (_input2IsSyn || _input2IsWildcard);
	virtual bool Input2IsAny() = 0;

	//return (_entityInput1 == "stmt" || _input1IsWildcard)
	virtual bool Input1IsStmtOrWildcard() = 0;

	//return (_entityInput2 == "stmt" || _input2IsWildcard)
	virtual bool Input2IsStmtOrWildcard() = 0;

	virtual string EntityInput1() = 0;
	virtual string EntityInput2() = 0;
};