#pragma once
#include <string>
#include "DescriberEntStmt.h"

using namespace std;

// contains builder functions to insert abstractions into the database
class BuilderSqlInsertStmt {
private:
	// method to insert a statement into the database
	string insertStatement(int stmtNum, string entity, string text);

	// method to insert a variable into the database
	string insertVariable(string variablename, int statementNumber);

	// method to insert a constant into the database
	string insertConstant(string value);

	//method to insert pattern into the database
	string insertPattern(int stmtNum, string LHS, string RHS, string expression);

public:
	BuilderSqlInsertStmt(Statement stmt, DescriberEntStmt describer);
};