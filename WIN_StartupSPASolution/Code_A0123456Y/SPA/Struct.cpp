#pragma once
#include "Struct.h"

using namespace std;

Statement::Statement(int stmtNum) {
	_stmtNum = stmtNum;
}

Statement::Statement(int stmtNum, string stmt) {
	_stmtNum = stmtNum;
	_stmt = stmt;
}