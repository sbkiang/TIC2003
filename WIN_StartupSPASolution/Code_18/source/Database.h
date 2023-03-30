#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"
#include <format>
#include "../SPA/Struct.h"
#include "../SPA/define.h"
#include "../SPA/Rel_Modifies.h"
#include "../SPA/Rel_Uses.h"
#include "../SPA/Rel_Parent.h"

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
	static void GetProcedures(vector<string>& results);

	// method to insert a statement into the database
	static void insertStatement(int stmtNum, string entity, string text);

	// method to insert a variable into the database
	static void insertVariable(string variablename, int statementNumber);
	static void GetVariable(set<string>& result);
	// method to insert a constant into the database
	static void insertConstant(string value);
	static void GetConstant(vector<string>& results);

	// method to insert a parent into the database
	static void insertParent(int parentStmt, int childStart, int childEnd);

	static string GetDirectParent(string input1);

	static bool GetParent(string stmtNum1, string stmtNum2, bool input1IsSpecific, bool input2IsSpecific, string parentEntity, string childEntity, SqlResultStore& rs);
	
	static bool GetParentT(string stmtNum1, string stmtNum2, bool input1IsSpecific, bool input2IsSpecific, string parentEntity, string childEntity, SqlResultStore& rs);	static bool GetParentAnyAnyT(string frontSql, SqlResultStore& rs); // Parent(stmt/_ , stmt/_) 
	
	// method to insert/get a modify into the database
	static void insertModifies(int stmtNum, string variablename);

	// method to insert/get a use into the database
	static void insertUses(int stmtNum, string variablename);

	// isSpecific is false if the input is not part of select synonym, and can be found on synonymEntityMap

	static bool GetUsesForAssign(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);

	static bool GetUsesForPrint(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);

	static bool GetUsesForWhile(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);

	static bool GetUsesForIf(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);

	static bool GetUsesForCall(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);

	static bool GetUsesForProcedure(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);

	static bool GetUsesForUnknownInput1(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
 // E.g., Uses(10,v). We don't know what's the entity at statement 10

	static bool GetModifiesForAssign(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
	static bool GetModifiesForRead(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
	static bool GetModifiesForWhile(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
	static bool GetModifiesForIf(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
	static bool GetModifiesForCall(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
	static bool GetModifiesForProcedure(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
	static bool GetModifiesForUnknownInput1(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs); // E.g., Uses(10,v). We don't know what's the entity at statement 10

	// method to insert/get a next into the database
	static void insertNext(int stmtNum1, int stmtNum2);
	static bool GetNext(int stmtNum1, int stmtNum2);
	static bool GetNextT(int stmtNum1, int stmtNum2);

	//method to insert pattern into the database
	static void insertPattern(int stmtNum, string LHS, string RHS, string expression);
	static void GetPatternLike(string input1, string input2, SqlResultStore& rs);
	static void GetPatternIn(string input1, string input2, SqlResultStore& rs);

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

