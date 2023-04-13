#include "BuilderSqlColumnPattern.h"

string BuilderSqlColumnPattern::Build_Synonym(string selectSynonym, string input1Synonym)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select line_num as %s, lhs as %s", selectSynonym.c_str(), input1Synonym.c_str());
	return string(sqlBuf);
}

string BuilderSqlColumnPattern::Build_NotSynonym(string selectSynonym)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select line_num as %s", selectSynonym.c_str());
	return string(sqlBuf);
}

BuilderSqlColumnPattern::BuilderSqlColumnPattern()
{
}

string BuilderSqlColumnPattern::GetSql(IClConstrain& re, IDescriberClConstrain& describer)
{
	string input1 = re.GetInput1();
	string input2 = re.GetInput2();
	string syn = re.GetPatternSynonym();
	bool input1IsSyn = describer.Input1IsSyn();
	input1 = re.GetInput1Unquoted();
	if(describer.Input1IsSyn()){
		return Build_Synonym(syn, input1);
	}
	else {
		return Build_NotSynonym(syn);
	}
	return string();
}
