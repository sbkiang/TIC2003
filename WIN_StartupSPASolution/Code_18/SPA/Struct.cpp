#include "Struct.h"

using namespace std;

Statement::Statement(int stmtNum, string stmt, int stmtNumSubtract) {
	_stmtNum = stmtNum;
	_stmt = stmt;
	_stmtNumSubtract = stmtNumSubtract;
}

Statement::Statement(int stmtNum, int level, Container* container, int stmtNumSubtract) {
	_stmtNum = stmtNum;
	_level = level;
	_container = container;
	_stmtNumSubtract = stmtNumSubtract;
}

Statement::Statement(int stmtNum, string stmt, Container* container, int stmtNumSubtract) {
	_stmtNum = stmtNum;
	_stmt = stmt;
	_level = 0;
	_container = container;
	_stmtNumSubtract = stmtNumSubtract;
}

Statement::Statement(int stmtNum, string stmt, int level, Container* container, int stmtNumSubtract) {
	_stmtNum = stmtNum;
	_stmt = stmt;
	_level = level;
	_container = container;
	_stmtNumSubtract = stmtNumSubtract;
}

int Statement::getAdjustedStmtNum() {
	return _stmtNum - _stmtNumSubtract;
}

string SqlResult::getColumnValue(string col) {
	return row.at(col);
}

vector<string> SqlResultSet::getColumnValues(string col) {
	vector<string> result;
	if (sqlResult.empty()) {
		return result;
	}
	for (auto it = sqlResult.begin(); it != sqlResult.end(); it++) {
		if ((*it)->getColumnValue(col) == "") { break; }
		result.push_back((*it)->getColumnValue(col));
	}
	return result;
}

vector<string> SqlResultSet::getColumnValues2(string col) {
	vector<string> result;
	if (sqlResultSet.empty()) {
		return result;
	}
	for (auto it = sqlResultSet.begin(); it != sqlResultSet.end(); it++) {
		if ((*it)->row.find(col) == (*it)->row.end()) { break; } // can't find the col in the result
		result.push_back((*it)->row.at(col));
	}
	return result;
}