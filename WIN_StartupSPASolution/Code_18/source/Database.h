#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"
#include <format>
#include "../SPA/Struct.h"

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
	static void getStatement(vector<string>& results);
	static void getStmt(string type, vector<string>& results);

	static void insertVariable(string variablename, int statementNumber);
	static void getVariable(vector<string>& results);

	// method to insert a constant into the database
	static void insertConstant(string value);
	static void getConstant(vector<string>& results);

	// method to insert a parent into the database
	static void insertParent(int parentStmt, int childStart, int childEnd);
	static void getParent(string stmtNum1, string stmtNum2, vector<string>& results);
	static bool getParent(string input1, string input2); // for Parent(x,y), specific is number. non-specific is synonym
	static void getChildren(string stmtNum1, string stmtNum2, string statementType, vector<string>& results);
	static void getParentChildren(bool findparent, string resultType, string filterType, vector<string>& results);
	
	// method to insert/get a modify into the database
	static void insertModifies(int statementNumber, string procedureName, string variablename);
	static void getModifyStmt(string stmtNum1, string stmtNum2, bool lhs, vector<string>& results);

	// method to insert/get a use into the database
	static void insertUses(int statementNumber, string procedureName, string variablename);

	// for cases such as uses(<entity>,v) where <entity> = if,while,procedure,call,assign and v = variable v. Return type depends on entity
	static void getUses(string entity, vector<string>& results); 
	static void getUses(string entity, SqlResult& sqlRowSet);
	static void getUses(string entity, SqlResult* sqlRowSet);

	// for cases such as uses(<entity>,v) where <entity> = if,while,procedure,call,assign and v = named variable. Return type depends on entity, filtered to named variable
	static void getUses(string entity, string variable, vector<string>& results);

	// for cases such as uses(s,v) where s = stmt num and v = variable v. Return all variables in the specified stmt num
	static void getUses(int stmtNum, vector<string>& results); 

	// for cases such as uses(s,v) where s = stmt num and v = named variable. Return stmt num that has uses(s,"named_var")
	static void getUses(int stmtNum, string variable, vector<string>& results);


	// isSpecific is false if the input is not part of select synonym, and can be found on synonymEntityMap
	static bool GetUsesForWhileIf(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& sqlResultStore, map<string, string> synEntMap);
	static bool GetUsesForCall(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetUsesForAssign(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetUsesForPrint(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetUsesForWhile(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetUsesForIf(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetUsesForProcedureCall(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetUsesForUnknownInput1(string input1, string input2, bool input2IsSpecific); // E.g., Uses(10,v). We don't know what's the entity at statement 10


	// method to insert/get a next into the database
	static void insertNext(int stmtNum1, int stmtNum2);
	static void getNext(int stmtNum1, int stmtNum2, vector<string>& results);
	static void getNext_T(int stmtNum1, int stmtNum2, vector<string>& results);

	//static void select(Select& st, SqlResultStore* sqlResultStore);
	//static void select(Select& st, SqlResultStore* sqlResultStore, map<string,string> synEntMap);
	static void suchThat(SuchThat& st, SqlResultStore* sqlResultStore);
	static void SelectPql(Select& st, SqlResultStore& sqlResultStore);

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
	static int callback(void* NotUsed, int argc, char** argv, char** azColName, SqlResult& sqlResult);
	static int callback_new(void* NotUsed, int argc, char** argv, char** azColName);
};

