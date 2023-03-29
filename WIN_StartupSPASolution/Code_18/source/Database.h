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
	//static bool GetParent(string stmtNum1, string stmtNum2, bool input1IsSpecific, bool input2IsSpecific, entity);

	static string GetParentConstruct_Synonym_Synonym(string input1, string input2);
	static string GetParentConstruct_Synonym_NotSynonym(string input1);
	static string GetParentConstruct_NotSynonym_Synonym(string input2);
	static string GetParentConstruct_NotSynonym_NotSynonym();

	static string GetDirectParent(string input1);

	static bool GetParent(string stmtNum1, string stmtNum2, bool input1IsSpecific, bool input2IsSpecific, string parentEntity, string childEntity, SqlResultStore& rs);
	static bool GetParentAnyAny(string frontSql, SqlResultStore& rs); // Parent(stmt/_ , stmt/_) 
	static bool GetParentAnySynonym(string frontSql, string input2, SqlResultStore& rs); // Parent(stmt/_, read/print/assign/while/if/call)
	static bool GetParentAnySpecific(string frontSql, string input2, SqlResultStore& rs); // Parent(stmt/_, 10)
	static bool GetParentSynonymAny(string frontSql, string input1, SqlResultStore& rs); // Parent(while/if, stmt/_)
	static bool GetParentSynonymSynonym(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(while/if, read/print/assign/while/if/call)
	static bool GetParentSynonymSpecific(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(while/if, 10)
	static bool GetParentSpecificAny(string frontSql, string input1, SqlResultStore& rs); // Parent(10, stmt/_)
	static bool GetParentSpecificSynonym(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(10, read/print/assign/while/if/call)
	static bool GetParentSpecificSpecific(string frontSql, string input1, string input2, SqlResultStore& rs); // // Parent(10, 10)
	
	static bool GetParentT(string stmtNum1, string stmtNum2, bool input1IsSpecific, bool input2IsSpecific, string parentEntity, string childEntity, SqlResultStore& rs);	static bool GetParentAnyAnyT(string frontSql, SqlResultStore& rs); // Parent(stmt/_ , stmt/_) 
	static bool GetParentTAnyAny(string frontSql, SqlResultStore& rs); // Parent(stmt/_ , stmt/_) 
	static bool GetParentTAnySynonym(string frontSql, string input2, SqlResultStore& rs); // Parent(stmt/_, read/print/assign/while/if/call)
	static bool GetParentTAnySpecific(string frontSql, string input2, SqlResultStore& rs); // Parent(stmt/_, 10)
	static bool GetParentTSynonymAny(string frontSql, string input1, SqlResultStore& rs); // Parent(while/if, stmt/_)
	static bool GetParentTSynonymSynonym(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(while/if, read/print/assign/while/if/call)
	static bool GetParentTSynonymSpecific(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(while/if, 10)
	static bool GetParentTSpecificAny(string frontSql, string input1, SqlResultStore& rs); // Parent(10, stmt/_)
	static bool GetParentTSpecificSynonym(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(10, read/print/assign/while/if/call)
	static bool GetParentTSpecificSpecific(string frontSql, string input1, string input2, SqlResultStore& rs); // // Parent(10, 10)
	
	// method to insert/get a modify into the database
	static void insertModifies(int stmtNum, string variablename);

	// method to insert/get a use into the database
	static void insertUses(int stmtNum, string variablename);

	// isSpecific is false if the input is not part of select synonym, and can be found on synonymEntityMap
	// need select distinct both Uses()
	static string GetUsesConstruct_StatementSynonym_Synonym(string input1, string input2);
	static string GetUsesConstruct_StatementSynonym_NotSynonym(string input1);
	static string GetUsesConstruct_NameSynonym_Synonym(string input1, string input2);
	static string GetUsesConstruct_NameSynonym_NotSynonym(string input1);

	static string GetUsesConstruct_StatementNotSynonym_Synonym(string input2);
	static string GetUsesConstruct_StatementNotSynonym_NotSynonym();
	static string GetUsesConstruct_NameNotSynonym_Synonym(string input2);
	static string GetUsesConstruct_NameNotSynonym_NotSynonym();

	static bool GetUsesForAssign(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);

	// Uses(print/assign, var/_)
	static bool GetUses_AnyPrintAssign_Any(string frontSql, string input1, SqlResultStore& rs); 

	// Uses(call, var/_)
	static bool GetUses_AnyCall_Any(string frontSql, SqlResultStore& rs); 

	// Uses(procedure, var/_)
	static bool GetUses_AnyProcedure_Any(string frontSql, SqlResultStore& rs); 

	// Uses(while/if, var/_)
	static bool GetUses_AnyWhileIf_Any(string frontSql, string input1, SqlResultStore& rs); 

	// Uses(print/assign, "x")
	static bool GetUses_AnyPrintAssign_Specific(string frontSql, string input1, string input2, SqlResultStore& rs); 

	// Uses(call, "x")
	static bool GetUses_AnyCall_Specific(string frontSql, string input2, SqlResultStore& rs); 

	// Uses(procedure, "x")
	static bool GetUses_AnyProcedure_Specific(string frontSql, string input2, SqlResultStore& rs); 

	// Uses(while/if, "x")
	static bool GetUses_AnyWhileIf_Specific(string frontSql, string input1, string input2, SqlResultStore& rs); 

	// Uses(10, var/_) where 10 = print/assign
	static bool GetUses_SpecificPrintAssign_Any(string frontSql, string input1, SqlResultStore& rs); 

	// Uses(10, var/_) where 10 = call
	static bool GetUses_SpecificCall_Any(string frontSql, string input1, SqlResultStore& rs); 

	// Uses("main", var/_)
	static bool GetUses_SpecificProcedure_Any(string frontSql, string input1, SqlResultStore& rs); 

	// Uses("10", var/_) where 10 = while/if
	static bool GetUses_SpecificWhileIf_Any(string frontSql, string input1, SqlResultStore& rs);

	// Uses(10, "x") where 10 = print/assign
	static bool GetUses_SpecificPrintAssign_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	// Uses(10, var/_) where 10 = call
	static bool GetUses_SpecificCall_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	// Uses("main", var/_)
	static bool GetUses_SpecificProcedure_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	// Uses("10", var/_) where 10 = while/if
	static bool GetUses_SpecificWhileIf_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	static bool GetUsesForPrint(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);

	static bool GetUsesForWhile(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);

	static bool GetUsesForIf(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);

	static bool GetUsesForCall(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);

	static bool GetUsesForProcedure(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);

	static bool GetUsesForUnknownInput1(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
 // E.g., Uses(10,v). We don't know what's the entity at statement 10

	static string GetModifiesConstruct(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
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

