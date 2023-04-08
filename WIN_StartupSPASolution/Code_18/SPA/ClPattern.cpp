#include "ClPattern.h"

ClPattern::ClPattern() {

}

ClPattern::ClPattern(string syn, string input1, string input2)
{
	_synonym = syn;
	_input1 = input1;
	_input2 = input2;
}

string ClPattern::GetSynonym()
{
	return _synonym;
}

string ClPattern::GetInput1()
{
	return _input1;
}

string ClPattern::GetInput2()
{
	return _input2;
}

string ClPattern::GetInput1Unquoted()
{
	return HelperFunction::RemoveQuote(_input1);
}

string ClPattern::GetInput2Unquoted()
{
	return HelperFunction::RemoveQuote(_input2);
}