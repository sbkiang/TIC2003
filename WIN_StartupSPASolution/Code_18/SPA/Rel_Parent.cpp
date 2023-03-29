#pragma once
#include "Rel_Parent.h"

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
	sprintf_s(sql, "select p.line_num as %s, s.line_num", input1.c_str());
	return string(sql);
}

// Parent(10, (stmt|read|print|assign|while|if|call))
string Parent::GetParentConstruct_NotSynonym_Synonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select p.line_num, s.line_num as %s", input2.c_str());
	return string(sql);
}

// Parent(10, 12)
string Parent::GetParentConstruct_NotSynonym_NotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select p.line_num, s.line_num");
	return string(sql);
}

bool Parent::GetParentAnyAny(string frontSql, SqlResultStore& rs) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num  as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

// Parent(stmt/_, read/print/assign/while/if/call)
bool Parent::GetParentAnySynonym(string frontSql, string input2, SqlResultStore& rs) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from statement s join parent p on s.line_num between p.child_start and p.child_end and s.entity = '%s' and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str(), input2.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

// Parent(stmt/_, 10)
bool Parent::GetParentAnySpecific(string frontSql, string input2, SqlResultStore& rs) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from statement s join parent p on s.line_num between p.child_start and p.child_end and s.line_num = %s order by p.line_num desc limit 1", frontSql.c_str(), input2.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

// Parent(while/if, stmt/_)
bool Parent::GetParentSynonymAny(string frontSql, string input1, SqlResultStore& rs) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num in (select line_num from statement where entity = '%s') and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str(), input1.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

// Parent(while/if, read/print/assign/while/if/call)
bool Parent::GetParentSynonymSynonym(string frontSql, string input1, string input2, SqlResultStore& rs) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num  as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where s.entity = '%s' and p.line_num in (select line_num from statement where entity = '%s') and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str(), input2.c_str(), input1.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

// Parent(while/if, 10)
bool Parent::GetParentSynonymSpecific(string frontSql, string input1, string input2, SqlResultStore& rs) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num  as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where s.line_num = %s and p.line_num in (select line_num from statement where entity = '%s') and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str(), input2.c_str(), input1.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

// Parent(10, stmt/_)
bool Parent::GetParentSpecificAny(string frontSql, string input1, SqlResultStore& rs) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num  as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = %s and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str(), input1.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

// Parent(10, read/print/assign/while/if/call)
bool Parent::GetParentSpecificSynonym(string frontSql, string input1, string input2, SqlResultStore& rs) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num  as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = %s and s.entity = '%s' and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", frontSql.c_str(), input1.c_str(), input2.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

// Parent(10, 10)
bool Parent::GetParentSpecificSpecific(string frontSql, string input1, string input2, SqlResultStore& rs) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num  as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end and p.line_num = %s and s.line_num = %s", frontSql.c_str(), input1.c_str(), input2.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

bool Parent::GetParentTAnyAny(string frontSql, SqlResultStore& rs)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num  as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end", frontSql.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

bool Parent::GetParentTAnySynonym(string frontSql, string input2, SqlResultStore& rs)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from statement s join parent p on s.line_num between p.child_start and p.child_end and s.entity = '%s'", frontSql.c_str(), input2.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

bool Parent::GetParentTAnySpecific(string frontSql, string input2, SqlResultStore& rs)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from statement s join parent p on s.line_num between p.child_start and p.child_end and s.line_num = %s", frontSql.c_str(), input2.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

bool Parent::GetParentTSynonymAny(string frontSql, string input1, SqlResultStore& rs)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num in (select line_num from statement where entity = '%s')", frontSql.c_str(), input1.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

bool Parent::GetParentTSynonymSynonym(string frontSql, string input1, string input2, SqlResultStore& rs)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where s.entity = '%s' and p.line_num in (select line_num from statement where entity = '%s')", frontSql.c_str(), input2.c_str(), input1.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

bool Parent::GetParentTSynonymSpecific(string frontSql, string input1, string input2, SqlResultStore& rs)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where s.line_num = %s and p.line_num in (select line_num from statement where entity = '%s')", frontSql.c_str(), input2.c_str(), input1.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

bool Parent::GetParentTSpecificAny(string frontSql, string input1, SqlResultStore& rs)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = %s", frontSql.c_str(), input1.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

bool Parent::GetParentTSpecificSynonym(string frontSql, string input1, string input2, SqlResultStore& rs)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = %s and s.entity = '%s'", frontSql.c_str(), input1.c_str(), input2.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}

bool Parent::GetParentTSpecificSpecific(string frontSql, string input1, string input2, SqlResultStore& rs)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "%s from parent p join statement s on s.line_num between p.child_start and p.child_end and p.line_num = %s and s.line_num = %s", frontSql.c_str(), input1.c_str(), input2.c_str());
	return Database::ExecuteSql(string(sqlBuf), rs);
}