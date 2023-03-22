#include "Statement.h"

Statement::Statement(int stmtNum, string stmt, int stmtNumSubtract) {
	_stmtNum = stmtNum;
	_stmt = stmt;
	_stmtNumSubtract = stmtNumSubtract;
}

Statement::Statement(int stmtNum, string stmt, Container* container, int stmtNumSubtract) {
	_stmtNum = stmtNum;
	_stmt = stmt;
	_stmtNumSubtract = stmtNumSubtract;
	_container = container;
}

Statement::Statement(int stmtNum, int level, Container* container, int stmtNumSubtract) {
	_stmtNum = stmtNum;
	_level = level;
	_container = container;
	_stmtNumSubtract = stmtNumSubtract;
}

Statement::Statement(int stmtNum, string stmt, int level, Container* container, int stmtNumSubtract) {
	_stmtNum = stmtNum;
	_stmt = stmt;
	_level = level;
	_stmtNumSubtract = stmtNumSubtract;
	_container = container;
}

Statement::~Statement() {}

int Statement::getAdjustedStmtNum() {
	return _stmtNum - _stmtNumSubtract;
}

int Statement::getStmtNum() {
	return _stmtNum;
}

void Statement::appendStmt(string s) {
	_stmt += s;
}

string Statement::getStmt() {
	return _stmt;
}

int Statement::getLevel() {
	return _level;
}

void Statement::setEntity(string entity) {
	_entity = entity;
}

string Statement::getEntity() {
	return _entity;
}