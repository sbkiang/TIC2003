#include "BuilderFullSqlSelect.h"

string BuilderFullSqlSelect::GetSql(DescriberClSelect describer)
{
	char sqlBuf[1024] = {};
	string selectFromTable;
	string selectColumnName;
	string whereColumnFilter;
	string whereDuplicateFilter;
	string SqlAND = " AND ";
	vector<string> whereSql = describer.GetWhereSql();
	vector<string> tableSql = describer.GetTableSql();
	vector<string> asSql = describer.GetAsSql();
	vector<string> colSql = describer.GetColumnSql();
	for (int i = 0; i < tableSql.size(); i++) {
		selectFromTable += (tableSql.at(i) + ",");
		sprintf_s(sqlBuf, "%s %s,", colSql.at(i).c_str(), asSql.at(i).c_str());
		selectColumnName += sqlBuf;
	}
	for (int i = 0; i < describer.GetWhereSql().size(); i++) {
		whereColumnFilter += (whereSql.at(i) + SqlAND);
	}
	/*
	for (int i = 0; i < st.columnSql.size(); i++) {
		for (int j = i + 1; j < st.columnSql.size(); j++) {
			if (st.columnSql.at(i)[0] != st.columnSql.at(j)[0]) { continue; }
			sprintf_s(sqlBuf, "%s <> %s%s", st.columnSql.at(i).c_str(), st.columnSql.at(j).c_str(), SqlAND.c_str());
			whereDuplicateFilter += sqlBuf;
		}
	}
	*/

	selectFromTable.pop_back(); // remove the last ","
	selectColumnName.pop_back(); // remove the last ","
	whereColumnFilter = whereColumnFilter.substr(0, whereColumnFilter.size() - SqlAND.size()); // remove the last " AND "
	whereDuplicateFilter = whereDuplicateFilter.substr(0, whereDuplicateFilter.size() - SqlAND.size()); // remove the last " AND "

	/*
	if(whereDuplicateFilter == ""){ 
		sprintf_s(sqlBuf, "SELECT %s FROM %s WHERE %s", selectColumnName.c_str(), selectFromTable.c_str(), whereColumnFilter.c_str()); 
	}
	else{ 
		sprintf_s(sqlBuf, "SELECT %s FROM %s WHERE %s AND %s", selectColumnName.c_str(), selectFromTable.c_str(), whereColumnFilter.c_str(), whereDuplicateFilter.c_str()); 
	}
	*/
	if (whereSql.empty()) { 
		sprintf_s(sqlBuf, "SELECT %s FROM %s", selectColumnName.c_str(), selectFromTable.c_str()); 
	}
	else { 
		sprintf_s(sqlBuf, "SELECT %s FROM %s WHERE %s", selectColumnName.c_str(), selectFromTable.c_str(), whereColumnFilter.c_str()); 
	}
	return string(sqlBuf);
}
