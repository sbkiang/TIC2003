#include "BuilderSqlSelectPattern.h"

string BuilderSqlSelectPattern::Build_NotAny_Expr(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select * from pattern where lhs = '%s' and expression like '%s'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectPattern::Build_Any_Expr(string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select * from pattern where expression like '%s' ", input2.c_str());
	return string(sqlBuf);
}

BuilderSqlSelectPattern::BuilderSqlSelectPattern(ClPattern clause)
{
	_clause = clause;
}

string BuilderSqlSelectPattern::GetSql(DescriberClPattern describer)
{
	string input1 = HelperFunction::ConvertPqlPatternOprtToSqlPatternOprt(_clause.GetInput1Unquoted());
	string input2 = HelperFunction::PatternExprToPostFix(_clause.GetInput2Unquoted());
	input2 = HelperFunction::EscapePercentSymbol(input2);
	input2 = HelperFunction::ConvertPqlPatternOprtToSqlPatternOprt(input2);
	if (describer.Input1IsAny()) {
		return Build_Any_Expr(input2);
	}
	else {
		return Build_NotAny_Expr(input1, input2);
	}
	return string();
}
