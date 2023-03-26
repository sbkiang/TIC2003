#pragma once

#include <string>
#include <vector>
#include "Database.h"
#include "Tokenizer.h"
#include <regex>
#include "../SPA/Struct.h"

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
	set<string> SqlResultStoreToSet(SqlResultStore rs, string col);

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

	int prec(char c);
	string infixToPostfix(string s);

};