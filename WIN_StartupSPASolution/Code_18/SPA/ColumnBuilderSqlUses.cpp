#include "ColumnBuilderSqlUses.h"

// Uses((stmt | print | assign | while | if | call), v)
string ColumnBuilderSqlUses::Build_StmtSynonym_NameSynonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct line_num as %s, variable_name as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Uses((stmt|print|assign|while|if|call), "x"/_)
string ColumnBuilderSqlUses::Build_StmtSynonym_NameNotSynonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct line_num as %s", input1.c_str());
	return string(sql);
}

// Uses(procedure, v)
string ColumnBuilderSqlUses::Build_NameSynonym_NameSynonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct name as %s, variable_name as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Uses(procedure, "x"/_)
string ColumnBuilderSqlUses::Build_NameSynonym_NameNotSynonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct name as %s", input1.c_str());
	return string(sql);
}

// Uses("10", v) where stmt 10 is (stmt|print|assign|while|if|call)
string ColumnBuilderSqlUses::Build_StmtNotSynonym_NameSynonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct variable_name as %s", input2.c_str());
	return string(sql);
}

// Uses("10", "x") where stmt 10 is (stmt|print|assign|while|if|call)
string ColumnBuilderSqlUses::Build_StmtNotSynonym_NameNotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct line_num, variable_name");
	return string(sql);
}

// Uses("main", v)
string ColumnBuilderSqlUses::Build_NameNotSynonym_NameSynonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct name, variable_name as %s", input2.c_str());
	return string(sql);
}

// Uses("main", "x")
string ColumnBuilderSqlUses::Build_NameNotSynonym_NameNotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct name, variable_name");
	return string(sql);
}

ColumnBuilderSqlUses::ColumnBuilderSqlUses(ClRelRef re)
{
	_re = re;
}

string ColumnBuilderSqlUses::GetSqlQuery(DescriberClRelRef describer)
{
	string input1 = _re.GetInput1();
	string input2 = _re.GetInput2();
	bool input1IsSyn = describer.Input1IsSyn();
	bool input2IsSyn = describer.Input2IsSyn();
	input1 = _re.GetInput1Unquoted();
	input2 = _re.GetInput2Unquoted();
	string entityInput1 = describer.EntityInput1();
	if (input1IsSyn && input2IsSyn) {
		if (regex_match(entityInput1, regex(regexStmtNumEntity))) {
			return ColumnBuilderSqlUses::Build_StmtSynonym_NameSynonym(input1, input2);
		}
		else if (entityInput1 == "procedure") {
			return ColumnBuilderSqlUses::Build_NameSynonym_NameSynonym(input1, input2);
		}
	}
	else if (!input1IsSyn && input2IsSyn) {
		if (isdigit(input1[0])) {
			return ColumnBuilderSqlUses::Build_StmtNotSynonym_NameSynonym(input2);
		}
		else {
			return ColumnBuilderSqlUses::Build_NameNotSynonym_NameSynonym(input2);
		}
	}
	else if (input1IsSyn && !input2IsSyn) {
		if (regex_match(entityInput1, regex(regexStmtNumEntity))) {
			return ColumnBuilderSqlUses::Build_StmtSynonym_NameNotSynonym(input1);
		}
		else if (entityInput1 == "procedure") {
			return ColumnBuilderSqlUses::Build_NameSynonym_NameNotSynonym(input1);
		}
	}
	else if (!input1IsSyn && !input2IsSyn) {
		if (isdigit(input1[0])) {
			return ColumnBuilderSqlUses::Build_StmtNotSynonym_NameNotSynonym();
		}
		else {
			return ColumnBuilderSqlUses::Build_NameNotSynonym_NameNotSynonym();
		}
	}

	return string();
}

