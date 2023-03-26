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


struct SqlResult {
	map<string, string> row; // store mapping of column name to value	
};

struct SqlResultStore {
	vector<SqlResult> sqlResult;
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

