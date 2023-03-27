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
	//string createStatementTableSQL = "CREATE TABLE statement ( line_num INT PRIMARY KEY, procedure_name VARCHAR(255) REFERENCES procedure(name), entity VARCHAR(255), text VARCHAR(255));";
	string createStatementTableSQL = "CREATE TABLE statement ( line_num INT PRIMARY KEY, entity VARCHAR(255), text VARCHAR(255));";
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
	string createModifyTableSQL = "CREATE TABLE modify ( line_num INT REFERENCES statement(line_num), variable_name VARCHAR(50) REFERENCES variable(name), PRIMARY KEY(line_num, variable_name));";
	sqlite3_exec(dbConnection, createModifyTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing use table (if any)
	string dropUseTableSQL = "DROP TABLE IF EXISTS use";
	sqlite3_exec(dbConnection, dropUseTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a use table
	string createUseTableSQL = "CREATE TABLE use ( line_num INT REFERENCES statement(line_num), variable_name VARCHAR(50) REFERENCES variable(name), PRIMARY KEY(line_num, variable_name));";
	sqlite3_exec(dbConnection, createUseTableSQL.c_str(), NULL, 0, &errorMessage);
	
	// drop the existing next table (if any)
	string dropNextTableSQL = "DROP TABLE IF EXISTS next";
	sqlite3_exec(dbConnection, dropNextTableSQL.c_str(), NULL, 0, &errorMessage);

	//create a next table
	string createNextTableSQL = "CREATE TABLE next (line_num_1 INT REFERENCES statement(line_num), line_num_2 INT REFERENCES statement(line_num), CONSTRAINT line_num_not_equal check(line_num_1 <> line_num_2));";
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
	//sprintf_s(sqlBuf, "INSERT INTO statement ('line_num','procedure_name','entity','text') VALUES ('%i','%s','%s','%s');", stmtNum, stmtName.c_str(), entity.c_str(), text.c_str());
	sprintf_s(sqlBuf, "INSERT INTO statement ('line_num','entity','text') VALUES (%i,'%s','%s');", stmtNum, entity.c_str(), text.c_str());
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertStatement SQL Error: " << errorMessage << endl; }
}

// method to insert a statement into the database
void Database::insertVariable(string stmtName, int stmtNum) {
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO variable('name','line_num') VALUES('%s','%i');", stmtName.c_str(), stmtNum);
	//string sql = "INSERT INTO variable ('name', 'line_num') VALUES ('" + statementName + "', '" + to_string(statementNumber) + "');";
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	char sqlError[256];
	sprintf_s(sqlError, "insertVariable SQL Error for %s at stmtNum %i: ", stmtName.c_str(), stmtNum);
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

void Database::insertNext(int stmtNum1, int stmtNum2) {
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO next ('line_num_1','line_num_2') VALUES ('%i','%i');", stmtNum1, stmtNum2);
	//string sql = "INSERT INTO next ('line_num_1', 'line_num_2') VALUES ('" + to_string(stmtNum1) + "', '" + to_string(stmtNum2) + "');";
	sqlite3_exec(dbConnection, sqlBuf, NULL, 0, &errorMessage);
	if (errorMessage) { cout << "insertNext SQL Error: " << errorMessage; }
}


bool Database::getNext(int stmtNum1, int stmtNum2) {
	dbResults.clear();
	char sqlBuf[256];
	sprintf_s(sqlBuf, "SELECT 1 FROM next WHERE line_num_1 = '%i' AND line_num_2 = '%i';", stmtNum1, stmtNum2);
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "getNext SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::getNextT(int stmtNum1, int stmtNum2) {
	dbResults.clear();
	char sqlBuf[256];
	vector<string> resultStore;
	sprintf_s(sqlBuf, "with recursive nextStmt as ("
					"select n.line_num_1, n.line_num_2 from next n where line_num_1 = %i"
					" union "
					"select n.line_num_1, n.line_num_2 from next n join nextStmt ns where n.line_num_1 = ns.line_num_2 and n.line_num_2 <= %i"
					")"
					"select * from nextStmt;", stmtNum1, stmtNum2);
	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "getNextT SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
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

void Database::GetVariable(set<string>& result) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select name from variable");
	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "getParent SQL Error: " << errorMessage; exit(1); }
	for (int i = 0; i < rs.sqlResult.size(); i++) {
		result.insert(rs.sqlResult.at(i).row["name"]);
	}
}

bool Database::GetParent(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, string parentEntity, string childEntity) {
	/*	select 1 from statement s where s.entity = '%s' and ((select p.line_num from parent p where p.line_num = %s and s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1) = (select p.line_num from parent p join statement s2 where s2.entity = '%s' and s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1))
		select 1 from statement s where s.entity = '%s' and ((select p.line_num from parent p join statement s2 on p.line_num = s2.line_num where s2.entity = '%s' and s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1) = (select p.line_num from parent p join statement s2 where s2.entity = '%s' and s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1))

		SQL statement for generic containers
			1) get the nearest parent (the parent with highest line_num) of the specified container type (while / if)
			2) get the nearest parent (the parent with highest line_num)
			3) compare if both parent line_num are the same. If the same, then the statement is under the specified container = direct parent. If not, then it's indirect parent	
				** Even for cases where the parent line_num is specific, we still need the SQL at (2) to ensure that it's direct parent instead of indirect parent
	*/

	char sqlBuf[512] = {};
	// Parent(s1,10) or Parent(s1,s2), only "s2" present in select. "s1' is either "while w" or "if i" = generic
	if (!input1IsSpecific && input2IsSpecific) {
		if (parentEntity == "stmt") { // if parentEntity is stmt, then it's any container
			sprintf_s(sqlBuf, "select line_num from statement s where s.line_num = %s and exists (select 1 from parent p join statement s2 on p.line_num = s2.line_num where s.line_num between p.child_start and p.child_end)", input2.c_str());
		}
		else {
			sprintf_s(sqlBuf, "select line_num from statement s where s.line_num = %s and ((select p.line_num from parent p join statement s2 on p.line_num = s2.line_num where s2.entity = '%s' and s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1) = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1))", input2.c_str(), parentEntity.c_str());
		}
	}

	// Parent(10,20) or Parent(s1,s2), both present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num from statement s where s.line_num = %s and ((select p.line_num from parent p where p.line_num = %s and s.line_num between p.child_start and p.child_end) = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1))", input2.c_str(), input1.c_str());
	}

	// Parent(10,s2) or Parent(s1,s2), only "s1" present in select. "s1" will be a line_num, "s2" can be (stmt / read / print / assign / while / if / call)
	else if (input1IsSpecific && !input2IsSpecific) {
		if (childEntity == "stmt") { // if childEntity is stmt, then it's any statement nested in a container
			sprintf_s(sqlBuf, "select line_num from statement s where exists (select 1 from parent p where p.line_num = %s and s.line_num between p.child_start and p.child_end)", input1.c_str());
		}
		else {
			sprintf_s(sqlBuf, "select line_num from statement s where s.entity = '%s' and ((select p.line_num from parent p where p.line_num = %s and s.line_num between p.child_start and p.child_end) = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1))", childEntity.c_str(), input1.c_str());
		}
	}

	// Parent(s1,s2), both no in select. "s1" can be (while / if / stmt) = generic. "s2" can be (stmt / read / print / assign / while / if / call) = generic
	else if (!input1IsSpecific && !input2IsSpecific) {
		if (childEntity == "stmt" && parentEntity == "stmt") {
			sprintf_s(sqlBuf, "select line_num from statement s where exists (select 1 from parent where s.line_num between p.child_start and p.child_end)");
		}
		else {
			sprintf_s(sqlBuf, "select line_num from statement s where s.entity = '%s' and ((select p.line_num from parent p join statement s2 on p.line_num = s2.line_num where s2.entity = '%s' and s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1) = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1))", childEntity.c_str(), parentEntity.c_str());
		}
	}
		
	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "getParent SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetParentT(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, string parentEntity, string childEntity) {
	char sqlBuf[512] = {};
	// Parent(s1,10) or Parent(s1,s2), only "s2" present in select. "s1' is either "while w" or "if i" = generic
	if (!input1IsSpecific && input2IsSpecific) {
		if (parentEntity == "stmt") { // if parentEntity is stmt, then it's any container
			sprintf_s(sqlBuf, "select line_num from statement s where s.line_num = %s and exists (select 1 from parent p join statement s2 on p.line_num = s2.line_num where s.line_num between p.child_start and p.child_end)", input2.c_str());
		}
		else {
			sprintf_s(sqlBuf, "select line_num from statement s where s.line_num = %s and exists (select 1 from parent p join statement s2 on p.line_num = s2.line_num where s2.entity = '%s' and s.line_num between p.child_start and p.child_end);", input2.c_str(), parentEntity.c_str());
		}
	}

	// Parent(10,20) or Parent(s1,s2), both present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num from statement s where s.line_num = %s and exists (select 1 from parent p where p.line_num = %s and s.line_num between p.child_start and p.child_end);", input2.c_str(), input1.c_str());
	}

	// Parent(10,s2) or Parent(s1,s2), only "s1" present in select. "s1" will be a line_num, "s2" can be (stmt / read / print / assign / while / if / call)
	else if (input1IsSpecific && !input2IsSpecific) {
		if (childEntity == "stmt") { // if childEntity is stmt, then it's any statement nested in a container
			sprintf_s(sqlBuf, "select line_num from statement s where exists (select 1 from parent p where p.line_num = %s and s.line_num between p.child_start and p.child_end)", input1.c_str());
		}
		else {
			sprintf_s(sqlBuf, "select line_num from statement s where s.entity = '%s' and exists (select 1 from parent p where p.line_num = %s and s.line_num between p.child_start and p.child_end)", childEntity.c_str(), input1.c_str());
		}
	}

	// Parent(s1,s2), both no in select. "s1" can be (while / if / stmt) = generic. "s2" can be (stmt / read / print / assign / while / if / call) = generic
	else if (!input1IsSpecific && !input2IsSpecific) {
		if (childEntity == "stmt" && parentEntity == "stmt") {
			sprintf_s(sqlBuf, "select line_num from statement s where exists (select 1 from parent where s.line_num between p.child_start and p.child_end)");
		}
		else {
			sprintf_s(sqlBuf, "select line_num from statement s where s.entity = '%s' and exists (select 1 from parent p join statement s2 on p.line_num = s2.line_num where s2.entity = '%s' and s.line_num between p.child_start and p.child_end)", childEntity.c_str(), parentEntity.c_str());
		}
	}

	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "getParentT SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetUsesForAssign(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	// select 1 from use u where u.variable_name = '%s' and u.line_num in (select s.line_num from statement s where entity = 'assign' and s.line_num = '%s');

	char sqlBuf[512] = {};
	// Uses(a,"cenX") or Uses(a,v) where "a" is "assign a", "v" is "variable v", "a" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) { 
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.variable_name = '%s' and u.line_num in (select s.line_num from statement s where entity = 'assign');", input2.c_str());
	}
	
	// Uses(a,"cenX") or Uses(a,v) where "a" is "assign a", "v" is "variable v", both present in select
	else if (input1IsSpecific && input2IsSpecific) { 
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.variable_name = '%s' and u.line_num in (select s.line_num from statement s where entity = 'assign' and s.line_num = %s);", input2.c_str(), input1.c_str());
	}

	// Uses(a,v) where "a" is assign a,"v" is variable v, only "a" present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.line_num in (select s.line_num from statement s where entity = 'assign' and s.line_num = %s);", input1.c_str());
	}

	// Uses(a,v) where "a" is assign a, "v" is variable, both not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.line_num in (select s.line_num from statement s where entity = 'assign');");
	}
	
	sqlResultStoreForCallback = &rs;
	rs.columnTypeIndex.insert(pair<string, int>("statement", 0));
	rs.columnTypeIndex.insert(pair<string, int>("variable", 0));
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetUsesForAssign SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetUsesForPrint(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	// select 1 from use u where u.variable_name = '%s' and u.line_num in (select s.line_num from statement s where entity = 'print' and s.line_num = '%s');

	char sqlBuf[512] = {};
	// Uses(p,"cenX") or Uses(p,v) where "p" is "print p", "v" is "variable v", "p" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.variable_name = '%s' and u.line_num in (select s.line_num from statement s where entity = 'print');", input2.c_str());
	}

	// Uses(p,"cenX") or Uses(p,v) where "p" is "print p", "v" is "variable v", both present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.variable_name = '%s' and u.line_num in (select s.line_num from statement s where entity = 'print' and s.line_num = %s);", input2.c_str(), input1.c_str());
	}

	// Uses(p,"cenX") or Uses(p,v) where "p" is "print p", "v" is "variable v", only "p" present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.line_num in (select s.line_num from statement s where entity = 'print' and s.line_num = %s);", input1.c_str());
	}

	// Uses(p,"cenX") or Uses(p,v) where "p" is "print p", "v" is "variable v", both not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.line_num in (select s.line_num from statement s where entity = 'print');");
	}

	sqlResultStoreForCallback = &rs;
	rs.columnTypeIndex.insert(pair<string, int>("statement", 0));
	rs.columnTypeIndex.insert(pair<string, int>("variable", 0));
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetUsesForPrint SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetUsesForWhile(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	//select 1 from use u where u.variable_name = '%s' and exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'while' and u.line_num between p.line_num and p.child_end)
	
	char sqlBuf[512] = {};
	// Uses(w,"cenX") or Uses(w,v) is true, where "w" is while w, "v" is variable v, "w" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		//sprintf_s(sqlBuf, "SELECT p.line_num FROM parent p JOIN statement s ON p.line_num = s.line_num WHERE s.entity = 'while' AND EXISTS (SELECT 1 FROM use u WHERE u.line_num BETWEEN p.line_num AND p.child_end AND u.variable_name = '%s');", input2.c_str());
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.variable_name = '%s' and exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'while' and u.line_num between p.line_num and p.child_end);", input2.c_str());
	}

	// Uses(w,"cenX") or Uses(w,v) is true, where "w" is while w, "v" is variable v, and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.variable_name = '%s' and exists (select 1 from parent p where p.line_num = %s and u.line_num between p.line_num and p.child_end);", input2.c_str(), input1.c_str());
	}

	// Uses(w,v) where "w" is while w,"v" is variable v, and only "w" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where exists (select 1 from parent p where p.line_num = %s and u.line_num between p.line_num and p.child_end);", input1.c_str());
	}

	// we are looking for each statement and checking if Uses(pn,v) is true, where "pn" is assign pn, "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'while' and u.line_num between p.line_num and p.child_end);");
	}

	//SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	rs.columnTypeIndex.insert(pair<string, int>("statement", 0));
	rs.columnTypeIndex.insert(pair<string, int>("variable", 0));
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetUsesForWhile SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetUsesForIf(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	//select 1 from use u where u.variable_name = '%s' and exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'if' and u.line_num between p.line_num and p.child_end)
	
	char sqlBuf[512] = {};

	// Uses(i,"cenX") or Uses(i,v) where "i" is "if i", "v" is "variable v", "i" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.variable_name = '%s' and exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'if' and u.line_num between p.line_num and p.child_end);", input2.c_str());
	}

	// Uses(i,"cenX") or Uses(i,v) where "i" is "if i", "v" is "variable v", and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.variable_name = '%s' and exists (select 1 from parent p where p.line_num = %s and u.line_num between p.line_num and p.child_end);", input2.c_str(), input1.c_str());
	}

	// Uses(i,v) where "i" is "if i", "v" is "variable v", and only "i" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where exists (select 1 from parent p where p.line_num = %s and u.line_num between p.line_num and p.child_end);", input1.c_str());
	}

	// Uses(pn,v) where "pn" is "if i", "v" "is variable", and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'if' and u.line_num between p.line_num and p.child_end);");
	}

	//SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	rs.columnTypeIndex.insert(pair<string, int>("statement", 0));
	rs.columnTypeIndex.insert(pair<string, int>("variable", 0));
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetUsesForIf SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetUsesForCall(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore &rs) {
	//select 1 from use u where u.variable_name = '%s' and exists (select 1 from statement s where s.entity = 'call' and s.text = '%s' and s.line_num = u.line_num)
	
	char sqlBuf[512] = {};

	// Uses(c,"cenX") or Uses(c,v) is true, where "c" is "call c", "v" is variable v, "c" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.variable_name = '%s' and exists (select 1 from statement s where s.entity = 'call' and s.line_num = u.line_num);", input2.c_str());
	}

	// Uses(c,"cenX") or Uses(c,v) is true, where "c" is "call c", "v" is "variable v", and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where u.variable_name = '%s' and exists (select 1 from statement s where s.entity = 'call' and s.line_num = %s and s.line_num = u.line_num);", input2.c_str(), input1.c_str());
	}

	// Uses(c,v) where "c" is "call c", "v" is "variable v", and only "c" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where exists (select 1 from statement s where s.entity = 'call' and s.line_num = %s and s.line_num = u.line_num);", input1.c_str());
	}

	// we are looking for each statement and checking if Uses(c,v) is true, where "c" is "call c", "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from use u where exists (select 1 from statement s where s.entity = 'call' and s.line_num = u.line_num);");
	}

	sqlResultStoreForCallback = &rs;
	rs.columnTypeIndex.insert(pair<string, int>("statement", 0));
	rs.columnTypeIndex.insert(pair<string, int>("variable", 0));
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetUsesForCall SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetUsesForProcedure(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore &rs) {
	//select 1 from use u where u.variable_name = '%s' and exists (select 1 from procedure p where p.name = '%s' and u.line_num between p.start and p.end)

	char sqlBuf[512] = {};

	// Uses(p,"cenX") or Uses(p,v) is true, where "p" is "procedure p", "v" is variable v, "p" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, name as procedure_name from use u where u.variable_name = '%s' and exists (select 1 from procedure p where u.line_num between p.start and p.end);", input2.c_str());
	}

	// Uses(p,"cenX") or Uses(p,v) is true, where "p" is "procedure p", "v" is "variable v", and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select select line_num, name as procedure_name from use u where u.variable_name = '%s' and exists (select 1 from procedure p where p.name = '%s' and u.line_num between p.start and p.end);", input2.c_str(), input1.c_str());
	}

	// Uses(p,v) where "p" is "procedure p", "v" is "variable v", and only "p" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select select line_num, name as procedure_name from use u where exists (select 1 from procedure p where p.name = '%s' and u.line_num between p.start and p.end);", input1.c_str());
	}

	// we are looking for each statement and checking if Uses(c,v) is true, where "c" is "call c", "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select select line_num, name as procedure_name from use u where exists (select 1 from procedure p where u.line_num between p.start and p.end);");
	}
	sqlResultStoreForCallback = &rs;
	rs.columnTypeIndex.insert(pair<string, int>("statement", 0));
	rs.columnTypeIndex.insert(pair<string, int>("procedure", 0));
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetUsesForProcedure SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetUsesForUnknownInput1(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) { // for cases like "uses(10,v)" . stmt 10 can be if, while, call. we don't know
	SqlResultStore temp;
	sqlResultStoreForCallback = &temp;
	char sqlBuf[512] = {};
	bool ret = false;
	if (isdigit(input1[0])) { // input first char is a digit = statement number
		sprintf_s(sqlBuf, "SELECT entity, text FROM statement WHERE line_num = %s;", input1.c_str());
		sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
		if (errorMessage) { cout << "GetUsesForUnknownInput1 SQL Error: " << errorMessage; }
		string entity = temp.sqlResult.at(0).row.at("entity");
		string text = temp.sqlResult.at(0).row.at("text");

		// e.g., use(10, v), and stmt 10 is "x = a + b" or "print x". We just need to select from use table with the correct stmtNum to get the variables
		if (entity == "assign") { 
			ret = Database::GetUsesForAssign(input1, input2, input1IsSpecific, input2IsSpecific, rs); }
		else if (entity == "print") { 
			ret = Database::GetUsesForPrint(input1, input2, input1IsSpecific, input2IsSpecific, rs); }
		else if (entity == "call") { 
			ret = Database::GetUsesForCall(text, input2, input1IsSpecific, input2IsSpecific, rs); }
		else if (entity == "while") { 
			ret = Database::GetUsesForWhile(input1, input2, input1IsSpecific, input2IsSpecific, rs); }
		else if (entity == "if") { 
			ret = Database::GetUsesForIf(input1, input2, input1IsSpecific, input2IsSpecific, rs); }
	}
	else { // input first char is not a digit = a name
		ret = Database::GetUsesForProcedure(input1, input2, input1IsSpecific, input2IsSpecific, rs);
	}
	return ret;
}

bool Database::GetModifiesForAssign(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore &rs) {
	// select 1 from modify m where m.variable_name = '%s' and m.line_num in (select s.line_num from statement s where entity = 'assign' and s.line_num = '%s');

	char sqlBuf[512] = {};
	// Modifies(a,"cenX") or Modifies(a,v) where "a" is "assign a", "v" is "variable v", "a" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.variable_name = '%s' and m.line_num in (select s.line_num from statement s where entity = 'assign');", input2.c_str());
	}

	// Modifies(a,"cenX") or Modifies(a,v) where "a" is "assign a", "v" is "variable v", both present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.variable_name = '%s' and m.line_num in (select s.line_num from statement s where entity = 'assign' and s.line_num = %s);", input2.c_str(), input1.c_str());
	}

	// Modifies(a,v) where "a" is assign a,"v" is variable v, only "a" present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.line_num in (select s.line_num from statement s where entity = 'assign' and s.line_num = %s);", input1.c_str());
	}

	// Modifies(a,v) where "a" is assign a, "v" is variable, both not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.line_num in (select s.line_num from statement s where entity = 'assign');");
	}

	sqlResultStoreForCallback = &rs;
	rs.columnTypeIndex.insert(pair<string, int>("statement", 0));
	rs.columnTypeIndex.insert(pair<string, int>("variable", 1));
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetModifiesForRead(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	// select 1 from modify m where m.variable_name = '%s' and m.line_num in (select s.line_num from statement s where entity = 'print' and s.line_num = '%s');

	char sqlBuf[512] = {};
	// Modifies(p,"cenX") or Modifies(p,v) where "r" is "read r", "v" is "variable v", "r" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.variable_name = '%s' and m.line_num in (select s.line_num from statement s where entity = 'read');", input2.c_str());
	}

	// Modifies(p,"cenX") or Modifies(p,v) where "r" is "read r", "v" is "variable v", both present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.variable_name = '%s' and m.line_num in (select s.line_num from statement s where entity = 'read' and s.line_num = %s);", input2.c_str(), input1.c_str());
	}

	// Modifies(p,"cenX") or Modifies(p,v) where "r" is "read r", "v" is "variable v", only "r" present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.line_num in (select s.line_num from statement s where entity = 'read' and s.line_num = %s);", input1.c_str());
	}

	// Modifies(p,"cenX") or Modifies(p,v) where "r" is "read r", "v" is "variable v", both not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.line_num in (select s.line_num from statement s where entity = 'read');");
	}

	sqlResultStoreForCallback = &rs;
	rs.columnTypeIndex.insert(pair<string, int>("statement", 0));
	rs.columnTypeIndex.insert(pair<string, int>("variable", 1));
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetModifiesForWhile(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	//select 1 from modify m where m.variable_name = '%s' and exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'while' and m.line_num between p.line_num and p.child_end)

	char sqlBuf[512] = {};
	// Modifies(w,"cenX") or Modifies(w,v) is true, where "w" is while w, "v" is variable v, "w" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		//sprintf_s(sqlBuf, "SELECT p.line_num FROM parent p JOIN statement s ON p.line_num = s.line_num WHERE s.entity = 'while' AND EXISTS (SELECT 1 FROM modify m WHERE m.line_num BETWEEN p.line_num AND p.child_end AND m.variable_name = '%s');", input2.c_str());
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.variable_name = '%s' and exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'while' and m.line_num between p.line_num and p.child_end);", input2.c_str());
	}

	// Modifies(w,"cenX") or Modifies(w,v) is true, where "w" is while w, "v" is variable v, and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.variable_name = '%s' and exists (select 1 from parent p where p.line_num = %s and m.line_num between p.line_num and p.child_end);", input2.c_str(), input1.c_str());
	}

	// Modifies(w,v) where "w" is while w,"v" is variable v, and only "w" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where exists (select 1 from parent p where p.line_num = %s and m.line_num between p.line_num and p.child_end);", input1.c_str());
	}

	// we are looking for each statement and checking if Modifies(pn,v) is true, where "pn" is assign pn, "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'while' and m.line_num between p.line_num and p.child_end);");
	}

	//SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	rs.columnTypeIndex.insert(pair<string, int>("statement", 0));
	rs.columnTypeIndex.insert(pair<string, int>("variable", 1));
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetModifiesForIf(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	//select 1 from modify m where m.variable_name = '%s' and exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'if' and m.line_num between p.line_num and p.child_end)

	char sqlBuf[512] = {};


	// Modifies(i,"cenX") or Modifies(i,v) where "i" is "if i", "v" is "variable v", "i" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.variable_name = '%s' and exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'if' and m.line_num between p.line_num and p.child_end);", input2.c_str());
	}

	// Modifies(i,"cenX") or Modifies(i,v) where "i" is "if i", "v" is "variable v", and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.variable_name = '%s' and exists (select 1 from parent p where p.line_num = %s and m.line_num between p.line_num and p.child_end);", input2.c_str(), input1.c_str());
	}

	// Modifies(i,v) where "i" is "if i", "v" is "variable v", and only "i" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where exists (select 1 from parent p where p.line_num = %s and m.line_num between p.line_num and p.child_end);", input1.c_str());
	}

	// Modifies(pn,v) where "pn" is "if i", "v" "is variable", and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'if' and m.line_num between p.line_num and p.child_end);");
	}

	//SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	rs.columnTypeIndex.insert(pair<string, int>("statement", 0));
	rs.columnTypeIndex.insert(pair<string, int>("variable", 1));
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetModifiesForCall(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	//select 1 from modify m where m.variable_name = '%s' and exists (select 1 from statement s where s.entity = 'call' and s.text = '%s' and s.line_num = m.line_num)

	char sqlBuf[512] = {};

	// Modifies(c,"cenX") or Modifies(c,v) is true, where "c" is "call c", "v" is variable v, "c" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.variable_name = '%s' and exists (select 1 from statement s where s.entity = 'call' and s.line_num = m.line_num);", input2.c_str());
	}

	// Modifies(c,"cenX") or Modifies(c,v) is true, where "c" is "call c", "v" is "variable v", and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where m.variable_name = '%s' and exists (select 1 from statement s where s.entity = 'call' and s.line_num = %s and s.line_num = m.line_num);", input2.c_str(), input1.c_str());
	}

	// Modifies(c,v) where "c" is "call c", "v" is "variable v", and only "c" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where exists (select 1 from statement s where s.entity = 'call' and s.line_num = %s and s.line_num = m.line_num);", input1.c_str());
	}

	// we are looking for each statement and checking if Modifies(c,v) is true, where "c" is "call c", "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, variable_name from modify m where exists (select 1 from statement s where s.entity = 'call' and s.line_num = m.line_num);");
	}
	//SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	rs.columnTypeIndex.insert(pair<string, int>("statement", 0));
	rs.columnTypeIndex.insert(pair<string, int>("variable", 1));
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetModifiesForProcedure(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	//select 1 from modify m where m.variable_name = '%s' and exists (select 1 from procedure p where p.name = '%s' and m.line_num between p.start and p.end)

	char sqlBuf[512] = {};

	// Modifies(p,"cenX") or Modifies(p,v) is true, where "r" is "procedure p", "v" is variable v, "r" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, name as procedure_name from modify m where m.variable_name = '%s' and exists (select 1 from procedure p where m.line_num between p.start and p.end);", input2.c_str());
	}

	// Modifies(p,"cenX") or Modifies(p,v) is true, where "r" is "procedure p", "v" is "variable v", and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, name as procedure_name from modify m where m.variable_name = '%s' and exists (select 1 from procedure p where p.name = '%s' and m.line_num between p.start and p.end);", input2.c_str(), input1.c_str());
	}

	// Modifies(p,v) where "r" is "procedure p", "v" is "variable v", and only "r" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, name as procedure_name from modify m where exists (select 1 from procedure p where p.name = '%s' and m.line_num between p.start and p.end);", input1.c_str());
	}

	// we are looking for each statement and checking if Modifies(c,v) is true, where "c" is "call c", "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select line_num, name as procedure_name from modify m where exists (select 1 from procedure p where m.line_num between p.start and p.end);");
	}

	// SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	rs.columnTypeIndex.insert(pair<string, int>("statement", 0));
	rs.columnTypeIndex.insert(pair<string, int>("procedure", 1));
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Database::GetModifiesForUnknownInput1(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore &rs) { // for cases like "Modifies(10,v)" . stmt 10 can be if, while, call. we don't know
	SqlResultStore temp;
	sqlResultStoreForCallback = &temp;
	char sqlBuf[512] = {};
	bool ret = false;
	if (isdigit(input1[0])) { // input first char is a digit = statement number
		sprintf_s(sqlBuf, "SELECT entity, text FROM statement WHERE line_num = %s;", input1.c_str());
		sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
		if (errorMessage) { cout << "GetModifiesForUnknownInput1 SQL Error: " << errorMessage; }
		string entity = temp.sqlResult.at(0).row.at("entity");
		string text = temp.sqlResult.at(0).row.at("text");

		// e.g., use(10, v), and stmt 10 is "x = a + b" or "read x". We just need to select from use table with the correct stmtNum to get the variables
		if (entity == "assign") { 
			ret = Database::GetModifiesForAssign(input1, input2, input1IsSpecific, input2IsSpecific, rs); }
		else if (entity == "read") { 
			ret = Database::GetModifiesForRead(input1, input2, input1IsSpecific, input2IsSpecific, rs); }
		else if (entity == "call") { 
			ret = Database::GetModifiesForCall(text, input2, input1IsSpecific, input2IsSpecific, rs); }
		else if (entity == "while") { 
			ret = Database::GetModifiesForWhile(input1, input2, input1IsSpecific, input2IsSpecific, rs); }
		else if (entity == "if") { 
			ret = Database::GetModifiesForIf(input1, input2, input1IsSpecific, input2IsSpecific, rs); }
	}
	else { // input first char is not a digit = a name
		ret = Database::GetModifiesForProcedure(input1, input2, input1IsSpecific, input2IsSpecific, rs);
	}
	return ret;
}

void Database::GetPatternIn(string input1, string input2, SqlResultStore& rs) {
	// reconstruct the variable set to be SQL statement using "where var_name IN (...)"
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select line_num, lhs from pattern p where p.lhs in (%s) and p.rhs like '%s';", input1.c_str(), input2.c_str());
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetPattern SQL Error: " << errorMessage << endl; exit(1); }
}
void Database::GetPatternLike(string input1, string input2, SqlResultStore& rs) {
	// (input is synonym and synonym not in select) or (input is "_") is considered generic input
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select line_num, lhs from pattern p where p.lhs like '%s' and p.rhs like '%s';", input1.c_str(), input2.c_str());
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetPattern SQL Error: " << errorMessage << endl; exit(1); }
}

// get all the columns of PQL select block
//void Database::select(Select& st, SqlResultSet* sqlResultSet) {
void Database::SelectPql(Select& st, SqlResultStore& sqlResultStore, map<string,string> synonymEntityMap) {
	string regexStmtNumEntity = "(stmt|read|print|assign|while|if|call)";
	sqlResultStoreForCallback = &sqlResultStore;
	char sqlBuf[1024] = {};
	int index = 0;
	for (string synonym : st.synonym) {
		string entity = synonymEntityMap.at(synonym);
		if (entity == "procedure") {
			sqlResultStore.columnTypeIndex.insert(pair<string, int>("procedure", index));
			index++;
		}
		else if (regex_match(entity, regex(regexStmtNumEntity))) {
			sqlResultStore.columnTypeIndex.insert(pair<string, int>("statement", index));
			index++;
		}
		else if (entity == "variable") {
			sqlResultStore.columnTypeIndex.insert(pair<string, int>("variable", index));
			index++;
		}
		else if (entity == "constant") {
			sqlResultStore.columnTypeIndex.insert(pair<string, int>("constant", index));
			index++;
		}
	}

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
	
																										/*
	if(whereDuplicateFilter == ""){ sprintf_s(sqlBuf, "SELECT %s FROM %s WHERE %s", selectColumnName.c_str(), selectFromTable.c_str(), whereColumnFilter.c_str()); }
	else{ sprintf_s(sqlBuf, "SELECT %s FROM %s WHERE %s AND %s", selectColumnName.c_str(), selectFromTable.c_str(), whereColumnFilter.c_str(), whereDuplicateFilter.c_str()); }
	*/
	if (st.whereSql.empty()) {	sprintf_s(sqlBuf, "SELECT %s FROM %s", selectColumnName.c_str(), selectFromTable.c_str()); }
	else { sprintf_s(sqlBuf, "SELECT %s FROM %s WHERE %s", selectColumnName.c_str(), selectFromTable.c_str(), whereColumnFilter.c_str()); }
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "PQL select Sql Error: " << errorMessage; return; }
}


// callback method to put one row of results from the database into the dbResults vector
// This method is called each time a row of results is returned from the database
int Database::callback(void* NotUsed, int columnCount, char** columnValues, char** columnNames) {
	NotUsed = 0;
	vector<string> dbRow;
	SqlResult sqlResult;
	// argc is the number of columns for this row of results
	// argv contains the values for the columns
	// Each value is pushed into a vector.
	for (int i = 0; i < columnCount; i++) {
		dbRow.push_back(columnValues[i]);
		sqlResult.row.insert(std::pair<string, string>(columnNames[i], string(columnValues[i])));
	}
	// The row is pushed to the vector for storing all rows of results 
	dbResults.push_back(dbRow);
	sqlResultStoreForCallback->sqlResult.push_back(sqlResult);

	return 0;
}