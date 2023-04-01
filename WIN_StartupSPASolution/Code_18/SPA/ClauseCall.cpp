#pragma once
#include "ClauseCall.h"

// Call(procedure, procedure)
string Call::GetCallConstruct_Synonym_Synonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select c.line_num as %s", input1.c_str());
	return string(sql);
}

// Call(procedure, "Second"/_)
string Call::GetCallConstruct_Synonym_NotSynonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select c.line_num as %s", input1.c_str());
	return string(sql);
}

// Call("First"/_, procedure)
string Call::GetCallConstruct_NotSynonym_Synonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select c.line_num as %s", input2.c_str());
	return string(sql);
}

// Call("First"/_, "Second"/_)
string Call::GetCallConstruct_NotSynonym_NotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select c.line_num, c.line_num");
	return string(sql);
}

// Call(_, procedure)
string Call::GetCall_Any_Synonym(string frontSql, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, " %s from call c where c.variable_name = '%s'", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}


