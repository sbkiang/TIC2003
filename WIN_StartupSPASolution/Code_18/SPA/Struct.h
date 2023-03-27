#pragma once
#include <string>
#include "Container.h"
#include <map>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct SqlResult;

struct CFGNode {
	Statement* _stmtPtr = NULL;
	CFGNode* _sJump = NULL; // For non-condition statement, _sJump points to next statement. For condition statement, _sJump points to first statement in the success condition block
	CFGNode* _fJump = NULL; //For non-condition statement, _fJump points to NULL. For condition statement, _fJump points to first statement in the fail condition block
};

struct SqlResultStore {
	vector<SqlResult> sqlResult;
	set<SqlResult> sqlResult;
	set<string> colName;
};

struct SqlResult : SqlResultStore {
	map<string, string> row; // store mapping of column name to value
	bool operator< (const SqlResult& otherSqlRow) const
	{
		vector<string> intersect;
		set_intersection(colName.begin(), colName.end(), otherSqlRow.colName.begin(), otherSqlRow.colName.end(), inserter(intersect, intersect.begin()));
		size_t thisHashResult = 0;
		size_t otherHashResult = 0;
		for (int i = 0; i < intersect.size(); i++) {
			string col = intersect.at(i);
			thisHashResult = thisHashResult ^ hash<string>{}(row.at(col));
			otherHashResult = otherHashResult ^ hash<string>{}(otherSqlRow.row.at(col));
		}
		return thisHashResult < otherHashResult;
	}
	bool operator==(const SqlResult& otherSqlRow) const
	{
		vector<string> intersect;
		set_intersection(colName.begin(), colName.end(), otherSqlRow.colName.begin(), otherSqlRow.colName.end(), inserter(intersect, intersect.begin()));
		size_t thisHashResult = 0;
		size_t otherHashResult = 0;
		for (int i = 0; i < intersect.size(); i++) {
			string col = intersect.at(i);
			thisHashResult = thisHashResult ^ hash<string>{}(row.at(col));
			otherHashResult = otherHashResult ^ hash<string>{}(otherSqlRow.row.at(col));
		}
		return thisHashResult == otherHashResult;
	}
};


struct SuchThat {
	string relationship = "";
	string input1 = "";
	string input2 = "";
};

struct Pattern {
	string synonym = "";
	string input1 = "";
	string input2 = "";
};

struct Select {
	vector<string> synonym;
	vector<string> tableSql;
	vector<string> whereSql;
	vector<string> columnSql;
	vector<string> asSql;
};

