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
#include "../SPA/ColumnBuilderSqlCalls.h"
#include "../SPA/ColumnBuilderSqlModifies.h"
#include "../SPA/ColumnBuilderSqlUses.h"
#include "../SPA/ColumnBuilderSqlNext.h"
#include "../SPA/ColumnBuilderSqlParent.h"
#include "../SPA/ColumnBuilderSqlPattern.h"

#include "../SPA/QueryBuilderSqlUses.h"
#include "../SPA/QueryBuilderSqlModifies.h"
#include "../SPA/QueryBuilderSqlParent.h"
#include "../SPA/QueryBuilderSqlParentT.h"
#include "../SPA/QueryBuilderSqlCalls.h"
#include "../SPA/QueryBuilderSqlCallsT.h"
#include "../SPA/QueryBuilderSqlNext.h"
#include "../SPA/QueryBuilderSqlNextT.h"
#include "../SPA/QueryBuilderSqlPattern.h"

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