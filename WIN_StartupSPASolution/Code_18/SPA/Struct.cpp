#include "Struct.h"

using namespace std;

<<<<<<< Updated upstream
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
=======
Statement::Statement(int stmtNum, int level, Container* container, int cfgStmtNumAdd) {
	_stmtNum = stmtNum;
	_level = level;
	_container = container;
	_cfgStmtNum = stmtNum + cfgStmtNumAdd;

}

Statement::Statement(int stmtNum, string token, Container* container, int cfgStmtNumAdd) {
	_stmtNum = stmtNum;
	_level = 0;
	_container = container;
	_cfgStmtNum = stmtNum + cfgStmtNumAdd;
}

Statement::Statement(int stmtNum, string stmt, int level, Container* container, int cfgStmtNumAdd) {
>>>>>>> Stashed changes
	_stmtNum = stmtNum;
	_stmt = stmt;
	_level = level;
	_container = container;
<<<<<<< Updated upstream
}

int Statement::getFinalStmtNum() {
	return _stmtNum - _elseOffset;
}
=======
	_cfgStmtNum = stmtNum + cfgStmtNumAdd;
}

int Statement::getAdjustedStmtNum() {
	return _stmtNum - _cfgStmtNum;
}
>>>>>>> Stashed changes
