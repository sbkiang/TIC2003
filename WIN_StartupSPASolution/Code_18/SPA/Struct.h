#pragma once
#include <string>
#include "Container.h"
#include <map>
#include <set>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct SqlResult;

struct Statement {
	string _stmt;
	int _stmtNum = 0;
	int _level = 0;
	int _stmtNumSubtract = 0;
	bool _containerHead = false;
	bool _containerTail = false;
	Container* _container = nullptr;
	Statement(int stmtNum, string stmt, int stmtNumSubtract);
	Statement(int stmtNum, string stmt, Container* container, int stmtNumSubtract);
	Statement(int stmtNum, int level, Container* container, int stmtNumSubtract);
	Statement(int stmtNum, string stmt, int level, Container* container, int stmtNumSubtract);
	int getAdjustedStmtNum();
};

struct CFGNode {
	Statement* _stmtPtr = NULL;
	CFGNode* _sJump = NULL; // For non-condition statement, _sJump points to next statement. For condition statement, _sJump points to first statement in the success condition block
	CFGNode* _fJump = NULL; //For non-condition statement, _fJump points to NULL. For condition statement, _fJump points to first statement in the fail condition block
};

struct SqlResultStore {
	vector<SqlResult> sqlResult;
	set<SqlResult> sqlResultSet;
	set<string> resultColumns;
	vector<string> column;
	vector<string> getColumnValues(string column);
	vector<string> getColumnValues2(string column);
};


struct SqlResult : SqlResultStore {
	map<string, string> row; // store mapping of column name to value
	SqlResult() {}
	SqlResult(string col, string val) {
		row.insert(pair<string, string>(col, val));
	}
	
	bool operator< (const SqlResult& otherSqlRow) const
	{
		vector<string> intersect;
		set_intersection(resultColumns.begin(), resultColumns.end(), otherSqlRow.resultColumns.begin(), otherSqlRow.resultColumns.end(), inserter(intersect, intersect.begin()));
		size_t thisHashResult = 0;
		size_t otherHashResult = 0;
		for (int i = 0; i < intersect.size(); i++){
			string col = intersect.at(i);
			thisHashResult = thisHashResult ^ hash<string>{}(row.at(col));
			otherHashResult = otherHashResult ^ hash<string>{}(otherSqlRow.row.at(col));
		}
		return thisHashResult < otherHashResult;
	}

	bool operator==(const SqlResult& otherSqlRow) const
	{
		vector<string> intersect;
		set_intersection(resultColumns.begin(), resultColumns.end(), otherSqlRow.resultColumns.begin(), otherSqlRow.resultColumns.end(), inserter(intersect, intersect.begin()));
		size_t thisHashResult = 0;
		size_t otherHashResult = 0;
		for (int i = 0; i < intersect.size(); i++) {
			string col = intersect.at(i);
			thisHashResult = thisHashResult ^ hash<string>{}(row.at(col));
			otherHashResult = otherHashResult ^ hash<string>{}(otherSqlRow.row.at(col));
		}
		return thisHashResult == otherHashResult;
	}
	
	
	string getColumnValue(string column);
};

struct SuchThat {
	string relationship = "";
	string first = "";
	string second = "";
};

struct Pattern {
	string synonym = "";
	string first = "";
	string second = "";
};

struct Select {
	vector<string> synonym;
	vector<string> tableSql;
	vector<string> whereSql;
	vector<string> columnSql;
	vector<string> asSql;
    //SqlResultSet sqlResultSet;
};