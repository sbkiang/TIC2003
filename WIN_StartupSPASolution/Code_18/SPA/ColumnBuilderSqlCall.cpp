#include "ColumnBuilderSqlCall.h"

// Call(procedure, procedure)
string ColumnBuilderSqlCall::Build_NameSynonym_NameSynonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct caller as %s, callee as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Call(procedure, "Second"/_)
string ColumnBuilderSqlCall::Build_NameSynonym_NameNotSynonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct caller as %s", input1.c_str());
	return string(sql);
}

// Call("First"/_, procedure)
string ColumnBuilderSqlCall::Build_NameNotSynonym_NameSynonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct callee as %s", input2.c_str());
	return string(sql);
}

// Call("First"/_, "Second"/_)
string ColumnBuilderSqlCall::Build_NameNotSynonym_NameNotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct caller, callee");
	return string(sql);
}

string ColumnBuilderSqlCall::GetSqlColumnQuery(RelEnt re, map<string,string> synEntMap)
{
	string input1 = re.GetInput1();
	string input2 = re.GetInput2();
	bool input1IsSyn = (synEntMap.find(input1) != synEntMap.end());
	bool input2IsSyn = (synEntMap.find(input2) != synEntMap.end());
	input1 = re.GetInput1Unquoted();
	input2 = re.GetInput2Unquoted();
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
}
