#pragma once
#include <string>
#include <vector>
#include <map>
#include <regex>
#include "define.h"
#include "Statement.h"
#include "HelperFunction.h"

using namespace std;

class DescriberEntStmt {
private:
	map<string, vector<string>> _entDataMap;
	void ExtractConstant(vector<string> token);
	void ExtractVariable(vector<string> token);
	void ExtractPattern(string text);

public:
	DescriberEntStmt(Statement stmt);
	vector<string> GetEntData(string ent);
};