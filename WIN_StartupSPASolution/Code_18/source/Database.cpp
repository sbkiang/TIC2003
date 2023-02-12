#include "Database.h"
#include <iostream>

sqlite3* Database::dbConnection;
vector<vector<string>> Database::dbResults;
char* Database::errorMessage;

// method to connect to the database and initialize tables in the database
void Database::initialize() {
	// open a database connection and store the pointer into dbConnection
	sqlite3_open("database.db", &dbConnection);

	// drop the existing procedure table (if any)
	string dropProcedureTableSQL = "DROP TABLE IF EXISTS procedures";
	sqlite3_exec(dbConnection, dropProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a procedure table
	string createProcedureTableSQL = "CREATE TABLE procedures ( name VARCHAR(50) PRIMARY KEY );";
	sqlite3_exec(dbConnection, createProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing stmt table (if any)
	string dropStatementTableSQL = "DROP TABLE IF EXISTS statement";
	sqlite3_exec(dbConnection, dropStatementTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a statement table
	string createStatementTableSQL = "CREATE TABLE statement ( line_num INT PRIMARY KEY, procedure_name VARCHAR(255) REFERENCES procedures(name), entity VARCHAR(255), text VARCHAR(255));";
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
	string createParentTableSQL = "CREATE TABLE parent ( parent_line INT REFERENCES statement(line_num), child_line INT REFERENCES statement(line_num), direct_child CHAR(1));";
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
	string createNextTableSQL = "CREATE TABLE next (line_num_1 INT REFERENCES statement(line_num), line_num_2 INT REFERENCES statement(line_num), loopWhile CHAR(1), CONSTRAINT line_num_not_equal check(line_num_1 <> line_num_2));";
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
void Database::insertProcedure(string procedureName) {
	string sql = "INSERT INTO procedures ('name') VALUES ('" + procedureName + "');";
	sqlite3_exec(dbConnection, sql.c_str(), NULL, 0, &errorMessage);
}

// method to insert a statement into the database
void Database::insertStatement(int statementNumber, string statementName, string entity, string text) {
	string sql = "INSERT INTO statement ('line_num', 'procedure_name', 'entity', 'text') VALUES ('" + to_string(statementNumber) + "', '" + statementName + "', '" + entity + "', '" + text + "');";
	sqlite3_exec(dbConnection, sql.c_str(), NULL, 0, &errorMessage);
}

// method to insert a statement into the database
void Database::insertVariable(string statementName, int statementNumber) {
	string sql = "INSERT INTO variable ('name', 'line_num') VALUES ('" + statementName + "', '" + to_string(statementNumber) + "');";
	sqlite3_exec(dbConnection, sql.c_str(), NULL, 0, &errorMessage);
}

void Database::insertConstant(string value) {
	string sql = "INSERT INTO constant ('value') VALUES ('" + value + "');";
	sqlite3_exec(dbConnection, sql.c_str(), NULL, 0, &errorMessage);
}

void Database::insertParent(int parent, int child, bool direct) {
	string sql = "INSERT INTO parent ('parent_line', 'child_line', 'direct_child' ) VALUES ('" + to_string(parent) + "', '" + to_string(child) + "', '" + to_string(direct) + "');";
	sqlite3_exec(dbConnection, sql.c_str(), NULL, 0, &errorMessage);
}

void Database::insertUse(int statementNumber, string procedureName, string variablename) {
	string sql = "INSERT INTO use ('line_num', 'procedure_name', 'variable_name' ) VALUES ('" + to_string(statementNumber) + "', '" + procedureName + "', '" + variablename + "');";
	sqlite3_exec(dbConnection, sql.c_str(), NULL, 0, &errorMessage);
}

void Database::insertModifies(int statementNumber, string procedureName, string variablename) {
	string sql = "INSERT INTO modify ('line_num', 'procedure_name', 'variable_name' ) VALUES ('" + to_string(statementNumber) + "', '" + procedureName + "', '" + variablename + "');";
	sqlite3_exec(dbConnection, sql.c_str(), NULL, 0, &errorMessage);
}

void Database::insertNext(int stmtNum1, int stmtNum2, string loopWhile) {
	string sql = "INSERT INTO next ('line_num_1', 'line_num_2' , loopWhile) VALUES ('" + to_string(stmtNum1) + "', '" + to_string(stmtNum2) + "', '" + loopWhile + "');";
	sqlite3_exec(dbConnection, sql.c_str(), NULL, 0, &errorMessage);
	if (errorMessage) {
		cout << "SQL Error: " << errorMessage;
	}
}

void Database::getNext_direct(int stmtNum1, int stmtNum2) {

}
void Database::getNext_indirect(int stmtNum1, int stmtNum2) {

}

// method to get all the procedures from the database
void Database::getProcedures(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string sql = "SELECT * FROM procedures;";
	sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
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

	// postprocess the results from the database so that the output is just a vector of procedure names
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
		string sql = "SELECT line_num FROM statement WHERE entity = '" + type + "';";
		sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);
	}
	// postprocess the results from the database so that the output is just a vector of procedure names
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

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string sql = "SELECT * FROM statement WHERE stmtType = '" + type + "';";
	sqlite3_exec(dbConnection, sql.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// callback method to put one row of results from the database into the dbResults vector
// This method is called each time a row of results is returned from the database
int Database::callback(void* NotUsed, int argc, char** argv, char** azColName) {
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