#pragma once
#include "Struct.h"

using namespace std;

Statement::Statement(int stmtNum) {
	_stmtNum = stmtNum;
}

Statement::Statement(int stmtNum, int level) {
	_stmtNum = stmtNum;
	_level = level;
}

Statement::Statement(int stmtNum, string stmt, int level) {
	_stmtNum = stmtNum;
	_stmt = stmt;
	_level = level;
}