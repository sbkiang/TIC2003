#include "DescriberClRelation.h"

DescriberClRelation::DescriberClRelation(ClRelation re, map<string, string> synEntMap)
{
	string input1 = re.GetInput1Unquoted();
	string input2 = re.GetInput2Unquoted();
	bool input1IsQuoted = (re.GetInput1()[0] == '"');
	bool input2IsQuoted = (re.GetInput2()[0] == '"');
	_input1IsSyn = (synEntMap.find(input1) != synEntMap.end() && !input1IsQuoted);
	_input2IsSyn = (synEntMap.find(input2) != synEntMap.end() && !input2IsQuoted);
	_input1IsWildcard = (input1 == "_") ? true : false;
	_input2IsWildcard = (input2 == "_") ? true : false;
	if (_input1IsSyn) { _entityInput1 = synEntMap.at(input1); }
	if (_input2IsSyn) { _entityInput2 = synEntMap.at(input2); }
}

bool DescriberClRelation::Input1IsSyn()
{
	return _input1IsSyn;
}

bool DescriberClRelation::Input2IsSyn()
{
	return _input2IsSyn;
}

bool DescriberClRelation::Input1IsWildcard()
{
	return _input1IsWildcard;
}

bool DescriberClRelation::Input2IsWildcard()
{
	return _input2IsWildcard;
}

bool DescriberClRelation::Input1IsAny()
{
	return (_input1IsSyn || _input1IsWildcard);
}

bool DescriberClRelation::Input2IsAny()
{
	return (_input2IsSyn || _input2IsWildcard);
}

string DescriberClRelation::EntityInput1()
{
	return _entityInput1;
}

string DescriberClRelation::EntityInput2()
{
	return _entityInput2;
}

bool DescriberClRelation::Input1IsStmtOrWildcard()
{
	return (_entityInput1 == "stmt" || _input1IsWildcard);
}

bool DescriberClRelation::Input2IsStmtOrWildcard()
{
	return (_entityInput2 == "stmt" || _input2IsWildcard);
}
