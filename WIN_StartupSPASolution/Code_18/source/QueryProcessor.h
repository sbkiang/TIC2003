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

#include "../SPA/ClRelRef.h"
#include "../SPA/ClPattern.h"
#include "../SPA/ClSelect.h"

#include "../SPA/DescriberClPattern.h"
#include "../SPA/DescriberClRelref.h"
#include "../SPA/DescriberClSelect.h"

#include "../SPA/BuilderSqlColumnCalls.h"
#include "../SPA/BuilderSqlColumnModifies.h"
#include "../SPA/BuilderSqlColumnUses.h"
#include "../SPA/BuilderSqlColumnNext.h"
#include "../SPA/BuilderSqlColumnParent.h"
#include "../SPA/BuilderSqlColumnPattern.h"

#include "../SPA/BuilderSqlSelectUses.h"
#include "../SPA/BuilderSqlSelectModifies.h"
#include "../SPA/BuilderSqlSelectParent.h"
#include "../SPA/BuilderSqlSelectParentT.h"
#include "../SPA/BuilderSqlSelectCalls.h"
#include "../SPA/BuilderSqlSelectCallsT.h"
#include "../SPA/BuilderSqlSelectNext.h"
#include "../SPA/BuilderSqlSelectNextT.h"
#include "../SPA/BuilderSqlSelectPattern.h"

#include "../SPA/BuilderFullSqlSelect.h"
#include "../SPA/BuilderFullSqlRelRefAndPattern.h"

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