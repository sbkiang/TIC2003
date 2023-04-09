#pragma once
#include <string>

using namespace std;

class BuilderFullSqlRelRefAndPattern {
public:
	static string BuildSql(string colSql, string querySql);
};