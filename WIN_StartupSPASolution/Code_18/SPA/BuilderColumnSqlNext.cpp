#include "BuilderColumnSqlNext.h"

string BuilderColumnSqlNext::Build_StmtSynonym_StmtSynonym(string synonym1, string synonym2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select distinct from_line as %s, to_line as %s", synonym1.c_str(), synonym2.c_str());
	return string(sqlBuf);
}

string BuilderColumnSqlNext::Build_StmtSynonym_StmtNotSynonym(string synonym1)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select distinct from_line as %s", synonym1.c_str());
	return string(sqlBuf);
}

string BuilderColumnSqlNext::Build_StmtNotSynonym_StmtSynonym(string synonym2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select distinct to_line as %s", synonym2.c_str());
	return string(sqlBuf);
}

string BuilderColumnSqlNext::Build_StmtNotSynonym_StmtNotSynonym()
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select distinct from_line, to_line");
	return string(sqlBuf);
}

BuilderColumnSqlNext::BuilderColumnSqlNext(ClRelRef re)
{
	_re = re;
}

string BuilderColumnSqlNext::GetSql(DescriberClRelRef describer)
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
