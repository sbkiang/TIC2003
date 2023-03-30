#pragma once
#include <string>
#include "Container.h"
#include <map>
#include <algorithm>
#include <set>

using namespace std;

struct CFGNode {
	Statement* _stmtPtr = NULL;
	CFGNode* _sJump = NULL; // For non-condition statement, _sJump points to next statement. For condition statement, _sJump points to first statement in the success condition block
	CFGNode* _fJump = NULL; //For non-condition statement, _fJump points to NULL. For condition statement, _fJump points to first statement in the fail condition block
};


struct SqlResult {
	map<string, string> row; // store mapping of column name to value
};


struct SqlResultSet {
	map<string, string> row; // store mapping of column name to value
	bool operator< (const SqlResult& otherSqlRow) const
	{
		vector<string> commonColName;
		set<string> thisColName;
		set<string> otherColName;
		for (auto it = row.begin(); it != row.end(); it++) {
			thisColName.insert(it->first);
		}
		for (auto it = otherSqlRow.row.begin(); it != otherSqlRow.row.end(); it++) {
			otherColName.insert(it->first);
		}
		set_intersection(thisColName.begin(), thisColName.end(), otherColName.begin(), otherColName.end(), inserter(commonColName, commonColName.begin()));
		size_t thisHashResult = 0;
		size_t otherHashResult = 0;
		for (int i = 0; i < commonColName.size(); i++) {
			string col = commonColName.at(i);
			thisHashResult = thisHashResult ^ hash<string>{}(row.at(col));
			otherHashResult = otherHashResult ^ hash<string>{}(otherSqlRow.row.at(col));
		}
		return thisHashResult < otherHashResult;
	}

	bool operator==(const SqlResult& otherSqlRow) const
	{
		vector<string> commonColName;
		set<string> thisColName;
		set<string> otherColName;
		for (auto it = row.begin(); it != row.end(); it++) {
			thisColName.insert(it->first);
		}
		for (auto it = otherSqlRow.row.begin(); it != otherSqlRow.row.end(); it++) {
			otherColName.insert(it->first);
		}
		set_intersection(thisColName.begin(), thisColName.end(), otherColName.begin(), otherColName.end(), inserter(commonColName, commonColName.begin()));
		size_t thisHashResult = 0;
		size_t otherHashResult = 0;
		for (int i = 0; i < commonColName.size(); i++) {
			string col = commonColName.at(i);
			thisHashResult = thisHashResult ^ hash<string>{}(row.at(col));
			otherHashResult = otherHashResult ^ hash<string>{}(otherSqlRow.row.at(col));
		}
		return thisHashResult == otherHashResult;
	}
};

struct SqlResultStore{
	vector<SqlResult> sqlResult;
	set<SqlResultSet> sqlResultSet;
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

