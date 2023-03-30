#pragma once
#include <string>
#include <format>

using namespace std;

class ClausePattern {
public:
	static string PatternConstruct_Synonym_NotSynonym(string input1); // Pattern(v, _/_"x"_)
	static string PatternConstruct_NotSynonym_NotSynonym(); // Pattern(_/_"x"_, _/_"x"_)
	static string ConvertPqlPatternOprtToSqlPatternOprt(string input);
	static string insertPattern(int stmtNum, string LHS, string RHS, string expression);
	static string GetPattern_NotAny_Expr(string frontSql, string input1, string input2); // Pattern("x", _/_"x"_)
	static string GetPattern_Any_Expr(string frontSql, string input2); // Pattern(v/_, _/_"x"_) 
};