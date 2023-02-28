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

	// method to insert a procedure into the database
	static void insertProcedure(string procedureName, int start, int end);
	static void getProcedures(vector<string>& results);

	// method to insert a statement into the database
	static void insertStatement(int stmtNum, string statementName, string statementType, string text);
	static void getStatement(string type, vector<string>& results);
	static void getStatement(vector<string>& results);
	static void getStmt(string type, vector<string>& results);

	// method to insert a variable into the database
	static void insertVariable(string variablename, int statementNumber);
	static void getVariable(vector<string>& results);

	// method to insert a constant into the database
	static void insertConstant(string value);
	static void getConstant(vector<string>& results);

	// method to insert a parent into the database
	static void insertParent(int parentStmt, int childStart, int childEnd);
	//static bool GetParent(string stmtNum1, string stmtNum2, bool input1IsSpecific, bool input2IsSpecific, entity);
	static bool GetParent(string stmtNum1, string stmtNum2, bool input1IsSpecific, bool input2IsSpecific, string parentEntity, string childEntity);
	static bool GetParentForStmt(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetParentT(string input1, string input2); // for Parent(x,y), specific is number. non-specific is synonym
	static bool GetParentTForStmt(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, string parentEntity, string childEntity);
	
	// method to insert/get a modify into the database
	static void insertModifies(int stmtNum, string variablename);
	static void getModifyStmt(string stmtNum1, string stmtNum2, bool lhs, vector<string>& results);

	// method to insert/get a use into the database
	static void insertUses(int stmtNum, string variablename);

	// isSpecific is false if the input is not part of select synonym, and can be found on synonymEntityMap
	static bool GetUsesForAssign(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetUsesForPrint(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetUsesForWhile(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetUsesForIf(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetUsesForCall(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetUsesForProcedure(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetUsesForUnknownInput1(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific); // E.g., Uses(10,v). We don't know what's the entity at statement 10

	static bool GetModifiesForAssign(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetModifiesForRead(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetModifiesForWhile(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetModifiesForIf(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetModifiesForCall(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetModifiesForProcedure(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool GetModifiesForUnknownInput1(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific); // E.g., Uses(10,v). We don't know what's the entity at statement 10


	// method to insert/get a next into the database
	static void insertNext(int stmtNum1, int stmtNum2);
	static void getNext(int stmtNum1, int stmtNum2, vector<string>& results);
	static void getNextT(int stmtNum1, int stmtNum2, vector<string>& results);

	//method to insert pattern into the database
	static void insertPattern(int stmtNum, string LHS, string RHS, string experssion);


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
	static int callback(void* NotUsed, int columnCount, char** columnValues, char** columnNames);
};

