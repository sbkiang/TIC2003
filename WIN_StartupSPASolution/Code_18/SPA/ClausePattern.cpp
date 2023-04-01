#include "ClausePattern.h"

string ClausePattern::PatternConstruct_Synonym(string selectSynonym, string input1Synonym)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select line_num as %s, lhs as %s", selectSynonym.c_str(), input1Synonym.c_str());
	return string(sqlBuf);
}

string ClausePattern::PatternConstruct_NotSynonym(string selectSynonym)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select line_num as %s", selectSynonym.c_str());
	return string(sqlBuf);
}

string ClausePattern::insertPattern(int stmtNum, string LHS, string RHS, string expression)
{
	return "";
}

// Pattern("x", _/_"x+1"_/"x+1")
string ClausePattern::GetPattern_NotAny_Expr(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from pattern p where p.lhs = '%s' and p.expression like '%s';", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

// Pattern(v/_, _/_"x+1"_/"x+1") 
string ClausePattern::GetPattern_Any_Expr(string frontSql, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from pattern p where p.expression like '%s';", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}
