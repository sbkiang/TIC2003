#pragma once
#include "ClauseCall.h"

// Call(procedure, procedure)
string Call::GetCallConstruct_Synonym_Synonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct c.procedure_name as %s, c.variable_name as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Call(procedure, "Second"/_)
string Call::GetCallConstruct_Synonym_NotSynonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct c.procedure_name as %s", input1.c_str());
	return string(sql);
}

// Call("First"/_, procedure)
string Call::GetCallConstruct_NotSynonym_Synonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct c.variable_name as %s", input2.c_str());
	return string(sql);
}

//Call(_, "Second")
string Call::GetCallConstruct_Any_Specific()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct c.procedure_name as p");
	return string(sql);
}

//Call("First", _)
string Call::GetCallConstruct_Specific_Any()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct c.variable_name as p");
	return string(sql);
}

// Call("First"/_, "Second"/_)
string Call::GetCallConstruct_NotSynonym_NotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct c.procedure_name, c.variable_name");
	return string(sql);
}

// Call(_,_)
string Call::GetCall_Any_Any(string frontSql) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from call c and direct_call = 1", frontSql.c_str());
	return string(sqlBuf);
}

// Call(_, procedure)
string Call::GetCall_Any_Synonym(string frontSql) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.variable_name in (select name FROM procedure) and c.direct_call = 1", frontSql.c_str());
	return string(sqlBuf);
}

// Call(_, "Second")
string Call::GetCall_Any_Specific(string frontSql, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.variable_name = '%s' and direct_call = 1", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

// Call(procedure, _)
string Call::GetCall_Synonym_Any(string frontSql) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.procedure_name in (select name FROM procedure) and c.direct_call = 1", frontSql.c_str());
	return string(sqlBuf);
}

// Call("First", _)
string Call::GetCall_Specific_Any(string frontSql, string input1) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.procedure_name = '%s' and direct_call = 1", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

// Call(procedure, procedure)
string Call::GetCall_Synonym_Synonym(string frontSql) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.procedure_name in (select name from procedure) and c.direct_call = 1", frontSql.c_str());
	return string(sqlBuf);
}

// Call(procedure, "Second")
string Call::GetCall_Synonym_Specific(string frontSql, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.procedure_name in (select name from procedure) and c.variable_name = '%s' and direct_call = 1", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

// Call("First", procedure)
string Call::GetCall_Specific_Synonym(string frontSql, string input1) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.procedure_name = '%s' and c.variable_name in (select name from procedure) and direct_call = 1", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

// Call("First", "Second")
string Call::GetCall_Specific_Specific(string frontSql, string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.procedure_name = '%s' and c.variable_name = '%s' and direct_call = 1", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

// Call*(_,_)
string Call::GetCallT_Any_Any(string frontSql) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from call c", frontSql.c_str());
	return string(sqlBuf);
}

// Call*(_, procedure)
string Call::GetCallT_Any_Synonym(string frontSql) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.variable_name in (select name FROM procedure)", frontSql.c_str());
	return string(sqlBuf);
}

// Call*(_, "Second")
string Call::GetCallT_Any_Specific(string frontSql, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.variable_name = '%s'", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

// Call*(procedure, _)
string Call::GetCallT_Synonym_Any(string frontSql) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.procedure_name in (select name FROM procedure)", frontSql.c_str());
	return string(sqlBuf);
}

// Call*("First", _)
string Call::GetCallT_Specific_Any(string frontSql, string input1) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.procedure_name = '%s'", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

// Call*(procedure, procedure)
string Call::GetCallT_Synonym_Synonym(string frontSql) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.procedure_name in (select name from procedure)", frontSql.c_str());
	return string(sqlBuf);
}

// Call*(procedure, "Second")
string Call::GetCallT_Synonym_Specific(string frontSql, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.procedure_name in (select name from procedure) and c.variable_name = '%s'", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

// Call*("First", procedure)
string Call::GetCallT_Specific_Synonym(string frontSql, string input1) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.procedure_name = '%s' and c.variable_name in (select name from procedure)", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

// Call*("First", "Second")
string Call::GetCallT_Specific_Specific(string frontSql, string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.procedure_name = '%s' and c.variable_name = '%s'", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}