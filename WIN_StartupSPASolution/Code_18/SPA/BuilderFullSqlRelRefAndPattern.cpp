#include "BuilderFullSqlRelRefAndPattern.h"

string BuilderFullSqlRelRefAndPattern::BuildSql(string colSql, string querySql)
{
	char sqlBuf[700];
	sprintf_s(sqlBuf, "%s from (%s)", colSql.c_str(), querySql.c_str());
	return string(sqlBuf);
}
