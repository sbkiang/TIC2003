#pragma once
#include <string>
#include "DescriberSIMPLE.h"

using namespace std;

// contains builder functions to insert abstractions into the database
class BuilderSqlInsertSIMPLE {
private:
	// method to insert a procedure into the database
	string insertProcedure(string procedureName, int start, int end);

	// method to insert a statement into the database
	string insertStatement(int stmtNum, string entity, string text);

	// method to insert a variable into the database
	string insertVariable(string variablename, int statementNumber);

	// method to insert a constant into the database
	string insertConstant(string value);

	// method to insert a parent into the database
	string insertParent(int parentStmt, int childStart, int childEnd);

	// method to insert/get a modify into the database
	string insertModifies(int stmtNum, string variablename);

	// method to insert/get a use into the database
	string insertUses(int stmtNum, string variablename);

	// method to insert/get a call into the database
	string insertCall(string caller, string callee, int directcall);

	// method to insert/get a next into the database
	string insertNext(int from, int to);

	//method to insert pattern into the database
	string insertPattern(int stmtNum, string LHS, string RHS, string expression);
};