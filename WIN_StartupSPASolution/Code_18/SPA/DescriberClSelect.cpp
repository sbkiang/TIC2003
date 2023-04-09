#include "DescriberClSelect.h"

DescriberClSelect::DescriberClSelect(ClSelect clSelect, map<string,string> synEntMap)
{
	string columnName = "";
	string stmtTable = "statement";
	map<string, int> tableCountMap;
	int counter = 0;
	char sqlBuf[1024] = {};
	for(string synonym : clSelect.GetSynonym()){
		string entity = synEntMap.at(synonym);
		string tableAlias = "";
		if (regex_match(entity, regex(regexStmtNumEntity))) { // for entity that return statement number, we can get them from statement table by filtering based on entity type
			if (tableCountMap.find(stmtTable) == tableCountMap.end()) { 
				tableCountMap.insert(pair<string, int>(stmtTable, 0)); 
			}
			else { 
				tableCountMap.at(stmtTable)++; 
			}
			columnName = "line_num";
			sprintf_s(sqlBuf, "s%s", to_string(tableCountMap.at(stmtTable)).c_str());
			tableAlias = sqlBuf;
			sprintf_s(sqlBuf, "%s %s", stmtTable.c_str(), tableAlias.c_str()); // E.g., statement s0, statement s1
		}
		else if (regex_match(entity, regex(regexNameEntity))) { // for entity that returns name, we need to get it from their respective table
			if (tableCountMap.find(entity) == tableCountMap.end()) { 
				tableCountMap.insert(pair<string, int>(entity, 0)); 
			}
			else { 
				tableCountMap.at(entity)++; 
			}
			if (entity == "constant") { 
				columnName = "value"; 
			}
			else { 
				columnName = "name"; 
			}
			sprintf_s(sqlBuf, "%c%s", entity[0], to_string(tableCountMap.at(entity)).c_str());
			tableAlias = sqlBuf;
			sprintf_s(sqlBuf, "%s %s", entity.c_str(), tableAlias.c_str()); // E.g., procedure p0, variable v0, constant c0
		}
		else if (entity == "constant") {
			if (tableCountMap.find(entity) == tableCountMap.end()) { 
				tableCountMap.insert(pair<string, int>(entity, 0)); 
			}
			else { 
				tableCountMap.at(entity)++; 
			}
			columnName = "value";
			sprintf_s(sqlBuf, "c%s", to_string(tableCountMap.at(entity)).c_str());
			tableAlias = sqlBuf;
			sprintf_s(sqlBuf, "%s %s", entity.c_str(), tableAlias.c_str()); // E.g., constant c0, constant c1
		}
		_tableSql.push_back(sqlBuf);
		sprintf_s(sqlBuf, "%s.%s", tableAlias.c_str(), columnName.c_str()); // E.g., s0.line_num as a, s1.line_num as b, p0.name as p
		_columnSql.push_back(sqlBuf);
		sprintf_s(sqlBuf, "AS %s", synonym.c_str());
		_asSql.push_back(sqlBuf);
		if (regex_match(entity, regex(regexStmtNumEntityNoStmt))) { // if we are getting all the statement number entity, we need the "WHERE" SQL to filter based on entity
			sprintf_s(sqlBuf, "%s.entity='%s'", tableAlias.c_str(), entity.c_str()); // E.g., s0.type='while', s1.type='if'
			_whereSql.push_back(sqlBuf);
		}
	}
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
