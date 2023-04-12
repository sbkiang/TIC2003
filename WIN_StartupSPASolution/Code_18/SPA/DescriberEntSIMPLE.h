#pragma once
#include <string>
#include <vector>
#include "Statement.h"

using namespace std;

class DescriberEntSIMPLE {
private:
	string _type;
	vector<string> _data;

public:
	DescriberEntSIMPLE(Statement stmt);
	void AddData(string addData);
	void SetData(vector<string> data);
	vector<string> GetData();
	string GetType();

};