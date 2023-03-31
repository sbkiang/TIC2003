#pragma once
#include "ClauseParent.h"

// Parent((while | if), (stmt | read | print | assign | while | if | call))
string Parent::GetParentConstruct_Synonym_Synonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select p.line_num as %s, s.line_num as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Parent((while|if), 10)
string Parent::GetParentConstruct_Synonym_NotSynonym(string input1)
{
	char sql[100] = {};
	//sprintf_s(sql, "select p.line_num as %s, s.line_num", input1.c_str());
	sprintf_s(sql, "select p.line_num as %s", input1.c_str());
	return string(sql);
}

// Parent(10, (stmt|read|print|assign|while|if|call))
string Parent::GetParentConstruct_NotSynonym_Synonym(string input2)
{
	char sql[100] = {};
	//sprintf_s(sql, "select p.line_num, s.line_num as %s", input2.c_str());
	sprintf_s(sql, "select s.line_num as %s", input2.c_str());
	return string(sql);
}

// Parent(10, 12)
string Parent::GetParentConstruct_NotSynonym_NotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select p.line_num, s.line_num");
	return string(sql);
}

string Parent::GetParent_Any_Any(string frontSql) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str());
	return string(sqlBuf);
}

// Parent(stmt/_, read/print/assign/while/if/call)
string Parent::GetParent_Any_Synonym(string frontSql, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from statement s join parent p on s.line_num between p.child_start and p.child_end and s.entity = '%s' and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

// Parent(stmt/_, 10)
string Parent::GetParent_Any_Specific(string frontSql, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from statement s join parent p on s.line_num between p.child_start and p.child_end and s.line_num = %s order by p.line_num desc limit 1", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

// Parent(while/if, stmt/_)
string Parent::GetParent_Synonym_Any(string frontSql, string input1) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num in (select line_num from statement where entity = '%s') and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

// Parent(while/if, read/print/assign/while/if/call)
string Parent::GetParent_Synonym_Synonym(string frontSql, string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where s.entity = '%s' and p.line_num in (select line_num from statement where entity = '%s') and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str(), input2.c_str(), input1.c_str());
	return string(sqlBuf);
}

// Parent(while/if, 10)
string Parent::GetParent_Synonym_Specific(string frontSql, string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where s.line_num = %s and p.line_num in (select line_num from statement where entity = '%s') and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str(), input2.c_str(), input1.c_str());
	return string(sqlBuf);
}

// Parent(10, stmt/_)
string Parent::GetParent_Specific_Any(string frontSql, string input1) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = %s and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

// Parent(10, read/print/assign/while/if/call)
string Parent::GetParent_Specific_Synonym(string frontSql, string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = %s and s.entity = '%s' and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

// Parent(10, 10)
string Parent::GetParent_Specific_Specific(string frontSql, string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end and p.line_num = %s and s.line_num = %s", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Parent::GetParentT_Any_Any(string frontSql)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end", frontSql.c_str());
	return string(sqlBuf);
}

string Parent::GetParentT_Any_Synonym(string frontSql, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from statement s join parent p on s.line_num between p.child_start and p.child_end and s.entity = '%s'", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Parent::GetParentT_Any_Specific(string frontSql, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from statement s join parent p on s.line_num between p.child_start and p.child_end and s.line_num = %s", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Parent::GetParentT_Synonym_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num in (select line_num from statement where entity = '%s')", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Parent::GetParentT_Synonym_Synonym(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where s.entity = '%s' and p.line_num in (select line_num from statement where entity = '%s')", frontSql.c_str(), input2.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Parent::GetParentT_Synonym_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where s.line_num = %s and p.line_num in (select line_num from statement where entity = '%s')", frontSql.c_str(), input2.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Parent::GetParentT_Specific_Any(string frontSql, string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = %s", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Parent::GetParentT_Specific_Synonym(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = %s and s.entity = '%s'", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Parent::GetParentT_Specific_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end and p.line_num = %s and s.line_num = %s", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}