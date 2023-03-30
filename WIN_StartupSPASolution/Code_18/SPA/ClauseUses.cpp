#pragma once
#include "ClauseUses.h"

// Uses((stmt|print|assign|while|if|call), v)
string Uses::GetUsesConstruct_StatementSynonym_Synonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct s.line_num as %s, u.variable_name as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Uses((stmt|print|assign|while|if|call), "x"/_)
string Uses::GetUsesConstruct_StatementSynonym_NotSynonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct s.line_num as %s, u.variable_name", input1.c_str());
	return string(sql);
}

// Uses(procedure, v)
string Uses::GetUsesConstruct_NameSynonym_Synonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct p.name as %s, u.variable_name as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Uses(procedure, "x"/_)
string Uses::GetUsesConstruct_NameSynonym_NotSynonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct p.name as %s, u.variable_name", input1.c_str());
	return string(sql);
}

// Uses("10", v) where stmt 10 is (stmt|print|assign|while|if|call)
string Uses::GetUsesConstruct_StatementNotSynonym_Synonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct s.line_num, u.variable_name as %s", input2.c_str());
	return string(sql);
}

// Uses("10", "x") where stmt 10 is (stmt|print|assign|while|if|call)
string Uses::GetUsesConstruct_StatementNotSynonym_NotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct s.line_num, u.variable_name");
	return string(sql);
}

// Uses("10", v) where stmt 10 is (stmt|print|assign|while|if|call)
string Uses::GetUsesConstruct_NameNotSynonym_Synonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct p.name, u.variable_name");
	return string(sql);
}

// Uses("10", "x") where stmt 10 is (stmt|print|assign|while|if|call)
string Uses::GetUsesConstruct_NameNotSynonym_NotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct s.line_num, u.variable_name as %s");
	return string(sql);
}

string Uses::GetUses_AnyPrintAssign_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from use u join statement s on s.line_num = u.line_num where s.entity = %s;", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_AnyCall_Any(string frontSql)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from statement s join use u on u.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.entity = 'call';", frontSql.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_AnyProcedure_Any(string frontSql)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from procedure p join use u on u.line_num between p.start and p.end;", frontSql.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_AnyWhileIf_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num = p.line_num join use u on u.line_num between p.line_num and p.child_end where s.entity = '%s'; ", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_AnyPrintAssign_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from use u join statement s on s.line_num = u.line_num where s.entity = '%s' and u.variable_name = '%s'", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_AnyCall_Specific(string frontSql, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from statement s join use u on u.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.entity = 'call' and u.variable_name = '%s'", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_AnyProcedure_Specific(string frontSql, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from procedure p join use u on u.line_num between p.start and p.end where u.variable_name = '%s';", frontSql.c_str(), input2.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_AnyWhileIf_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num = p.line_num join use u on u.line_num between p.line_num and p.child_end where s.entity = '%s' and u.variable_name = '%s'", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_SpecificPrintAssign_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from use u join statement s on s.line_num = u.line_num where s.line_num = %s", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_SpecificCall_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from statement s join use u on u.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.line_num = %s", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_SpecificProcedure_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from procedure p join use u on u.line_num between p.start and p.end where p.name = '%s'", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_SpecificWhileIf_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join use u on u.line_num between p.line_num where p.child_end and p.line_num = %s", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_SpecificPrintAssign_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from use u join statement s on s.line_num = u.line_num where s.line_num = %s and u.variable_name = '%s'", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_SpecificCall_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from statement s join use u on u.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.line_num = %s where u.variable_name = '%s'", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Uses::GetUses_SpecificProcedure_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from procedure p join use u on u.line_num between p.start and p.end where p.name = '%s' and u.variable_name = '%s'", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}


string Uses::GetUses_SpecificWhileIf_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join use u on u.line_num between p.line_num where p.child_end and p.line_num = %s and u.variable_name = '%s'", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}