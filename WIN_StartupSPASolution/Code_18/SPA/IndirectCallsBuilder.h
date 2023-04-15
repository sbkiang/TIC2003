#pragma once
#include <vector>
#include <string>
#include "Statement.h"

using namespace std;

class IndirectCallsBuilder {
private:
	void _BuildIndirectCalls(vector<Statement*> callStmt, vector<pair<string, string>>& callerCallee);

public:
	vector<pair<string, string>> BuildIndirectCalls(vector<Statement*> callStmt);
};