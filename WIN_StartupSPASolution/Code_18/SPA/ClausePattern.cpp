#include "ClausePattern.h"

// Pattern(v, _ / _"x"_)
string ClausePattern::PatternConstruct_Synonym_NotSynonym(string input1)
{
	char sqlBuf[512] = {};
	//sprintf_s(sqlBuf, "select lhs as %s, rhs", input1.c_str());
	sprintf_s(sqlBuf, "select lhs as %s", input1.c_str());
	return string(sqlBuf);
}

// Pattern(_/"x", _/_"x"_)
string ClausePattern::PatternConstruct_NotSynonym_NotSynonym()
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select lhs, rhs");
	return string(sqlBuf);
}

string ClausePattern::ConvertPqlPatternOprtToSqlPatternOprt(string input)
{
	int pos = 0;
	do {
		pos = input.find("_");
		if (pos > -1) { input.replace(pos, 1, "%"); }
	} while (pos > -1);
	return input;
}

string ClausePattern::insertPattern(int stmtNum, string LHS, string RHS, string expression)
{
	return "";
}

// Pattern("x", _/_"x+1"_/"x+1")
string ClausePattern::GetPattern_NotAny_Expr(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from pattern p where p.lhs = '%s' and p.rhs like '%s';", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

// Pattern(v/_, _/_"x+1"_/"x+1") 
string ClausePattern::GetPattern_Any_Expr(string frontSql, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from pattern p where p.rhs like '%s';", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}
