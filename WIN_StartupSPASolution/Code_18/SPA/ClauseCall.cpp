#pragma once
#include "ClauseCall.h"

// Call(procedure, procedure)
string Call::GetCallConstruct_Synonym_Synonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select c.line_num");
	return string(sql);
}

// Call(procedure, "Second"/_)
string Call::GetCallConstruct_Synonym_NotSynonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select c.line_num as %s", input1.c_str());
	return string(sql);
}

// Parent(10, (stmt|read|print|assign|while|if|call))
string Call::GetCallConstruct_NotSynonym_Synonym(string input2)
{
	char sql[100] = {};
	//sprintf_s(sql, "select p.line_num, s.line_num as %s", input2.c_str());
	sprintf_s(sql, "select s.line_num as %s", input2.c_str());
	return string(sql);
}

// Parent(10, 12)
string Call::GetCallConstruct_NotSynonym_NotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select p.line_num, s.line_num");
	return string(sql);
}