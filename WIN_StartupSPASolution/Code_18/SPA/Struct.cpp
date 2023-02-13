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

Statement::Statement(int stmtNum, string stmt, int stmtNumSubtract) {
	_stmtNum = stmtNum;
	_stmt = stmt;
	_stmtNumSubtract = stmtNumSubtract;
}

int Statement::getAdjustedStmtNum() {
	return _stmtNum - _stmtNumSubtract;
}