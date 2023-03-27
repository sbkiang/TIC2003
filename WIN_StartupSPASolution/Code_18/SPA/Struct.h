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
	map<string, int> columnTypeIndex; // store the index of each column type
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

static void SqlResultStoreCompare(SqlResultStore& rsMain, SqlResultStore& rs2) {
	// for each col in rsMain, find the column of correct type in rs2, and compare
	string synonym = "";
	vector<int> rs2ColIndex; // find column index in rs2 where entity type is the same as rsMain at vector index
	for (auto it = rsMain.columnTypeIndex.begin(); it != rsMain.columnTypeIndex.end(); it++) {
		auto itFind = rs2.columnTypeIndex.find(it->first);
		if (itFind != rs2.columnTypeIndex.end()) {
			rs2ColIndex.push_back(itFind->second);
		}
		else {
			rs2ColIndex.push_back(-1);
		}
	}
	vector<SqlResult> resPass;
	int resMainColIndex = 0;
	for (SqlResult resMain : rsMain.sqlResult) { // loop thru each result rsMain
		for (auto resMainIt = resMain.row.begin(); resMainIt != resMain.row.end(); resMainIt++) { // loop thru each row
			for (SqlResult res2 : rs2.sqlResult) { // loop thru each result in rs2
				auto Res2It = res2.row.begin();
				if (rs2ColIndex.at(resMainColIndex) == -1) { // means illogical query. We just check if result exists
					if (!rs2.sqlResult.empty()) {
						resPass.push_back(resMain);
					}
				}
				else { // means logical query
					for (int i = 0; i < rs2ColIndex.at(resMainColIndex); i++) { Res2It++; }
					if (Res2It->second == resMainIt->second) {
						resPass.push_back(resMain);
					}
				}
			}
			resMainColIndex++;
		}
	}
	rsMain.sqlResult = resPass;
}