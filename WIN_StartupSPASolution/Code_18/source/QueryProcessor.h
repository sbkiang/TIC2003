#pragma once
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <algorithm>
#include <stack>
#include <map>
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
#include "../SPA/RelEnt.h"
#include "../SPA/Pattern.h"
#include "../SPA/ColumnBuilderSqlCall.h"
#include "../SPA/ColumnBuilderSqlModifies.h"
#include "../SPA/ColumnBuilderSqlUses.h"
#include "../SPA/ColumnBuilderSqlNext.h"
#include "../SPA/ColumnBuilderSqlParent.h"
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