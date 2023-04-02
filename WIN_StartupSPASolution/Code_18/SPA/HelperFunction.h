#pragma once
#include <format>
#include <string>
#include <stack>
#include <regex>
#include "../source/Database.h"
#include <set>

using namespace std;

class HelperFunction {
public:
	static string GetEntityByStatement(string input);
	static string PatternExprToPostFix(string input);
	static string ConvertPqlPatternOprtToSqlPatternOprt(string input);
	static string RemoveQuote(string input);
	static set<string> GetColName(set<RowSet> set1);
	static set<RowSet> CartesianProduct(set<RowSet> set1, set<RowSet> set2);

	// this function is to compare each set1 with set2 and return set same column values instead of relying on set_intersection
	static set<RowSet> CommonColumnIntersect(set<RowSet> set1, set<RowSet> set2);
	static void PrintRowSet(set<RowSet> rs);

private:
	static int Prec(char c);
	static string InfixToPostfix(string s);
};