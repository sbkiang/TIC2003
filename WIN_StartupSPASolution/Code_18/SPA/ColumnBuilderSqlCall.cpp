#include "ColumnBuilderSqlCall.h"

ColumnBuilderSqlCall::ColumnBuilderSqlCall(RelEnt re)
{
	_re = re;
}

string ColumnBuilderSqlCall::Build_NameSynonym_NameSynonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct p.line_num as %s, s.line_num as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Parent((while|if), 10)
string ColumnBuilderSqlCall::Build_NameSynonym_NameNotSynonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct p.line_num as %s", input1.c_str());
	return string(sql);
}

// Parent(10, (Name|read|print|assign|while|if|call))
string ColumnBuilderSqlCall::Build_NameNotSynonym_NameSynonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct s.line_num as %s", input2.c_str());
	return string(sql);
}

// Parent(10, 12)
string ColumnBuilderSqlCall::Build_NameNotSynonym_NameNotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct p.line_num, s.line_num");
	return string(sql);
}

string ColumnBuilderSqlCall::GetSqlColumnQuery(map<string,string> synEntMap)
{
	string input1 = _re.GetInput1();
	string input2 = _re.GetInput2();
	bool input1IsSyn = (synEntMap.find(input1) != synEntMap.end());
	bool input2IsSyn = (synEntMap.find(input2) != synEntMap.end());
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
