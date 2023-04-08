#include "DescriberClSelect.h"

DescriberClSelect::DescriberClSelect(ClSelect clSelect, map<string,string> synEntMap)
{


	_tableSql = tableSql;
	_whereSql = whereSql;
	_columnSql = columnSql;
	_asSql = asSql;
}

vector<string> DescriberClSelect::GetTableSql()
{
	return _tableSql;
}

vector<string> DescriberClSelect::GetWhereSql()
{
	return _whereSql;
}

vector<string> DescriberClSelect::GetColumnSql()
{
	return _columnSql;
}

vector<string> DescriberClSelect::GetAsSql()
{
	return _asSql;
}
