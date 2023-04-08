#pragma once
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <algorithm>
#include <stack>
#include <set>
#include <ranges>

#include "../SPA/Struct.h"
#include "../SPA/define.h"
#include "../SPA/ClauseModifies.h"
#include "../SPA/ClauseUses.h"
#include "../SPA/ClauseParent.h"
#include "../SPA/ClauseCall.h"
#include "../SPA/ClausePattern.h"
#include "../SPA/HelperFunction.h"
#include "../SPA/ClauseNext.h"
#include "../SPA/ClRelRef.h"
#include "../SPA/ClPattern.h"
#include "../SPA/DescriberClPattern.h"
#include "../SPA/DescriberClRelref.h"
#include "../SPA/BuilderColumnSqlCalls.h"
#include "../SPA/BuilderColumnSqlModifies.h"
#include "../SPA/BuilderColumnSqlUses.h"
#include "../SPA/BuilderColumnSqlNext.h"
#include "../SPA/BuilderColumnSqlParent.h"
#include "../SPA/BuilderColumnSqlPattern.h"

#include "../SPA/BuilderQuerySqlUses.h"
#include "../SPA/BuilderQuerySqlModifies.h"
#include "../SPA/BuilderQuerySqlParent.h"
#include "../SPA/BuilderQuerySqlParentT.h"
#include "../SPA/BuilderQuerySqlCalls.h"
#include "../SPA/BuilderQuerySqlCallsT.h"
#include "../SPA/BuilderQuerySqlNext.h"
#include "../SPA/BuilderQuerySqlNextT.h"
#include "../SPA/BuilderQuerySqlPattern.h"

#include "Tokenizer.h"
#include "Database.h"

using namespace std;

class QueryProcessor {
public:
	// default constructor
	QueryProcessor();

	// destructor
	~QueryProcessor();

	// method for evaluating a query
	void evaluate(string query, vector<string>& results);
	void EvaluateSelect(Select& st, map<string,string> synEntMap);

	//Check for number or string
	bool isNumber(string text) {
		bool isNum = true;
		for (char c : text) {
			if (!isdigit(c)) {
				isNum = false;
				break;
			}
		}
		return isNum;
	}
};