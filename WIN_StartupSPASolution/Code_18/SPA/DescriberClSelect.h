#pragma once
#include <vector>
#include <string>
#include "ClSelect.h"
#include "define.h"
#include <map>
#include <regex>

using namespace std;

class DescriberClSelect {
private:
	vector<string> _tableSql;
	vector<string> _whereSql;
	vector<string> _columnSql;
	vector<string> _asSql;

public:
	DescriberClSelect(ClSelect clSelect, map<string,string> synEntMap);
	vector<string> GetTableSql();
	vector<string> GetWhereSql();
	vector<string> GetColumnSql();
	vector<string> GetAsSql();
};