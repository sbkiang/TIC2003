#include "BuilderColumnSqlPattern.h"

string BuilderColumnSqlPattern::Build_Synonym(string selectSynonym, string input1Synonym)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select line_num as %s, lhs as %s", selectSynonym.c_str(), input1Synonym.c_str());
	return string(sqlBuf);
}

string BuilderColumnSqlPattern::Build_NotSynonym(string selectSynonym)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select line_num as %s", selectSynonym.c_str());
	return string(sqlBuf);
}

BuilderColumnSqlPattern::BuilderColumnSqlPattern(ClPattern cp)
{
    _cp = cp;
}

string BuilderColumnSqlPattern::GetSqlQuery(DescriberClPattern describer)
{
	string input1 = _cp.GetInput1();
	string input2 = _cp.GetInput2();
	string syn = _cp.GetSynonym();
	bool input1IsSyn = describer.Input1IsSyn();
	input1 = _cp.GetInput1Unquoted();
	if(describer.Input1IsSyn()){
		return Build_Synonym(syn, input1);
	}
	else {
		return Build_NotSynonym(syn);
	}
	return string();
}
