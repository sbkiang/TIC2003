#include "ClRelRef.h"

ClRelRef::ClRelRef()
{
}

ClRelRef::ClRelRef(string relationship, string input1, string input2)
{
	_relationship = relationship;
	_input1 = input1;
	_input2 = input2;
}

string ClRelRef::GetRelationship()
{
	return _relationship;
}

string ClRelRef::GetInput1()
{
	return _input1;
}

string ClRelRef::GetInput2()
{
	return _input2;
}

string ClRelRef::GetInput1Unquoted()
{
	return HelperFunction::RemoveQuote(_input1);
}

string ClRelRef::GetInput2Unquoted()
{
	return HelperFunction::RemoveQuote(_input2);
}
