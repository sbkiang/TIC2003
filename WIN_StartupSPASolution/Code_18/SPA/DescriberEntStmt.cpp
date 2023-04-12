#include "DescriberEntStmt.h"

DescriberEntStmt::DescriberEntStmt(Statement stmt)
{
	_type = stmt.GetEntity();
	if (stmt.GetEntity() == "while") {
		_typeDataMap[_type].push_back(to_string(stmt.GetStmtNum()));
		_typeDataMap[_type].push_back(stmt.GetEntity());
		_typeDataMap[_type].push_back(stmt.GetStmt());
	}
	for (string token : stmt.GetToken()) {
	}
}

void DescriberEntStmt::AddData(string addData)
{
	_data.push_back(addData);
}

void DescriberEntStmt::SetData(vector<string> data)
{
	_data = data;
}

vector<string> DescriberEntStmt::GetData()
{
	return _data;
}

string DescriberEntStmt::GetType()
{
	return _type;
}
