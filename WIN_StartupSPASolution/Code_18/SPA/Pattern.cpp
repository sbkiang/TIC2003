#include "Pattern.h"

Pattern::Pattern(string syn, string input1, string input2)
{
	_synonym = syn;
	_input1 = input1;
	_input2 = input2;
}

string Pattern::GetSynonym()
{
	return _synonym;
}

string Pattern::GetInput1()
{
	return _input1;
}

string Pattern::GetInput2()
{
	return _input2;
}

string Pattern::GetInput1Unquoted()
{
	return HelperFunction::RemoveQuote(_input1);
}
