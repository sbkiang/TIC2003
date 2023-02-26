#include "Database.h"
#include <iostream>
#include <regex>


sqlite3* Database::dbConnection;
vector<vector<string>> Database::dbResults;
char* Database::errorMessage;
SqlResultStore* sqlResultStoreForCallback;
string regexDigit = "^\\d+";

// method to connect to the database and initialize tables in the database
void Database::initialize() {
	// open a database connection and store the pointer into dbConnection
	sqlite3_open("database.db", &dbConnection);

	// drop the existing procedure table (if any)
	string dropProcedureTableSQL = "DROP TABLE IF EXISTS procedure";
	sqlite3_exec(dbConnection, dropProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a procedure table
	string createProcedureTableSQL = "CREATE TABLE procedure ( name VARCHAR(50) PRIMARY KEY, start INT, end INT);";
	sqlite3_exec(dbConnection, createProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing stmt table (if any)
	string dropStatementTableSQL = "DROP TABLE IF EXISTS statement";
	sqlite3_exec(dbConnection, dropStatementTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a statement table
	string createStatementTableSQL = "CREATE TABLE statement ( line_num INT PRIMARY KEY, procedure_name VARCHAR(255) REFERENCES procedure(name), entity VARCHAR(255), text VARCHAR(255));";
	sqlite3_exec(dbConnection, createStatementTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing variable table (if any)
	string dropVariableTableSQL = "DROP TABLE IF EXISTS variable";
	sqlite3_exec(dbConnection, dropVariableTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a variable table
	string createVariableTableSQL = "CREATE TABLE variable ( name VARCHAR(20) PRIMARY KEY, line_num INT REFERENCES statement(line_num) );";
	sqlite3_exec(dbConnection, createVariableTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing constant table (if any)
	string dropConstantTableSQL = "DROP TABLE IF EXISTS constant";
	sqlite3_exec(dbConnection, dropConstantTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a constant table
	string createConstantTableSQL = "CREATE TABLE constant ( value VARCHAR(20) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createConstantTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing parent table (if any)
	string dropParentTableSQL = "DROP TABLE IF EXISTS parent";
	sqlite3_exec(dbConnection, dropParentTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a parent table
	string createParentTableSQL = "CREATE TABLE parent ( line_num INT REFERENCES statement(line_num), child_start INT REFERENCES statement(line_num), child_end INT REFERENCES statement(line_num));";
	sqlite3_exec(dbConnection, createParentTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing modify table (if any)
	string dropModifyTableSQL = "DROP TABLE IF EXISTS modify";
	sqlite3_exec(dbConnection, dropModifyTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a modify table
	string createModifyTableSQL = "CREATE TABLE modify ( line_num INT REFERENCES statement(line_num), procedure_name VARCHAR(50) REFERENCES procedure(name), variable_name VARCHAR(50) REFERENCES variable(name));";
	sqlite3_exec(dbConnection, createModifyTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing use table (if any)
	string dropUseTableSQL = "DROP TABLE IF EXISTS use";
	sqlite3_exec(dbConnection, dropUseTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a use table
	string createUseTableSQL = "CREATE TABLE use ( line_num INT REFERENCES statement(line_num), procedure_name VARCHAR(50) REFERENCES procedure(name), variable_name VARCHAR(50) REFERENCES variable(name), PRIMARY KEY(line_num, procedure_name, variable_name));";
	sqlite3_exec(dbConnection, createUseTableSQL.c_str(), NULL, 0, &errorMessage);

	string dropNextTableSQL = "DROP TABLE IF EXISTS next";
	sqlite3_exec(dbConnection, dropNextTableSQL.c_str(), NULL, 0, &errorMessage);
	string createNextTableSQL = "CREATE TABLE next (line_num_1 INT REFERENCES statement(line_num), line_num_2 INT REFERENCES statement(line_num), CONSTRAINT line_num_not_equal check(line_num_1 <> line_num_2));";
	sqlite3_exec(dbConnection, createNextTableSQL.c_str(), NULL, 0, &errorMessage);
	if (errorMessage) {
		cout << errorMessage << endl;
	}

	// initialize the result vector
	dbResults = vector<vector<string>>();
}

// method to close the database connection
void Database::close() {
	sqlite3_close(dbConnection);
}

// method to insert a procedure into the database
void Database::insertProcedure(string procedureName, int start, int end) {
	char sqlBuf[256];
	sprintf(sqlBuf, "INSERT INTO procedure ('name','start','end') VALUES ('%s',%i,%i);", procedureName.c_str(), start, end);
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertProcedure SQL Error: " << errorMessage << endl; }
}

// method to insert a statement into the database
void Database::insertStatement(int stmtNum, string stmtName, string entity, string text) {
	char sqlBuf[256];
	sprintf(sqlBuf, "INSERT INTO statement ('line_num','procedure_name','entity','text') VALUES ('%i','%s','%s','%s');", stmtNum, stmtName.c_str(), entity.c_str(), text.c_str());
	//string sql = "INSERT INTO statement ('line_num', 'procedure_name', 'entity', 'text') VALUES ('" + to_string(statementNumber) + "', '" + statementName + "', '" + entity + "', '" + text + "');";
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertStatement SQL Error: " << errorMessage << endl; }
}

// method to insert a statement into the database
void Database::insertVariable(string stmtName, int stmtNum) {
	char sqlBuf[256];
	sprintf(sqlBuf, "INSERT INTO variable('name','line_num') VALUES('%s','%i');", stmtName.c_str(), stmtNum);
	//string sql = "INSERT INTO variable ('name', 'line_num') VALUES ('" + statementName + "', '" + to_string(statementNumber) + "');";
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	char sqlError[256];
	sprintf_s(sqlError, "insertVariable SQL Error for %s at stmtNum %i: ", stmtName.c_str(), stmtNum);
	if (errorMessage) { cout << sqlError << errorMessage << endl; }
}

void Database::insertConstant(string value) {
	char sqlBuf[256];
	sprintf(sqlBuf, "INSERT INTO constant ('value') VALUES ('%s');", value.c_str());
	//string sql = "INSERT INTO constant ('value') VALUES ('" + value + "');";
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	//if (errorMessage) { cout << "insertConstant SQL Error: " << errorMessage << endl; }
}

void Database::insertParent(int parent, int child_start, int child_end) {
	char sqlBuf[256];
	sprintf(sqlBuf, "INSERT INTO parent ('line_num','child_start','child_end') VALUES (%i,%i,%i);", parent, child_start, child_end);
	//string sql = "INSERT INTO parent ('line_num', 'child_line', 'direct_child' ) VALUES ('" + to_string(parent) + "', '" + to_string(child) + "', '" + to_string(direct) + "');";
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertParent SQL Error: " << errorMessage << endl; }
}

void Database::insertUses(int stmtNum, string procedureName, string variableName) {
	char sqlBuf[256];
	sprintf(sqlBuf, "INSERT INTO use ('line_num','procedure_name','variable_name' ) VALUES ('%i','%s','%s');", stmtNum, procedureName.c_str(), variableName.c_str());
	//string sql = "INSERT INTO use ('line_num', 'procedure_name', 'variable_name' ) VALUES ('" + to_string(statementNumber) + "', '" + procedureName + "', '" + variablename + "');";
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	char sqlError[256];
	sprintf_s(sqlError, "insertUse SQL Error for %s at stmtNum %i: ", variableName.c_str(), stmtNum);
	if (errorMessage) { cout << sqlError << errorMessage << endl; }

}

void Database::insertModifies(int stmtNum, string procedureName, string variablename) {
	char sqlBuf[256];
	sprintf(sqlBuf, "INSERT INTO modify ('line_num','procedure_name','variable_name' ) VALUES ('%i','%s','%s');", stmtNum, procedureName.c_str(), variablename.c_str());
	//string sql = "INSERT INTO modify ('line_num', 'procedure_name', 'variable_name' ) VALUES ('" + to_string(statementNumber) + "', '" + procedureName + "', '" + variablename + "');";
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertModifies SQL Error: " << errorMessage << endl; }
}

void Database::insertNext(int stmtNum1, int stmtNum2) {
	char sqlBuf[256];
	sprintf(sqlBuf, "INSERT INTO next ('line_num_1','line_num_2') VALUES ('%i','%i');", stmtNum1, stmtNum2);
	//string sql = "INSERT INTO next ('line_num_1', 'line_num_2') VALUES ('" + to_string(stmtNum1) + "', '" + to_string(stmtNum2) + "');";
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertNext SQL Error: " << errorMessage; }
}

void Database::getNext_T(int stmtNum1, int stmtNum2, vector<string>& results) {
	dbResults.clear();
	char sqlBuf[256];
	vector<string> resultStore;
	sprintf(sqlBuf, "with recursive nextStmt as ("
					"select n.line_num_1, n.line_num_2 from next n where line_num_1 = %i"
					" union "
					"select n.line_num_1, n.line_num_2 from next n join nextStmt ns where n.line_num_1 = ns.line_num_2 and n.line_num_2 <= %i"
					")"
					"select* from nextStmt;", stmtNum1, stmtNum2);
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) {
		cout << "getNext_T SQL Error: " << errorMessage;
		return;
	}
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0) + "," + dbRow.at(1);
		results.push_back(result);
	}
}

void Database::getNext(int stmtNum1, int stmtNum2, vector<string>& results) {
	dbResults.clear();
	char sqlBuf[256];
	sprintf(sqlBuf, "SELECT * FROM next WHERE line_num_1 = '%i' AND line_num_2 = '%i';", stmtNum1, stmtNum2);
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) {
		cout << "getNext SQL Error: " << errorMessage;
		return;
	}
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0) + "," + dbRow.at(1);
		results.push_back(result);
	}
}

// method to get all the procedure from the database
void Database::getProcedures(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedure from the procedure table
	// The callback method is only used when there are results to be returned.
	string sql = "SELECT name FROM procedure;";
	sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);
	
	if (errorMessage) {
		cout << "SQL Error: " << errorMessage;
		return;
	}
	
	// Add output from database to vector for return
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

void Database::getVariable(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the variable name from the variable table
	// The callback method is only used when there are results to be returned.
	string sql = "SELECT name FROM variable;";
	sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);

	if (errorMessage) {
		cout << "SQL Error: " << errorMessage;
		return;
	}

	// Add output from database to vector for return
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

void Database::getStmt(string type, vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the variable name from the variable table
	// The callback method is only used when there are results to be returned.
	if (type == "stmt") {
		string sql = "SELECT line_num FROM statement;";
		sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);
	}
	else {
		char sqlBuf[256];
		sprintf(sqlBuf, "SELECT line_num FROM statement WHERE entity = '%s';", type.c_str());
		sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	}
	
	if (errorMessage) {
		cout << "SQL Error: " << errorMessage;
		return;
	}

	// Add output from database to vector for return
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

void Database::getConstant(vector<string>& results) {
	dbResults.clear();
	string sql = "SELECT * FROM constant;";
	sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);

	if (errorMessage) {
		cout << "SQL Error: " << errorMessage;
		return;
	}

	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// method to get all the statement from the database
void Database::getStatement(string type, vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedure from the procedure table
	// The callback method is only used when there are results to be returned.
	string sql = "SELECT * FROM statement WHERE stmtType = '" + type + "';";
	sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);

	if (errorMessage) {
		cout << "getStatement SQL Error: " << errorMessage;
		return;
	}
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

void Database::getStatement(vector<string>& results) {
	dbResults.clear();
	string sql = "SELECT line_num FROM statement;";
	sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);
	if (errorMessage) {
		cout << "getStatement SQL Error: " << errorMessage;
		return;
	}
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

void Database::getParent(string stmtNum1, string stmtNum2, vector<string>& results) {
	dbResults.clear();

	string sql = "SELECT MAX(line_num) FROM parent WHERE child_start <= '" + stmtNum2 + "' AND child_end >= '" + stmtNum2 + "';";
	sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);
		
	if (errorMessage) {
		cout << "getParent SQL Error: " << errorMessage;
	}

	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

void Database::getChildren(string stmtNum1, string stmtNum2, string statementType, vector<string>& results) {
	
	// clear the existing results
	dbResults.clear();

	string sql;

	if(statementType == "stmt")
		 sql = "WITH RECURSIVE expanded_range(n) AS ( SELECT child_start FROM parent WHERE line_num = '" + stmtNum1 + "' UNION ALL SELECT n+1 FROM expanded_range WHERE n+1 <= (SELECT child_end FROM parent WHERE line_num = '" + stmtNum1 + "')) SELECT line_num FROM statement WHERE line_num IN (SELECT n FROM expanded_range);";
	else
		 sql = "WITH RECURSIVE expanded_range(n) AS ( SELECT child_start FROM parent WHERE line_num = '" + stmtNum1 + "' UNION ALL SELECT n+1 FROM expanded_range WHERE n+1 <= (SELECT child_end FROM parent WHERE line_num = '" + stmtNum1 + "')) SELECT line_num FROM statement WHERE line_num IN (SELECT n FROM expanded_range) AND entity = '" + statementType + "';";

	sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);

	if (errorMessage) {
		cout << "getParent SQL Error: " << errorMessage;
	}

	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

void Database::getParentChildren(bool findparent, string resultType, string filterType, vector<string>& results) {

	// clear the existing results
	dbResults.clear();
	string sql;

	if (findparent)
		sql = "SELECT DISTINCT(p.line_num) FROM parent p JOIN statement s ON s.line_num BETWEEN p.child_start AND p.child_end WHERE s.entity = '" + filterType + "' AND line_num IN (SELECT line_num FROM statement WHERE entity = '" + resultType + "');";
	 
	else 
		sql = "WITH RECURSIVE expanded_range(n) AS ( SELECT child_start FROM parent WHERE p.line_num IN (SELECT line_num FROM statement WHERE entity = '" + filterType + "') UNION ALL SELECT n+1 FROM expanded_range WHERE n+1 <= (SELECT child_end FROM parent WHERE line_num IN (SELECT line_num FROM statement WHERE entity = '" + filterType + "'))) SELECT line_num FROM statement WHERE line_num IN (SELECT n FROM expanded_range) AND entity = '" + resultType + "';";


	sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);

	if (errorMessage) {
		cout << "getParent SQL Error: " << errorMessage;
	}

	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

/* Input types
	- input1 is statement number or name
		-- Possible entities are assign, print, container for statement number
			--- for container, we return true if there's Uses(c,v) inside container
			--- for assign, print, we return true if statement number is inside "use" table
		-- Possible entities are procedure, call for name
			--- for both, we return true if there's Uses(c,v) inside container
	- input2 is name
*/

bool Database::GetUsesForAssign(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific) {
	char sqlBuf[512] = {};
	// we are looking for each statements and checking if Uses(a,"cenX") is true, where "a" is assign a, but not present in select
	if (!input1IsSpecific && input2IsSpecific) { 
		sprintf_s(sqlBuf, "SELECT 1 FROM use u WHERE u.variable_name = '%s' AND u.line_num in (SELECT s.line_num FROM statement s WHERE entity = 'assign');", input2.c_str());
	}
	
	// we are looking for each statements and checking if Uses(a,"cenX") or Uses(a,v) is true, where "a" is assign a, "v" is variable v, and both are present in select
	else if (input1IsSpecific && input2IsSpecific) { 
		sprintf_s(sqlBuf, "SELECT 1 FROM use u WHERE u.line_num = '%s' AND u.variable_name = '%s'", input1.c_str(), input2.c_str());
	}

	// Uses(a,v) where "a" is assign a,"v" is variable v, and only "a" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT 1 FROM statement s WHERE s.line_num = '%s' AND s.entity = 'assign'", input1.c_str());
	}

	// we are looking for each statement and checking if Uses(a,v) is true, where "a" is assign a, "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT 1 FROM use u WHERE u.line_num in (SELECT s.line_num FROM statement s WHERE s.entity = 'assign');");
	}
	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	return (sqlResultStoreForCallback->sqlResult.empty());
}

bool Database::GetUsesForPrint(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific) {
	char sqlBuf[512] = {};
	// we are looking for each statements and checking if Uses(pn,"cenX") is true, where "pn" is print pn, but not present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT 1 FROM use u WHERE u.variable_name = '%s' AND u.line_num in (SELECT s.line_num FROM statement s WHERE entity = 'print');", input2.c_str());
	}

	// we are looking for each statements and checking if Uses(pn,"cenX") or Uses(pn,v) is true, where "pn" is print pn, "v" is variable v, and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT 1 FROM use u WHERE u.line_num = '%s' AND u.variable_name = '%s'", input1.c_str(), input2.c_str());
	}

	// Uses(pn,v) where "pn" is print pn,"v" is variable v, and only "pn" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT 1 FROM statement s WHERE s.line_num = '%s' AND s.entity = 'print'", input1.c_str());
	}

	// we are looking for each statement and checking if Uses(pn,v) is true, where "pn" is assign pn, "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT 1 FROM use u WHERE u.line_num in (SELECT s.line_num FROM statement s WHERE s.entity = 'print');");
	}
	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	return (sqlResultStoreForCallback->sqlResult.empty());
}

bool Database::GetUsesForWhile(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific) {
	char sqlBuf[512] = {};
	// we are looking for each statements and checking if Uses(w,"cenX") or Uses(w,v) is true, where "w" is while w, "v" is variable v,  "w" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT p.line_num FROM parent p JOIN statement s ON p.line_num = s.line_num WHERE s.entity = 'while' AND EXISTS (SELECT 1 FROM use u WHERE u.line_num BETWEEN p.line_num AND p.child_end AND variable_name = '%s');", input2.c_str());
	}

	// we are looking for each statements and checking if Uses(w,"cenX") or Uses(w,v) is true, where "w" is while w, "v" is variable v, and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT p.line_num FROM parent p WHERE p.line_num = '%s' AND EXISTS (SELECT 1 FROM use u WHERE u.line_num BETWEEN p.line_num AND p.child_end AND variable_name = '%s');", input1.c_str(), input2.c_str());
	}

	// Uses(w,v) where "w" is while w,"v" is variable v, and only "w" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT p.line_num FROM parent p WHERE p.line_num = '%s' AND EXISTS (SELECT 1 FROM use u WHERE u.line_num BETWEEN p.line_num AND p.child_end);", input1.c_str());
	}

	// we are looking for each statement and checking if Uses(pn,v) is true, where "pn" is assign pn, "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT p.line_num FROM parent p  JOIN statement s ON p.line_num = s.line_num WHERE s.entity = 'while' AND EXISTS (SELECT 1 FROM use u WHERE u.line_num BETWEEN p.line_num AND p.child_end);");
	}
	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	return (sqlResultStoreForCallback->sqlResult.empty());
}

bool Database::GetUsesForIf(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific) {
	char sqlBuf[512] = {};
	// we are looking for each statements and checking if Uses(i,"cenX") or Uses(i,v) is true, where "i" is if i, "v" is variable v,  "i" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT p.line_num FROM parent p JOIN statement s ON p.line_num = s.line_num WHERE s.entity = 'if' AND EXISTS (SELECT 1 FROM use u WHERE u.line_num BETWEEN p.line_num AND p.child_end AND variable_name = '%s');", input2.c_str());
	}

	// we are looking for each statements and checking if Uses(i,"cenX") or Uses(i,v) is true, where "i" is "while i", "v" is "variable v", and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT p.line_num FROM parent p WHERE p.line_num = '%s' AND EXISTS (SELECT 1 FROM use u WHERE u.line_num BETWEEN p.line_num AND p.child_end AND variable_name = '%s');", input1.c_str(), input2.c_str());
	}

	// Uses(w,v) where "w" is while w,"v" is variable v, and only "w" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT p.line_num FROM parent p WHERE p.line_num = '%s' AND EXISTS (SELECT 1 FROM use u WHERE u.line_num BETWEEN p.line_num AND p.child_end);", input1.c_str());
	}

	// we are looking for each statement and checking if Uses(pn,v) is true, where "pn" is assign pn, "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT p.line_num FROM parent p JOIN statement s ON p.line_num = s.line_num WHERE s.entity = 'if' AND EXISTS (SELECT 1 FROM use u WHERE u.line_num BETWEEN p.line_num AND p.child_end);");
	}
	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	return (sqlResultStoreForCallback->sqlResult.empty());
}

bool Database::GetUsesForCall(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific) {
	char sqlBuf[512] = {};
	// we are looking for each statements and checking if Uses(c,"cenX") or Uses(c,v) is true, where "c" is "call c", "v" is variable v, "c" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT u.variable_name FROM use u WHERE u.variable_name = '%s' AND EXISTS (SELECT 1 FROM procedure p WHERE p.name IN (SELECT text FROM statement s WHERE s.entity = 'call') AND u.line_num BETWEEN p.start AND p.end);", input2.c_str());
	}

	// we are looking for each statements and checking if Uses(c,"cenX") or Uses(c,v) is true, where "c" is "call c", "v" is "variable v", and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT u.variable_name FROM use u WHERE u.variable_name = '%s' AND EXISTS (SELECT 1 FROM procedure p WHERE p.name = '%s' AND u.line_num BETWEEN p.start AND p.end);", input2.c_str(), input1.c_str());
	}

	// Uses(c,v) where "c" is "call c", "v" is "variable v", and only "c" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT u.variable_name FROM use u WHERE EXISTS (SELECT 1 FROM procedure p WHERE p.name = '%s' AND u.line_num BETWEEN p.start AND p.end);", input1.c_str());
	}

	// we are looking for each statement and checking if Uses(c,v) is true, where "c" is "call c", "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "SELECT u.variable_name FROM use u WHERE EXISTS (SELECT 1 FROM procedure p WHERE p.name IN (SELECT text FROM statement s WHERE s.entity = 'call') AND u.line_num BETWEEN p.start AND p.end);");
	}
	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	return (sqlResultStoreForCallback->sqlResult.empty());
}

bool Database::GetUsesForUnknownInput1(string input1, string input2, bool input2IsSpecific) {
	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "SELECT entity, text FROM statement WHERE line_num = %i;", input1.c_str());
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetUsesForUnknownInput1 SQL Error: " << errorMessage; }
	string entity = rs.sqlResult.at(0)->row.at("entity");
	string text = rs.sqlResult.at(0)->row.at("text");

	// e.g., use(10, v), and stmt 10 is "x = a + b" or "print x". We just need to select from use table with the correct stmtNum to get the variables
	if (entity == "assign" || entity == "print") {
		sprintf_s(sqlBuf, "SELECT variable_name FROM use WHERE line_num = %i;", input1.c_str());
		sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	}

	// e.g, use(10, v), and stmt 10 is "call procedureX". We need to get all the var that fulfills the use(x,v) in this procedure, and all direct/indirect called procedures in this procedure
	else if (entity == "call") {
		rs.sqlResult.clear();
		sprintf_s(sqlBuf, "with recursive cte as("
			"select s.text from statement s join procedure p on s.procedure_name = p.name where p.name = '%s' and s.entity = 'call' union select '%s' "
			"union "
			"select s.text from statement s join procedure p on s.procedure_name = p.name join cte c where p.name = c.text and s.entity = 'call') "
			"select * from cte;", text.c_str(), text.c_str());
		sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
		if (errorMessage) { cout << "getUse \"entity == call\" SQL Error: " << errorMessage; }
		string proceduresJoin = "";
		for (int i = 0; i < dbResults.size(); i++) { // due to recursive query, each result is in its own dbResult[i] 
			char buf[256] = {};
			sprintf_s(buf, "'%s',", dbResults.at(i).at(0).c_str());
			proceduresJoin.append(buf);
		}
		proceduresJoin.pop_back(); // remove the last ","
		dbResults.clear();
		sprintf_s(sqlBuf, "select distinct u.variable_name from use u where exists (select 1 from procedure p where name in (%s) and u.line_num between p.start and p.end);", proceduresJoin.c_str());
		sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	}

	// e.g., use(10, v), and stmt 10 is "if(...)", and inside "if(...)", there's use(s, v). We get all uses(s,v) in that container
	else if (entity == "while" || entity == "if") {
		dbResults.clear();
		sprintf_s(sqlBuf, "select distinct u.variable_name from use u where exists (select 1 from parent p where u.line_num between p.line_num and p.child_end and p.line_num = %i);", input1.c_str());
		sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	}

	if (errorMessage) { cout << "GetUsesForUnknownInput1SQL Error: " << errorMessage; }
	return false;
}

void Database::getUses(int stmtNum, vector<string>& results) { // for cases such as uses(s,v) where s = stmt num, and v is "variable v;"
	dbResults.clear();
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "SELECT entity, text FROM statement WHERE line_num = %i;", stmtNum);
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "getUse start SQL Error: " << errorMessage; return; }
	
	string entity = dbResults.at(0).at(0);
	string text = dbResults.at(0).at(1);

	// e.g., use(10, v), and stmt 10 is "x = a + b" or "print x". We just need to select from use table with the correct stmtNum to get the variables
	if (entity == "assign" || entity == "print") { 
		sprintf_s(sqlBuf, "SELECT variable_name FROM use WHERE line_num = %i;", stmtNum);
		sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	}

	// e.g, use(10, v), and stmt 10 is "call procedureX". We need to get all the var that fulfills the use(x,v) in this procedure, and all direct/indirect called procedures in this procedure
	else if (entity == "call") {
		dbResults.clear();
		sprintf_s(sqlBuf, "with recursive cte as("
			"select s.text from statement s join procedure p on s.procedure_name = p.name where p.name = '%s' and s.entity = 'call' union select '%s' "
			"union "
			"select s.text from statement s join procedure p on s.procedure_name = p.name join cte c where p.name = c.text and s.entity = 'call') "
			"select * from cte;", text.c_str(), text.c_str());
		sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
		if (errorMessage) { cout << "getUse \"entity == call\" SQL Error: " << errorMessage; return; }
		string proceduresJoin = "";
		for (int i = 0; i < dbResults.size(); i++) { // due to recursive query, each result is in its own dbResult[i] 
			char buf[256] = {};
			sprintf_s(buf, "'%s',", dbResults.at(i).at(0).c_str());
			proceduresJoin.append(buf);
		}
		proceduresJoin.pop_back(); // remove the last ","
		dbResults.clear();
		sprintf_s(sqlBuf, "select distinct u.variable_name from use u where exists (select 1 from procedure p where name in (%s) and u.line_num between p.start and p.end);", proceduresJoin.c_str());
		sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	}

	// e.g., use(10, v), and stmt 10 is "if(...)", and inside "if(...)", there's use(s, v). We get all uses(s,v) in that container
	else if (entity == "while" || entity == "if") {
		dbResults.clear();
		sprintf_s(sqlBuf, "select distinct u.variable_name from use u where exists (select 1 from parent p where u.line_num between p.line_num and p.child_end and p.line_num = %i);", stmtNum);
		sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	}

	if (errorMessage) { cout << "getUse end SQL Error: " << errorMessage; return; }

	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

void Database::getUses(int stmtNum, string variable, vector<string>& results) {
	vector<string> localResults;
	Database::getUses(stmtNum, localResults);
	for (int i = 0; i < localResults.size(); i++) {
		if (localResults.at(i) == variable) {

		}
	}
}

void Database::getUses(string entity, SqlResult* sqlResult) { // for cases such as uses(<entity>, v), and <entity> and v is a general entity. E.g., "procedure p;" or "assign a;" or "variable v;"
	dbResults.clear();
	char sqlBuf[512] = {};
	if (entity == "assign" || entity == "print") { // for cases such as "assign a; variable v; select uses(a,v)"
		sprintf_s(sqlBuf, "SELECT * FROM statement s JOIN use u ON s.line_num = u.line_num WHERE s.entity = '%s';", entity.c_str());
	}

	else if (entity == "call") { // for cases such as "call c; select uses(c,v)" = return all call stmts that has uses(c,v)
		sprintf_s(sqlBuf,"select s.line_num from statement s join procedure p on p.name = s.text where s.entity = 'call' and exists (select 1 from use u where u.line_num between p.start and p.end);");
	}

	else if (entity == "procedure") { // for cases such as "procedure p; select uses(p,v)" = reuturn all procedure name that has uses(c,v)		
		sprintf_s(sqlBuf,"select p.name from procedure p where exists (select 1 from use u where u.line_num between p.start and p.end);");
	}

	else if (entity == "if" || entity == "while") { // for cases such as "if i; select uses(i,v)" = return all if statements that has uses(c,v). Kind of pointless because it'll return all of them
		sprintf_s(sqlBuf, "select p.line_num from parent p join statement s on p.line_num = s.line_num where s.entity = '%s' and exists (select 1 from use u where u.line_num between p.line_num and p.child_end);", entity.c_str());
	}

	dbResults.clear();
	//sqlResultSetPtrForCallback = sqlResult;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);

	if (errorMessage) {
		cout << "getUse SQL Error: " << errorMessage;
		return;
	}

	/*
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
	*/

}

void Database::getModifyStmt(string stmtNum1, string stmtNum2, bool lhs, vector<string>& results) {
	// clear the existing results
	dbResults.clear();
	string sql;

	if (lhs) {
		sql = "SELECT variable_name FROM modify WHERE line_num = '" + stmtNum1 + "'";
	}
	else {
		sql = "SELECT variable_name FROM modify WHERE line_num = '" + stmtNum2 + "'";
	}

	sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);

	if (errorMessage) {
		cout << "getModifyStmt SQL Error: " << errorMessage;
	}

	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
 }

// get all the columns of PQL select block
//void Database::select(Select& st, SqlResultSet* sqlResultSet) {
void Database::SelectPql(Select& st, SqlResultStore& sqlResultStore) {
	sqlResultStoreForCallback = &sqlResultStore;
	char sqlBuf[1024] = {};
	string selectFromTable = "";
	string selectColumnName = "";
	string whereColumnFilter = "";
	string whereDuplicateFilter = "";
	string SqlAND = " AND ";
	for (int i = 0; i < st.tableSql.size(); i++) {
		selectFromTable += (st.tableSql.at(i) + ",");
		sprintf_s(sqlBuf, "%s %s,", st.columnSql.at(i).c_str(), st.asSql.at(i).c_str());
		selectColumnName += sqlBuf;
	}
	for (int i = 0; i < st.whereSql.size(); i++) {
		whereColumnFilter += (st.whereSql.at(i) + SqlAND);
	}
	/*
	for (int i = 0; i < st.columnSql.size(); i++) {
		for (int j = i + 1; j < st.columnSql.size(); j++) {
			if (st.columnSql.at(i)[0] != st.columnSql.at(j)[0]) { continue; }
			sprintf_s(sqlBuf, "%s <> %s%s", st.columnSql.at(i).c_str(), st.columnSql.at(j).c_str(), SqlAND.c_str());
			whereDuplicateFilter += sqlBuf;
		}
	}
	*/

	selectFromTable.pop_back(); // remove the last ","
	selectColumnName.pop_back(); // remove the last ","
	whereColumnFilter = whereColumnFilter.substr(0, whereColumnFilter.size() - SqlAND.size()); // remove the last " AND "
	whereDuplicateFilter = whereDuplicateFilter.substr(0, whereDuplicateFilter.size() - SqlAND.size()); // remove the last " AND "
	//if(whereDuplicateFilter == ""){ sprintf_s(sqlBuf, "SELECT %s FROM %s WHERE %s", selectColumnName.c_str(), selectFromTable.c_str(), whereColumnFilter.c_str()); }
	//else{ sprintf_s(sqlBuf, "SELECT %s FROM %s WHERE %s AND %s", selectColumnName.c_str(), selectFromTable.c_str(), whereColumnFilter.c_str(), whereDuplicateFilter.c_str()); }
	sprintf_s(sqlBuf, "SELECT %s FROM %s WHERE %s", selectColumnName.c_str(), selectFromTable.c_str(), whereColumnFilter.c_str());
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "PQL select Sql Error: " << errorMessage; }
}


// callback method to put one row of results from the database into the dbResults vector
// This method is called each time a row of results is returned from the database
int Database::callback(void* NotUsed, int columnCount, char** columnValues, char** columnNames) {
	NotUsed = 0;
	vector<string> dbRow;
	SqlResult* sqlResult = new SqlResult;
	// argc is the number of columns for this row of results
	// argv contains the values for the columns
	// Each value is pushed into a vector.
	for (int i = 0; i < columnCount; i++) {
		dbRow.push_back(columnValues[i]);
		sqlResult->row.insert(std::pair<string, string>(columnNames[i], string(columnValues[i])));
	}
	
	// The row is pushed to the vector for storing all rows of results 
	dbResults.push_back(dbRow);
	sqlResultStoreForCallback->sqlResult.push_back(sqlResult);
	sqlResultStoreForCallback->sqlResultSet.insert(sqlResult);

	return 0;
}

//void Database::select(map)

int Database::callback_new(void* NotUsed, int argc, char** argv, char** azColName) {
	NotUsed = 0;
	vector<string> dbRow;
	
	// argc is the number of columns for this row of results
	// argv contains the values for the columns
	// Each value is pushed into a vector.
	for (int i = 0; i < argc; i++) {
		dbRow.push_back(argv[i]);
	}

	// The row is pushed to the vector for storing all rows of results 
	dbResults.push_back(dbRow);

	return 0;
}