#include "Statement.h"

Statement::Statement(int stmtNum, string stmt, int stmtNumSubtract) {
	_stmtNum = stmtNum;
	_stmt = stmt;
	_stmtNumSubtract = stmtNumSubtract;
}

Statement::Statement(int stmtNum, string stmt, IContainer* container, int stmtNumSubtract) {
	_stmtNum = stmtNum;
	_stmt = stmt;
	_stmtNumSubtract = stmtNumSubtract;
	_container = container;
}

Statement::Statement(int stmtNum, int level, IContainer* container, int stmtNumSubtract) {
	_stmtNum = stmtNum;
	_level = level;
	_container = container;
	_stmtNumSubtract = stmtNumSubtract;
}

Statement::Statement(int stmtNum, string stmt, int level, IContainer* container, int stmtNumSubtract) {
	_stmtNum = stmtNum;
	_stmt = stmt;
	_level = level;
	_stmtNumSubtract = stmtNumSubtract;
	_container = container;
}

void Statement::AddToken(string tk)
{
	_tokens.push_back(tk);
}

vector<string> Statement::GetToken()
{
	return _tokens;
}

int Statement::GetAdjustedStmtNum() {
	return _stmtNum - _stmtNumSubtract;
}

int Statement::GetStmtNum() {
	return _stmtNum;
}

void Statement::AddStmt(string s) {
	_stmt += s;
}

string Statement::GetStmt() {
	return _stmt;
}

int Statement::GetLevel() {
	return _level;
}

void Statement::SetEntity(string entity) {
	_entity = entity;
}

string Statement::GetEntity() {
	return _entity;
}