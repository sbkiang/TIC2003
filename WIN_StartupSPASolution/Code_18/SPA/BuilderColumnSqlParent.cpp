#include "BuilderColumnSqlParent.h"

string BuilderColumnSqlParent::Build_StmtSynonym_StmtSynonym(string input1, string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct parent as %s, child as %s", input1.c_str(), input2.c_str());
	return string(sql);
}

// Parent((while|if), 10)
string BuilderColumnSqlParent::Build_StmtSynonym_StmtNotSynonym(string input1)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct parent as %s", input1.c_str());
	return string(sql);
}

// Parent(10, (stmt|read|print|assign|while|if|call))
string BuilderColumnSqlParent::Build_StmtNotSynonym_StmtSynonym(string input2)
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct child as %s", input2.c_str());
	return string(sql);
}

// Parent(10, 12)
string BuilderColumnSqlParent::Build_StmtNotSynonym_StmtNotSynonym()
{
	char sql[100] = {};
	sprintf_s(sql, "select distinct parent, child");
	return string(sql);
}

BuilderColumnSqlParent::BuilderColumnSqlParent(ClRelRef re)
{
	_re = re;
}

string BuilderColumnSqlParent::GetSqlQuery(DescriberClRelRef describer)
{
	string input1 = _re.GetInput1();
	string input2 = _re.GetInput2();
	bool input1IsSyn = describer.Input1IsSyn();
	bool input2IsSyn = describer.Input2IsSyn();
	input1 = _re.GetInput1Unquoted();
	input2 = _re.GetInput2Unquoted();
	if (input1IsSyn && input2IsSyn) { // (entity, entity)
		return Build_StmtSynonym_StmtSynonym(input1, input2);
	}
	else if (!input1IsSyn && input2IsSyn) { // (10, entity)
		return Build_StmtNotSynonym_StmtSynonym(input2);
	}
	else if (input1IsSyn && !input2IsSyn) { // (entity, 10)
		return Build_StmtSynonym_StmtNotSynonym(input1);
	}
	else if (!input1IsSyn && !input2IsSyn) { // (10, 11)
		return Build_StmtNotSynonym_StmtNotSynonym();
	}
	return string();
}
