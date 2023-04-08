#include "EntityPattern.h"

EntityPattern::EntityPattern(string syn, string input1, string input2)
{
	_synonym = syn;
	_input1 = input1;
	_input2 = input2;
}

string EntityPattern::GetSynonym()
{
	return _synonym;
}

string EntityPattern::GetInput1()
{
	return _input1;
}

string EntityPattern::GetInput2()
{
	return _input2;
}

string EntityPattern::GetInput1Unquoted()
{
	return HelperFunction::RemoveQuote(_input1);
}

string EntityPattern::GetInput2Unquoted()
{
	return HelperFunction::RemoveQuote(_input2);
}
