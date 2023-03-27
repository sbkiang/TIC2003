#pragma once
#include "Uses.h"
/*
bool Uses::GetUsesForAssign(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	// select 1 from use u where u.variable_name = '%s' and u.line_num in (select s.line_num from statement s where entity = 'assign' and s.line_num = '%s');

	char sqlBuf[512] = {};
	if (!input1IsSpecific && input1 != "_") { // input1 is synonym
		sprintf_s(sqlBuf, "select %s", input1.c_str());
	}
	else {
		sprintf_s(sqlBuf, "select line_num"); // input1 not synonym
	}
	if (!input2IsSpecific && input2 != "_") {
		sprintf_s(sqlBuf, "%s ,%s", sqlBuf, input2.c_str()); // select v1,v2
	}
	else {
		sprintf_s(sqlBuf, "%s, variable_name"); // select line_num, variable_name
	}

	// Uses(a,"cenX") or Uses(a,v) where "a" is "assign a", "v" is "variable v", "a" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "%s from use u where u.variable_name = '%s' and u.line_num in (select s.line_num from statement s where entity = 'assign');", sqlBuf, input2.c_str());
	}

	// Uses(a,"cenX") or Uses(a,v) where "a" is "assign a", "v" is "variable v", both present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "%s from use u where u.variable_name = '%s' and u.line_num in (select s.line_num from statement s where entity = 'assign' and s.line_num = %s);", sqlBuf, input2.c_str(), input1.c_str());
	}

	// Uses(a,v) where "a" is assign a,"v" is variable v, only "a" present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "%s from use u where u.line_num in (select s.line_num from statement s where entity = 'assign' and s.line_num = %s);", sqlBuf, input1.c_str());
	}

	// Uses(a,v) where "a" is assign a, "v" is variable, both not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "%s from use u where u.line_num in (select s.line_num from statement s where entity = 'assign');", sqlBuf);
	}

	//SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	Database::sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetUsesForAssign SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Uses::GetUsesForPrint(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	// select 1 from use u where u.variable_name = '%s' and u.line_num in (select s.line_num from statement s where entity = 'print' and s.line_num = '%s');

	char sqlBuf[512] = {};

	if (!input1IsSpecific && input1 != "_") { // input1 is synonym
		sprintf_s(sqlBuf, "select %s", input1.c_str());
	}
	else {
		sprintf_s(sqlBuf, "select line_num"); // input1 not synonym
	}
	if (!input2IsSpecific && input2 != "_") {
		sprintf_s(sqlBuf, "%s ,%s", sqlBuf, input2.c_str()); // select v1,v2
	}
	else {
		sprintf_s(sqlBuf, "%s, variable_name"); // select line_num, variable_name
	}

	// Uses(p,"cenX") or Uses(p,v) where "p" is "print p", "v" is "variable v", "p" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where u.variable_name = '%s' and u.line_num in (select s.line_num from statement s where entity = 'print');", input2.c_str());
	}

	// Uses(p,"cenX") or Uses(p,v) where "p" is "print p", "v" is "variable v", both present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where u.variable_name = '%s' and u.line_num in (select s.line_num from statement s where entity = 'print' and s.line_num = %s);", input2.c_str(), input1.c_str());
	}

	// Uses(p,"cenX") or Uses(p,v) where "p" is "print p", "v" is "variable v", only "p" present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where u.line_num in (select s.line_num from statement s where entity = 'print' and s.line_num = %s);", input1.c_str());
	}

	// Uses(p,"cenX") or Uses(p,v) where "p" is "print p", "v" is "variable v", both not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where u.line_num in (select s.line_num from statement s where entity = 'print');");
	}

	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetUsesForPrint SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Uses::GetUsesForWhile(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	//select 1 from use u where u.variable_name = '%s' and exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'while' and u.line_num between p.line_num and p.child_end)

	char sqlBuf[512] = {};
	// Uses(w,"cenX") or Uses(w,v) is true, where "w" is while w, "v" is variable v, "w" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		//sprintf_s(sqlBuf, "SELECT p.line_num FROM parent p JOIN statement s ON p.line_num = s.line_num WHERE s.entity = 'while' AND EXISTS (SELECT 1 FROM use u WHERE u.line_num BETWEEN p.line_num AND p.child_end AND u.variable_name = '%s');", input2.c_str());
		sprintf_s(sqlBuf, "select * from use u where u.variable_name = '%s' and exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'while' and u.line_num between p.line_num and p.child_end);", input2.c_str());
	}

	// Uses(w,"cenX") or Uses(w,v) is true, where "w" is while w, "v" is variable v, and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where u.variable_name = '%s' and exists (select 1 from parent p where p.line_num = %s and u.line_num between p.line_num and p.child_end);", input2.c_str(), input1.c_str());
	}

	// Uses(w,v) where "w" is while w,"v" is variable v, and only "w" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where exists (select 1 from parent p where p.line_num = %s and u.line_num between p.line_num and p.child_end);", input1.c_str());
	}

	// we are looking for each statement and checking if Uses(pn,v) is true, where "pn" is assign pn, "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'while' and u.line_num between p.line_num and p.child_end);");
	}

	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetUsesForWhile SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Uses::GetUsesForIf(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	//select 1 from use u where u.variable_name = '%s' and exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'if' and u.line_num between p.line_num and p.child_end)

	char sqlBuf[512] = {};


	// Uses(i,"cenX") or Uses(i,v) where "i" is "if i", "v" is "variable v", "i" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where u.variable_name = '%s' and exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'if' and u.line_num between p.line_num and p.child_end);", input2.c_str());
	}

	// Uses(i,"cenX") or Uses(i,v) where "i" is "if i", "v" is "variable v", and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where u.variable_name = '%s' and exists (select 1 from parent p where p.line_num = %s and u.line_num between p.line_num and p.child_end);", input2.c_str(), input1.c_str());
	}

	// Uses(i,v) where "i" is "if i", "v" is "variable v", and only "i" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where exists (select 1 from parent p where p.line_num = %s and u.line_num between p.line_num and p.child_end);", input1.c_str());
	}

	// Uses(pn,v) where "pn" is "if i", "v" "is variable", and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where exists (select 1 from parent p join statement s on p.line_num = s.line_num where s.entity = 'if' and u.line_num between p.line_num and p.child_end);");
	}

	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetUsesForIf SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Uses::GetUsesForCall(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	//select 1 from use u where u.variable_name = '%s' and exists (select 1 from statement s where s.entity = 'call' and s.text = '%s' and s.line_num = u.line_num)

	char sqlBuf[512] = {};

	// Uses(c,"cenX") or Uses(c,v) is true, where "c" is "call c", "v" is variable v, "c" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where u.variable_name = '%s' and exists (select 1 from statement s where s.entity = 'call' and s.line_num = u.line_num);", input2.c_str());
	}

	// Uses(c,"cenX") or Uses(c,v) is true, where "c" is "call c", "v" is "variable v", and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where u.variable_name = '%s' and exists (select 1 from statement s where s.entity = 'call' and s.line_num = %s and s.line_num = u.line_num);", input2.c_str(), input1.c_str());
	}

	// Uses(c,v) where "c" is "call c", "v" is "variable v", and only "c" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where exists (select 1 from statement s where s.entity = 'call' and s.line_num = %s and s.line_num = u.line_num);", input1.c_str());
	}

	// we are looking for each statement and checking if Uses(c,v) is true, where "c" is "call c", "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where exists (select 1 from statement s where s.entity = 'call' and s.line_num = u.line_num);");
	}

	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetUsesForCall SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Uses::GetUsesForProcedure(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) {
	//select 1 from use u where u.variable_name = '%s' and exists (select 1 from procedure p where p.name = '%s' and u.line_num between p.start and p.end)

	char sqlBuf[512] = {};

	// Uses(p,"cenX") or Uses(p,v) is true, where "p" is "procedure p", "v" is variable v, "p" not present in select, "v" present in select
	if (!input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where u.variable_name = '%s' and exists (select 1 from procedure p where u.line_num between p.start and p.end);", input2.c_str());
	}

	// Uses(p,"cenX") or Uses(p,v) is true, where "p" is "procedure p", "v" is "variable v", and both are present in select
	else if (input1IsSpecific && input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where u.variable_name = '%s' and exists (select 1 from procedure p where p.name = '%s' and u.line_num between p.start and p.end);", input2.c_str(), input1.c_str());
	}

	// Uses(p,v) where "p" is "procedure p", "v" is "variable v", and only "p" is present in select
	else if (input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where exists (select 1 from procedure p where p.name = '%s' and u.line_num between p.start and p.end);", input1.c_str());
	}

	// we are looking for each statement and checking if Uses(c,v) is true, where "c" is "call c", "v" is variable, and both are not present in select
	else if (!input1IsSpecific && !input2IsSpecific) {
		sprintf_s(sqlBuf, "select * from use u where exists (select 1 from procedure p where u.line_num between p.start and p.end);");
	}
	SqlResultStore rs;
	sqlResultStoreForCallback = &rs;
	sqlite3_exec(dbConnection, sqlBuf, callback, 0, &errorMessage);
	if (errorMessage) { cout << "GetUsesForProcedure SQL Error: " << errorMessage; exit(1); }
	return (!(sqlResultStoreForCallback->sqlResult.empty()));
}

bool Uses::GetUsesForUnknownInput1(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs) { // for cases like "uses(10,v)" . stmt 10 can be if, while, call. we don't know
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
			ret = Uses::GetUsesForAssign(input1, input2, input1IsSpecific, input2IsSpecific);
		}
		else if (entity == "print") {
			ret = Uses::GetUsesForPrint(input1, input2, input1IsSpecific, input2IsSpecific);
		}
		else if (entity == "call") {
			ret = Uses::GetUsesForCall(text, input2, input1IsSpecific, input2IsSpecific);
		}
		else if (entity == "while") {
			ret = Uses::GetUsesForWhile(input1, input2, input1IsSpecific, input2IsSpecific);
		}
		else if (entity == "if") {
			ret = Uses::GetUsesForIf(input1, input2, input1IsSpecific, input2IsSpecific);
		}
	}
	else { // input first char is not a digit = a name
		ret = Uses::GetUsesForProcedure(input1, input2, input1IsSpecific, input2IsSpecific);
	}
	return ret;
}
*/