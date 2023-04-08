#include "DescriberClPattern.h"

DescriberClPattern::DescriberClPattern()
{
}

DescriberClPattern::DescriberClPattern(ClPattern clPattern, map<string,string> synEntMap)
{
	string input1 = clPattern.GetInput1Unquoted();
	bool input1IsQuoted = (clPattern.GetInput1()[0] == '"');
	_input1IsSyn = (synEntMap.find(input1) != synEntMap.end() && !input1IsQuoted);
	_input1IsWildcard = (input1 == "_") ? true : false;
	if (_input1IsSyn) { _entityInput1 = synEntMap.at(input1) ; }
}

bool DescriberClPattern::Input1IsSynonym()
{
	return _input1IsSyn;
}

bool DescriberClPattern::Input1IsAny()
{
	return (_input1IsSyn || _input1IsWildcard);
}