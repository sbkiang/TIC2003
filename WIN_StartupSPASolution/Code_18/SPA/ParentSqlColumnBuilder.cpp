#include "ParentSqlColumnBuilder.h"

ParentSqlColumnBuilder::ParentSqlColumnBuilder(string syn1, string syn2) : IRelRefSqlColumnBuilder(syn1, syn2)
{
	_syn1 = syn1;
	_syn2 = syn2;
}

string ParentSqlColumnBuilder::Build_Synonym_Synonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct p.line_num as %s, s.line_num as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Parent((while|if), 10)
string ParentSqlColumnBuilder::Build_Synonym_NotSynonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct p.line_num as %s", input1.c_str());
	return string(sql);
}

// Parent(10, (stmt|read|print|assign|while|if|call))
string ParentSqlColumnBuilder::Build_NotSynonym_Synonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct s.line_num as %s", input2.c_str());
	return string(sql);
}

// Parent(10, 12)
string ParentSqlColumnBuilder::Build_NotSynonym_NotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct p.line_num, s.line_num");
	return string(sql);
}

string ParentSqlColumnBuilder::getSqlColumnQuery()
{
	
	return string();
}
