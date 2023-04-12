#pragma once
#include <string>
#include <vector>
#include <map>
#include "Statement.h"

using namespace std;

class DescriberEntStmt {
private:
	string _type;
	map<string, vector<string>> _typeDataMap;
	vector<string> _data;

public:
	DescriberEntStmt(Statement stmt);
	void AddData(string addData);
	void SetData(vector<string> data);
	vector<string> GetData();
	string GetType();

};