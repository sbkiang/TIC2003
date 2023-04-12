#include "DescriberClPattern.h"

DescriberClPattern::DescriberClPattern(ClPattern clPattern, map<string,string> synEntMap)
{
	string input1 = clPattern.GetInput1Unquoted();
	bool input1IsQuoted = (clPattern.GetInput1()[0] == '"');
	_input1IsSyn = (synEntMap.find(input1) != synEntMap.end() && !input1IsQuoted);
	_input1IsWildcard = (input1 == "_") ? true : false;
	if (_input1IsSyn) { _entityInput1 = synEntMap.at(input1); }
}

bool DescriberClPattern::Input1IsSyn()
{
	return _input1IsSyn;
}

bool DescriberClPattern::Input2IsSyn()
{
	return false;
}

bool DescriberClPattern::Input1IsWildcard()
{
	return _input1IsWildcard;
}

bool DescriberClPattern::Input2IsWildcard()
{
	return false;
}

bool DescriberClPattern::Input1IsAny()
{
	return (_input1IsSyn || _input1IsWildcard);
}

bool DescriberClPattern::Input2IsAny()
{
	return false;
}

bool DescriberClPattern::Input1IsStmtOrWildcard()
{
	return (_entityInput1 == "stmt" || _input1IsWildcard);
}

bool DescriberClPattern::Input2IsStmtOrWildcard()
{
	return false;
}

string DescriberClPattern::EntityInput1()
{
	return _entityInput1;
}

string DescriberClPattern::EntityInput2()
{
	return "";
}
