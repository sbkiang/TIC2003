#include "BuilderSqlSelectPattern.h"

string BuilderSqlSelectPattern::Build_NotAny_Expr(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select * from pattern where lhs = '%s' and expression like '%s' escape '!'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectPattern::Build_Any_Expr(string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select * from pattern where expression like '%s' escape '!'", input2.c_str());
	return string(sqlBuf);
}

BuilderSqlSelectPattern::BuilderSqlSelectPattern()
{
}

string BuilderSqlSelectPattern::GetSql(IClConstrain& re, IDescriberClConstrain& describer)
{
	string input1 = HelperFunction::ConvertPqlPatternOprtToSqlPatternOprt(re.GetInput1Unquoted());
	string input2 = HelperFunction::PatternExprToPostFix(re.GetInput2Unquoted());
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
