#pragma once
#include <string>
#include <format>

using namespace std;

class ClausePattern {
public:

	static void insertPattern(int stmtNum, string LHS, string RHS, string expression);

	// Pattern a(v, expr)
	static string PatternConstruct_Synonym(string selectSynonym, string input1Synonym);

	// Pattern a(/"x", expr)
	static string PatternConstruct_NotSynonym(string selectSynonym);

	static string insertPattern(int stmtNum, string LHS, string RHS, string expression);

	// Pattern("x", _/_"x"_)
	static string GetPattern_NotAny_Expr(string frontSql, string input1, string input2);

	// Pattern(v/_, _/_"x"_) 
	static string GetPattern_Any_Expr(string frontSql, string input2); 
};