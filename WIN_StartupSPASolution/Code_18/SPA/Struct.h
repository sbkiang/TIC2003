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

struct Row {
	map<string, string> row; // store mapping of column name to value
};


struct RowSet {
	map<string, string> row; // store mapping of column name to value

	bool operator< (const RowSet& otherSqlRow) const;
};

struct SqlResultStore{
	//vector<Row> sqlResult;
	set<RowSet> sqlResultSet;
};

struct Select {
	vector<string> synonym;
	vector<string> tableSql;
	vector<string> whereSql;
	vector<string> columnSql;
	vector<string> asSql;
};
