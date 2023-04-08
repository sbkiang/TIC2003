#include "ColumnBuilderSqlPattern.h"

string ColumnBuilderSqlPattern::Build_Synonym(string selectSynonym, string input1Synonym)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select line_num as %s, lhs as %s", selectSynonym.c_str(), input1Synonym.c_str());
	return string(sqlBuf);
}

string ColumnBuilderSqlPattern::Build_NotSynonym(string selectSynonym)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select line_num as %s", selectSynonym.c_str());
	return string(sqlBuf);
}

ColumnBuilderSqlPattern::ColumnBuilderSqlPattern(ClPattern cp)
{
    _cp = cp;
}

string ColumnBuilderSqlPattern::GetSqlQuery(DescriberClPattern describer)
{
	string input1 = _cp.GetInput1();
	string input2 = _cp.GetInput2();
	string syn = _cp.GetSynonym();
	bool input1IsSyn = describer.Input1IsSyn();
	bool input2IsSyn = describer.Input2IsSyn();
	input1 = _cp.GetInput1Unquoted();
	input2 = _cp.GetInput2Unquoted();
	if(describer.input1IsSynonym()){
		return Build_Synonym(syn, input1);
	}
	else {
		return Build_NotSynonym(syn);
	}
	return string();
}
