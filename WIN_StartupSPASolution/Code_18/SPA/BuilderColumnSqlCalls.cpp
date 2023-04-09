#include "BuilderColumnSqlCalls.h"

// Call(procedure, procedure)
string BuilderColumnSqlCalls::Build_NameSynonym_NameSynonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct caller as %s, callee as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Call(procedure, "Second"/_)
string BuilderColumnSqlCalls::Build_NameSynonym_NameNotSynonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct caller as %s", input1.c_str());
	return string(sql);
}

// Call("First"/_, procedure)
string BuilderColumnSqlCalls::Build_NameNotSynonym_NameSynonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct callee as %s", input2.c_str());
	return string(sql);
}

// Call("First"/_, "Second"/_)
string BuilderColumnSqlCalls::Build_NameNotSynonym_NameNotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct caller, callee");
	return string(sql);
}

BuilderColumnSqlCalls::BuilderColumnSqlCalls(ClRelRef re)
{
	_re = re;
}

string BuilderColumnSqlCalls::GetSql(DescriberClRelRef describer)
{
	string input1 = _re.GetInput1();
	string input2 = _re.GetInput2();
	bool input1IsSyn = describer.Input1IsSyn();
	bool input2IsSyn = describer.Input2IsSyn();
	input1 = _re.GetInput1Unquoted();
	input2 = _re.GetInput2Unquoted();
	if (input1IsSyn && input2IsSyn) { // (entity, entity)
		return Build_NameSynonym_NameSynonym(input1, input2);
	}
	else if (!input1IsSyn && input2IsSyn) { // (10, entity)
		return Build_NameNotSynonym_NameSynonym(input2);
	}
	else if (input1IsSyn && !input2IsSyn) { // (entity, 10)
		return Build_NameSynonym_NameNotSynonym(input1);
	}
	else if (!input1IsSyn && !input2IsSyn) { // (10, 11)
		return Build_NameNotSynonym_NameNotSynonym();
	}
	return string();
}
