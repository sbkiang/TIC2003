#include "Database.h"
#include <iostream>
#include <regex>


sqlite3* Database::dbConnection;
vector<vector<string>> Database::dbResults;
char* Database::errorMessage;
SqlResultStore* sqlResultStoreForCallback;
string regexDigit = "^\\d+";
//string regexStmtNumEntity = "(stmt|read|print|assign|while|if|call)";
//string regexNameEntity = "(variable|procedure)";

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
	//string createStatementTableSQL = "CREATE TABLE statement ( line_num INT PRIMARY KEY, procedure_name VARCHAR(255) REFERENCES procedure(name), entity VARCHAR(255), text VARCHAR(255));";
	string createStatementTableSQL = "CREATE TABLE statement ( line_num INT PRIMARY KEY, entity VARCHAR(255), text VARCHAR(255));";
	sqlite3_exec(dbConnection, createStatementTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing variable table (if any)
	string dropVariableTableSQL = "DROP TABLE IF EXISTS variable";
	sqlite3_exec(dbConnection, dropVariableTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a variable table
	//string createVariableTableSQL = "CREATE TABLE variable ( name VARCHAR(20) PRIMARY KEY, line_num INT REFERENCES statement(line_num) );";
	string createVariableTableSQL = "CREATE TABLE variable ( name VARCHAR(20) PRIMARY KEY);";
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
	string createModifyTableSQL = "CREATE TABLE modify ( line_num INT REFERENCES statement(line_num), variable_name VARCHAR(50) REFERENCES variable(name), PRIMARY KEY(line_num, variable_name));";
	sqlite3_exec(dbConnection, createModifyTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing use table (if any)
	string dropUseTableSQL = "DROP TABLE IF EXISTS use";
	sqlite3_exec(dbConnection, dropUseTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a use table
	string createUseTableSQL = "CREATE TABLE use ( line_num INT REFERENCES statement(line_num), variable_name VARCHAR(50) REFERENCES variable(name), PRIMARY KEY(line_num, variable_name));";
	sqlite3_exec(dbConnection, createUseTableSQL.c_str(), NULL, 0, &errorMessage);
	
	// drop the existing call table (if any)
	string dropCallTableSQL = "DROP TABLE IF EXISTS call";
	sqlite3_exec(dbConnection, dropCallTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a call table
	//string createCallTableSQL = "CREATE TABLE call ( line_num INT REFERENCES statement(line_num), procedure_name VARCHAR(50) REFERENCES procedure(name), variable_name VARCHAR(50) REFERENCES variable(name), direct_call INT(1));";
	string createCallTableSQL = "CREATE TABLE call ( caller VARCHAR(50) REFERENCES procedure(name), callee VARCHAR(50) REFERENCES procedure(name), PRIMARY KEY(caller, callee), CONSTRAINT caller_callee_not_equal check(caller <> callee));";
	sqlite3_exec(dbConnection, createCallTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing next table (if any)
	string dropNextTableSQL = "DROP TABLE IF EXISTS next";
	sqlite3_exec(dbConnection, dropNextTableSQL.c_str(), NULL, 0, &errorMessage);

	//create a next table
	string createNextTableSQL = "CREATE TABLE next (from_line INT REFERENCES statement(line_num), to_line INT REFERENCES statement(line_num), CONSTRAINT line_num_not_equal check(from_line <> to_line));";
	sqlite3_exec(dbConnection, createNextTableSQL.c_str(), NULL, 0, &errorMessage);

	//drop the existing pattern table (if any)
	string dropPatternTableSQL = "DROP TABLE IF EXISTS pattern";
	sqlite3_exec(dbConnection, dropPatternTableSQL.c_str(), NULL, 0, &errorMessage);

	//create a pattern table
	string createPatternTableSQL = "CREATE TABLE pattern (line_num INT PRIMARY KEY REFERENCES statement(line_num), lhs VARCHAR(50), rhs VARCHAR(50), expression VARCHAR(50));";
	sqlite3_exec(dbConnection, createPatternTableSQL.c_str(), NULL, 0, &errorMessage);


	
	//error message output
	if (errorMessage) { cout << "insertStatement SQL Error: " << errorMessage << endl; return; }

	// initialize the result vector
	dbResults = vector<vector<string>>();
}

// method to close the database connection
void Database::close() {
	sqlite3_close(dbConnection);
}

bool Database::ExecuteSql(string sql, SqlResultStore& rs)
{
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);
	cout << endl << "SQL: " << sql << endl;
	if (errorMessage) {
		cout << "** SQL ERROR **" << endl;
		cout << errorMessage << endl; return false;
	}
	return true;
}

// method to insert a procedure into the database
void Database::insertProcedure(string procedureName, int start, int end) {
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO procedure ('name','start','end') VALUES ('%s',%i,%i);", procedureName.c_str(), start, end);
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertProcedure SQL Error: " << errorMessage << endl; }
}

// method to insert a statement into the database
void Database::insertStatement(int stmtNum, string entity, string text) {
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO statement ('line_num','entity','text') VALUES (%i,'%s','%s');", stmtNum, entity.c_str(), text.c_str());
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertStatement SQL Error: " << errorMessage << endl; }
}

// method to insert a statement into the database
void Database::insertVariable(string stmtName) {
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO variable('name') VALUES('%s');", stmtName.c_str());
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	char sqlError[256];
	sprintf_s(sqlError, "insertVariable SQL Error for %s: ", stmtName.c_str());
	if (errorMessage) { 

		cout << sqlError << errorMessage << endl; 
	}
}

void Database::insertConstant(string value) {
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO constant ('value') VALUES ('%s');", value.c_str());
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertConstant SQL Error: " << errorMessage << endl; }
}

void Database::insertParent(int parent, int child_start, int child_end) {
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO parent ('line_num','child_start','child_end') VALUES (%i,%i,%i);", parent, child_start, child_end);
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertParent SQL Error: " << errorMessage << endl; }
}

void Database::insertUses(int stmtNum, string variableName) {
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO use ('line_num','variable_name' ) VALUES ('%i','%s');", stmtNum, variableName.c_str());
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	char sqlError[256];
	sprintf_s(sqlError, "insertUse SQL Error for %s at stmtNum %i: ", variableName.c_str(), stmtNum);
	if (errorMessage) { cout << sqlError << errorMessage << endl; }

}

void Database::insertModifies(int stmtNum, string variablename) {
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO modify ('line_num','variable_name' ) VALUES ('%i','%s');", stmtNum, variablename.c_str());
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertModifies SQL Error: " << errorMessage << endl; }
}

void Database::insertPattern(int stmtNum, string lhs, string rhs, string expression) {
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO pattern ('line_num','lhs','rhs','expression') VALUES ('%i','%s','%s','%s');", stmtNum, lhs.c_str(), rhs.c_str(), expression.c_str());
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertPattern SQL Error: " << errorMessage; }
}

void Database::insertNext(int from, int to) {
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO next ('from_line','to_line') VALUES (%i,%i);", from, to);
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertNext SQL Error: " << errorMessage; }
}

void Database::insertCall(string caller, string callee) {
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO call ('caller','callee') VALUES ('%s','%s');", caller.c_str(), callee.c_str());
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertCall SQL Error: " << errorMessage << endl; }
}

// callback method to put one row of results from the database into the dbResults vector
// This method is called each time a row of results is returned from the database
int Database::callback(void* NotUsed, int columnCount, char** columnValues, char** columnNames) {
	NotUsed = 0;
	vector<string> dbRow;
	Row row;
	RowSet rowSet;
	// argc is the number of columns for this row of results
	// argv contains the values for the columns
	// Each value is pushed into a vector.
	
	for (int i = 0; i < columnCount; i++) {
		dbRow.push_back(columnValues[i]);
		//row.row.insert(std::pair<string, string>(columnNames[i], string(columnValues[i])));
		rowSet.row.insert(std::pair<string, string>(columnNames[i], string(columnValues[i])));
	}
	// The row is pushed to the vector for storing all rows of results 
	dbResults.push_back(dbRow);
	//sqlResultStoreForCallback->sqlResult.push_back(row);
	sqlResultStoreForCallback->sqlResultSet.insert(rowSet);

	return 0;
}