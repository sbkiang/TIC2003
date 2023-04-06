#pragma once
#include "ClauseModifies.h"

string Modifies::insertModifies(int stmtNum, string variablename)
{
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO modify ('line_num','variable_name' ) VALUES ('%i','%s');", stmtNum, variablename.c_str());
	return string(sqlBuf);
}

// Modifies((stmt|read|assign|while|if|call), v)
string Modifies::GetModifiesConstruct_StatementSynonym_Synonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct line_num as %s, variable_name as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Modifies((stmt|read|assign|while|if|call), "x"/_)
string Modifies::GetModifiesConstruct_StatementSynonym_NotSynonym(string input1)
{
	char sql[100] = {};
	//sprintf_s(sql, "select distinct s.line_num as %s, m.variable_name", input1.c_str());
	sprintf_s(sql, "select distinct line_num as %s", input1.c_str());
	return string(sql);
}

// Modifies(procedure, v)
string Modifies::GetModifiesConstruct_NameSynonym_Synonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct name as %s, variable_name as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Modifies(procedure, "x"/_)
string Modifies::GetModifiesConstruct_NameSynonym_NotSynonym(string input1)
{
	char sql[100] = {};
	//sprintf_s(sql, "select distinct p.name as %s, m.variable_name", input1.c_str());
	sprintf_s(sql, "select distinct name as %s", input1.c_str());
	return string(sql);
}

// Modifies("10", v) where stmt 10 is (stmt|read|assign|while|if|call)
string Modifies::GetModifiesConstruct_StatementNotSynonym_Synonym(string input2)
{
	char sql[100] = {};
	//sprintf_s(sql, "select distinct s.line_num, m.variable_name as %s", input2.c_str());
	sprintf_s(sql, "select distinct variable_name as %s", input2.c_str());
	return string(sql);
}

// Modifies("10", "x") where stmt 10 is (stmt|read|assign|while|if|call)
string Modifies::GetModifiesConstruct_StatementNotSynonym_NotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct line_num, variable_name");
	return string(sql);
}

// Modifies("10", v) where stmt 10 is (stmt|read|assign|while|if|call)
string Modifies::GetModifiesConstruct_NameNotSynonym_Synonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct name, variable_name as %s", input2.c_str());
	return string(sql);
}

// Modifies("10", "x") where stmt 10 is (stmt|read|assign|while|if|call)
string Modifies::GetModifiesConstruct_NameNotSynonym_NotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct line_num, variable_name");
	return string(sql);
}

string Modifies::GetModifies_AnyReadAssign_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select m.line_num, m.variable_name from modify m join statement s on s.line_num = m.line_num where s.entity = '%s')", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_AnyCall_Any(string frontSql)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select s.line_num, m.variable_name from statement s join modify m on m.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.entity = 'call')", frontSql.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_AnyProcedure_Any(string frontSql)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select p.name, m.variable_name from procedure p join modify m on m.line_num between p.start and p.end)", frontSql.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_AnyWhileIf_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select p.line_num, m.variable_name from parent p join statement s on s.line_num = p.line_num join modify m on m.line_num between p.line_num and p.child_end where s.entity = '%s')", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_Any_Any(string frontSql)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select p.line_num, m.variable_name from parent p join modify m on m.line_num between p.line_num and p.child_end union select line_num, variable_name from modify)", frontSql.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_AnyReadAssign_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select m.line_num, m.variable_name from modify m join statement s on s.line_num = m.line_num where s.entity = '%s' and m.variable_name = '%s')", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_AnyCall_Specific(string frontSql, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select c.line_num, m.variable_name from call c join modify m on m.line_num between (select start from procedure where name = c.name) and (select end from procedure where name = s.text) where m.variable_name = '%s')", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_AnyProcedure_Specific(string frontSql, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select p.name, m.variable_name from procedure p join modify m on m.line_num between p.start and p.end where m.variable_name = '%s')", frontSql.c_str(), input2.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_AnyWhileIf_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select p.line_num, m.variable_name parent p join statement s on s.line_num = p.line_num join modify m on m.line_num between p.line_num and p.child_end where s.entity = '%s' and m.variable_name = '%s')", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_Any_Specific(string frontSql, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select p.line_num, m.variable_name from parent p join modify m on m.line_num between p.line_num and p.child_end where m.variable_name = '%s' union select line_num, variable_name from modify where variable_name = '%s')", frontSql.c_str(), input2.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_SpecificReadAssign_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select m.line_num, m.variable_name from modify m join statement s on s.line_num = m.line_num where s.line_num = %s)", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_SpecificCall_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select s.line_num, m.variable from statement s join modify m on m.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.line_num = %s)", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_SpecificProcedure_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select p.name, m.variable_name from procedure p join modify m on m.line_num between p.start and p.end where p.name = '%s')", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_SpecificWhileIf_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select p.line_num, m.variable_name from parent p join modify m on m.line_num between p.line_num and p.child_end where p.line_num = %s)", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_SpecificReadAssign_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select m.line_num, m.variable_name from modify m join statement s on s.line_num = m.line_num where s.line_num = %s and m.variable_name = '%s')", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_SpecificCall_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select s.line_num, m.variable_name from statement s join modify m on m.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.line_num = %s where m.variable_name = '%s')", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Modifies::GetModifies_SpecificProcedure_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select p.line_num, m.variable_name from procedure p join modify m on m.line_num between p.start and p.end where p.name = '%s' and m.variable_name = '%s')", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}


string Modifies::GetModifies_SpecificWhileIf_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from (select p.line_num, m.variable_name from parent p join modify m on m.line_num between p.line_num and p.child_end where p.line_num = %s and m.variable_name = '%s')", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}