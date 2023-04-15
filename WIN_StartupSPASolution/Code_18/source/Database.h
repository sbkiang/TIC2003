#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"
#include <format>
#include "../SPA/Struct.h"
#include "../SPA/define.h"

#include <set>

using namespace std;

// The Database has to be a static class due to various constraints.
// It is advisable to just add the insert / get functions based on the given examples.
class Database {
public:
	// method to connect to the database and initialize tables in the database
	static void initialize();

	// method to close the database connection
	static void close();

	static bool ExecuteSql(string sql, SqlResultStore& rs);

	// method to insert a procedure into the database
	static void insertProcedure(string procedureName, int start, int end);

	// method to insert a statement into the database
	static void insertStatement(int stmtNum, string entity, string text);

	// method to insert a variable into the database
	static void insertVariable(string variablename, int statementNumber);

	// method to insert a constant into the database
	static void insertConstant(string value);

	// method to insert a parent into the database
	static void insertParent(int parentStmt, int childStart, int childEnd);
	
	// method to insert/get a modify into the database
	static void insertModifies(int stmtNum, string variablename);

	// method to insert/get a use into the database
	static void insertUses(int stmtNum, string variablename);

	// method to insert/get a call into the database
	static void insertCall(string caller, string callee);

	// isSpecific is false if the input is not part of select synonym, and can be found on synonymEntityMap

	// method to insert/get a next into the database
	static void insertNext(int from, int to);

	//method to insert pattern into the database
	static void insertPattern(int stmtNum, string LHS, string RHS, string expression);

private:
	// the connection pointer to the database
	static sqlite3* dbConnection; 
	// a vector containing the results from the database
	static vector<vector<string>> dbResults; 
	// the error message from the database
	static char* errorMessage;
	// callback method to put one row of results from the database into the dbResults vector
	// This method is called each time a row of results is returned from the database	
	static int callback(void* NotUsed, int columnCount, char** columnValues, char** columnNames);
};

