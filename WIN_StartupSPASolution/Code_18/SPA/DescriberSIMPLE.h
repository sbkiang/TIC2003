#pragma once
#include <string>
#include <vector>

using namespace std;

class DescriberSIMPLE {
private:
	string _type;
	vector<string> _data;

public:
	DescriberSIMPLE();
	DescriberSIMPLE(string type);
	DescriberSIMPLE(string type, vector<string> data);
	void AddData(string addData);
	void SetData(vector<string> data);
	vector<string> GetData();
	string GetType();

};