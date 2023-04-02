#pragma once
#include "ClauseCall.h"

// Call(procedure, procedure)
string Call::GetCallConstruct_Synonym_Synonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select c.procedure_name as %s, variable_name as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Call(procedure, "Second"/_)
string Call::GetCallConstruct_Synonym_NotSynonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select c.procedure_name as %s", input1.c_str());
	return string(sql);
}

// Call("First"/_, procedure)
string Call::GetCallConstruct_NotSynonym_Synonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select c.variable_name as %s", input2.c_str());
	return string(sql);
}

// Call("First"/_, "Second"/_)
string Call::GetCallConstruct_NotSynonym_NotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select c.procedure_name, c.variable_name");
	return string(sql);
}

// Call(_,_)
string Call::GetCall_Any_Any(string frontSql) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from call c", frontSql.c_str());
	return string(sqlBuf);
}

// Call(_, procedure)
string Call::GetCall_Any_Synonym(string frontSql, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.variable_name = '%s'", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

// Call(procedure, _)
string Call::GetCall_Synonym_Any(string frontSql, string input1) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.procedure_name = '%s'", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}


