#include "DescriberEntSIMPLE.h"

DescriberEntSIMPLE::DescriberEntSIMPLE(Statement stmt)
{
	_type = stmt.GetEntity();
	if (stmt.GetEntity() == "while") {
		//_type
	}
}

void DescriberEntSIMPLE::AddData(string addData)
{
	_data.push_back(addData);
}

void DescriberEntSIMPLE::SetData(vector<string> data)
{
	_data = data;
}

vector<string> DescriberEntSIMPLE::GetData()
{
	return _data;
}

string DescriberEntSIMPLE::GetType()
{
	return _type;
}
