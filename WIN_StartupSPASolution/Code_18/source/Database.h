#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"

using namespace std;

// The Database has to be a static class due to various constraints.
// It is advisable to just add the insert / get functions based on the given examples.
class Database {
public:
	// method to connect to the database and initialize tables in the database
	static void initialize();

	// method to close the database connection
	static void close();



	// method to insert a procedure into the database
	static void insertProcedure(string procedureName);

	// method to insert a statement into the database
	static void insertStatement(int statementNumber, string statementName, string statementType, string text);

	// method to insert a variable into the database
	static void insertVariable(string variablename, int statementNumber);

	// method to insert a constant into the database
	static void insertConstant(string value);

	// method to insert a parent into the database
	static void insertParent(int parentNumber, int childNumber, bool direct);

	// method to insert a modify into the database
	static void insertModify(int statementNumber, string procedureName, string variablename);

	// method to insert a use into the database
	static void insertUse(int statementNumber, string procedureName, string variablename);

	// method to get all the procedures from the database
	static void getProcedures(vector<string>& results);

	// method to get all the statement from the database
	static void getStatement(string type, vector<string>& results);

	// method to get all variable name from the database
	static void getVariable(vector<string>& results);

	// method to get statements from the database
	static void getStmt(string type, vector<string>& results);

	// method to get constant from the database
	static void getConstant(vector<string>& results);

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

