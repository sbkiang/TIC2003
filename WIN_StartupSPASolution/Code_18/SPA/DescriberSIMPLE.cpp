#include "DescriberSIMPLE.h"

DescriberSIMPLE::DescriberSIMPLE()
{
}

DescriberSIMPLE::DescriberSIMPLE(string type)
{
	_type = type;
}

DescriberSIMPLE::DescriberSIMPLE(string type, vector<string> data)
{
	_type = type;
	_data = data;
}

void DescriberSIMPLE::AddData(string addData)
{
	_data.push_back(addData);
}

void DescriberSIMPLE::SetData(vector<string> data)
{
	_data = data;
}

vector<string> DescriberSIMPLE::GetData()
{
	return _data;
}

string DescriberSIMPLE::GetType()
{
	return _type;
}
