#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"
#include <format>

using namespace std;

// The Database has to be a static class due to various constraints.
// It is advisable to just add the insert / get functions based on the given examples.
class Database {
public:
	// method to connect to the database and initialize tables in the database
	static void initialize();

	// method to close the database connection
	static void close();

	static void insertProcedure(string procedureName, int start, int end);
	static void getProcedures(vector<string>& results);

	static void insertStatement(int statementNumber, string statementName, string statementType, string text);
	static void getStatement(string type, vector<string>& results);
	static void getStmt(string type, vector<string>& results);

	static void insertVariable(string variablename, int statementNumber);
	static void getVariable(vector<string>& results);

	// method to insert a constant into the database
	static void insertConstant(string value);
	static void getConstant(vector<string>& results);

	// method to insert a parent into the database
	static void insertParent(int parentStmt, int childStart, int childEnd);
	static void getParent(string stmtNum1, string stmtNum2, vector<string>& results);
	static void getChildren(string stmtNum1, string stmtNum2, string statementType, vector<string>& results);
	static void getParentChildren(bool findparent, string resultType, string filterType, vector<string>& results);
	
	// method to insert/get a modify into the database
	static void insertModifies(int statementNumber, string procedureName, string variablename);

	// method to insert/get a use into the database
	static void getUseStmt(string stmtNum1, string stmtNum2, bool lhs, vector<string>& results);
	static void getUseEnt(string stmtNum1, string stmtNum2, vector<string>& results);

	// method to insert a use into the database
	static void insertUse(int statementNumber, string procedureName, string variablename);
	static void getUse(string entity, string variable, vector<string>& results); // for "assign a" and "print pn"

	static void insertNext(int stmtNum1, int stmtNum2);
	static void getNext(int stmtNum1, int stmtNum2, vector<string>& results);
	static void getNext_T(int stmtNum1, int stmtNum2, vector<string>& results);

private:
	// the connection pointer to the database
	static sqlite3* dbConnection; 
	// a vector containing the results from the database
	static vector<vector<string>> dbResults; 
	// the error message from the database
	static char* errorMessage;
	// callback method to put one row of results from the database into the dbResults vector
	// This method is called each time a row of results is returned from the database
	static int callback(void* NotUsed, int argc, char** argv, char** azColName);
};

