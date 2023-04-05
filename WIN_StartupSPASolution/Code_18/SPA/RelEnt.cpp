#include "RelEnt.h"

RelEnt::RelEnt(string relationship, string input1, string input2)
{
	_relationship = relationship;
	_input1 = input1;
	_input2 = input2;
}

string RelEnt::GetRelationship()
{
	return _relationship;
}

string RelEnt::GetInput1()
{
	return _input1;
}

string RelEnt::GetInput2()
{
	return _input2;
}

string RelEnt::GetInput1Unquoted()
{
	return HelperFunction::RemoveQuote(_input1);
}

string RelEnt::GetInput2Unquoted()
{
	return HelperFunction::RemoveQuote(_input2);
}
