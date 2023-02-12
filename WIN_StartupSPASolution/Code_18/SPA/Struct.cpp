#include "Struct.h"

using namespace std;

Statement::Statement(int stmtNum, int level, Container* container) {
	_stmtNum = stmtNum;
	_level = level;
	_container = container;
}

Statement::Statement(int stmtNum, string token) {
	_stmtNum = stmtNum;
	_level = 0;
}

Statement::Statement(int stmtNum, string token, Container* container) {
	_stmtNum = stmtNum;
	_level = 0;
	_container = container;
}

Statement::Statement(int stmtNum, string stmt, int level, Container* container) {
	_stmtNum = stmtNum;
	_stmt = stmt;
	_level = level;
	_container = container;
}

int Statement::getFinalStmtNum() {
	return _stmtNum - _elseOffset;
}
