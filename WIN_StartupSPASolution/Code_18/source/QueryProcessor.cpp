#include "QueryProcessor.h"
#include <iostream>
#include <algorithm>
#include <stack>
#include <map>

// constructor
QueryProcessor::QueryProcessor() {}

// destructor
QueryProcessor::~QueryProcessor() {}

void QueryProcessor::EvaluateSelect(Select& st, map<string,string> synEntMap) {
	string regexStmtNumEntity = "stmt|read|print|assign|while|if|call", regexNameEntity = "variable|procedure", columnName = "", stmtTable = "statement";
	map<string, int> tableCountMap;
	int counter = 0;
	char sqlBuf[1024] = {};
	//for (auto it = st.synonymEntityMap.begin(); it != st.synonymEntityMap.end(); it++) {
	for (auto it = st.synonym.begin(); it != st.synonym.end(); it++) {
		string synonym = (*it);
		string entity = synEntMap.at(synonym);
		string tableAlias = "";
		if (regex_match(entity, regex(regexStmtNumEntity))) { // assign stmt can be obtained from statement table
			if (tableCountMap.find(stmtTable) == tableCountMap.end()) { tableCountMap.insert(pair<string, int>(stmtTable, 0)); }
			else { tableCountMap.at(stmtTable)++; }
			columnName = "line_num";
			sprintf_s(sqlBuf, "s%s", to_string(tableCountMap.at(stmtTable)).c_str());
			tableAlias = sqlBuf;
			sprintf_s(sqlBuf, "%s %s", stmtTable.c_str(), tableAlias.c_str()); // E.g., statement s0, statement s1
		}
		else if (regex_match(entity, regex(regexNameEntity))) { // entity belongs to the group that returns name
			if (tableCountMap.find(entity) == tableCountMap.end()) { tableCountMap.insert(pair<string, int>(entity, 0)); }
			else { tableCountMap.at(entity)++; }
			columnName = "name";
			sprintf_s(sqlBuf, "%c%s", entity[0], to_string(tableCountMap.at(entity)).c_str());
			tableAlias = sqlBuf;
			sprintf_s(sqlBuf, "%s %s", entity.c_str(), tableAlias.c_str()); // E.g., statement s0, statement s1
		}
		st.tableSql.push_back(sqlBuf);
		sprintf_s(sqlBuf, "%s.%s", tableAlias.c_str(), columnName.c_str()); // E.g., s0.line_num as a, s1.line_num as b, p0.name as p
		st.columnSql.push_back(sqlBuf);
		sprintf_s(sqlBuf, "AS %s", synonym.c_str());
		st.asSql.push_back(sqlBuf);
		if (entity != "stmt" && !regex_match(entity, regex(regexNameEntity))) { // not stmt and not procedure and not variable
			sprintf_s(sqlBuf, "%s.entity='%s'", tableAlias.c_str(), entity.c_str()); // E.g., s0.type='while', s1.type='if'
			st.whereSql.push_back(sqlBuf);
		}
	}
}

// method to evaluate a query
// This method currently only handles queries for getting all the procedure names,
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all required queries.
void QueryProcessor::evaluate(string query, vector<string>& output) {
	// clear the output vector
	output.clear();

	// tokenize the query
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(query, tokens);

	// check what type of synonym is being declared
	vector<string> synonymType, synonymVar;
	// check for such that or pattern is being declared
	vector<int> suchThatIdx, patternIdx;

	// create a vector for storing the results from database
	vector<string> databaseResults;

	string designAbstract = "", stmtNum1 = "", stmtNum2 = "";
	int idx = 0, selectIdx = 0;
	bool comma = false;
	string regexWord = "\\w+";
	string regexQuote = "\\\"";
	string regexRelation = "(Parent|Next|Calls|Modifies|Uses)";
	string regexEntity = "(variable|constant|call|assign|stmt|read|print|while|if)";
	map<string, string> synonymEntityMap;
	//vector<map<string, string>> synonymEntityMapVector;
	stack<SuchThat> suchThatStack;
	stack<Pattern> patternStack;
	Select select;
	for (int i = 0; i < tokens.size(); i++) {
		//if (tokens[i] == "procedure" || tokens[i] == "variable" || tokens[i] == "constant" || tokens[i] == "call" || tokens[i] == "assign" || tokens[i] == "stmt" || tokens[i] == "read" || tokens[i] == "print" || tokens[i] == "while" || tokens[i] == "if") {
		if (regex_match(tokens.at(i), regex(regexEntity))){
			synonymType.push_back(tokens[i]);
			synonymVar.push_back(tokens[i + 1]);
			string entityType = tokens.at(i);
			i++; // skip entity type
			while (tokens.at(i) != ";") {
				if (tokens.at(i) != ",") {  // skip the ","
					synonymEntityMap.insert(pair<string, string>(tokens.at(i), entityType));
				}
				i++;
			}
			
		}
		else if (tokens[i] == "such" && tokens[i + 1] == "that") { // such that relationship(x,y)
			suchThatIdx.push_back(i);
			i += 2;
			SuchThat st;
			st.relationship = tokens.at(i);
			i++;
			if (tokens.at(i) == "*") {
				st.relationship += tokens.at(i);
				i++;
			}

			int brackets = 0;
			string word = "";
			do {
				if (tokens.at(i) == "(") { // starting bracket
					brackets++;
					i++;
					continue;
				}
				else if (tokens.at(i) == ")") { // closing bracket
					brackets--;
				}
				if (brackets >= 1) {
					word += tokens.at(i);
				}
				i++;
			} while (brackets > 0);
			int comma = word.find(",");
			st.first = word.substr(0, comma);
			st.second = word.substr(comma + 1, word.length());
			suchThatStack.push(st);
		}
		else if (tokens[i] == "pattern") {
			patternIdx.push_back(i);
			Pattern pt;
			i++;
			pt.synonym = tokens.at(i);
			i++;
			int brackets = 0;
			string word = "";
			do {
				if (tokens.at(i) == "(") { // starting bracket
					brackets++;
					i++;
					continue;
				}
				else if (tokens.at(i) == ")") { // closing bracket
					brackets--;
				}
				if (brackets >= 1) {
					word += tokens.at(i);
				}
				i++;
			} while (brackets > 0);
			int comma = word.find(",");
			pt.first = word.substr(0, comma);
			pt.second = word.substr(comma + 1, word.length());
			patternStack.push(pt);
		}
		else if (tokens[i] == "Select") {
			selectIdx = i;
			i++;
			if (tokens.at(i) == "<") {
				while (tokens.at(i) != ">") {
					if (regex_match(tokens.at(i), regex(regexWord))) {
						select.synonym.push_back(tokens.at(i));
						//select.synonymEntityMap.insert(pair<string, string>(tokens.at(i),synonymEntityMap.at(tokens.at(i))));
					}
					i++;
				}
			}
			else {
				select.synonym.push_back(tokens.at(i));
				//select.synonymEntityMap.insert(pair<string, string>(tokens.at(i), synonymEntityMap.at(tokens.at(i))));
			}
		}
	}
	SqlResultStore sqlResultStore;
	EvaluateSelect(select, synonymEntityMap);
	vector<SqlResult> sqlResultPass;
	Database::SelectPql(select, sqlResultStore);
	

	// need to determine if the input to such that is generic or specific
	//	generic when it's a synonym and not part of select
	//  specific when it's part of select or it's not a synonym
	while (!suchThatStack.empty()) {
		SuchThat suchThatTemp = suchThatStack.top();
		bool synonymFirst = (synonymEntityMap.find(suchThatTemp.first) != synonymEntityMap.end()); // first input is a synonym
		bool synonymSecond = (synonymEntityMap.find(suchThatTemp.second) != synonymEntityMap.end()); // second input is a synonym
		bool synonymFirstSelect = (find(select.synonym.begin(), select.synonym.end(), suchThatTemp.first) != select.synonym.end()); // first input is part of select
		bool synonymSecondSelect = (find(select.synonym.begin(), select.synonym.end(), suchThatTemp.second) != select.synonym.end()); // second input is part of select
		string entity = "", first = suchThatTemp.first, second = suchThatTemp.second;
		if (synonymFirstSelect) { entity = synonymEntityMap.at(suchThatTemp.first); }
		if (synonymSecondSelect) { entity = synonymEntityMap.at(suchThatTemp.second); }
		for (int i = 0; i < sqlResultStore.sqlResult.size(); i++) {
			SqlResult sqlResulTemp = sqlResultStore.sqlResult.at(i);
			if (synonymFirstSelect) { // if the first input is a synonym, and is part of select, get the input 
				first = sqlResulTemp.row.at(suchThatTemp.first);
			}
			if (synonymSecondSelect) { // if the second input is a synonym, and is part of select, get the input 
				second = sqlResulTemp.row.at(suchThatTemp.second);
			}
			if (suchThatTemp.relationship == "Uses") {
				bool pass = false;
				if (entity == "assign") { pass = Database::GetUsesForAssign(first, second, synonymFirstSelect, synonymSecondSelect); }
				else if (entity == "print") { pass = Database::GetUsesForPrint(first, second, synonymFirstSelect, synonymSecondSelect); }
				else if (entity == "while") { pass = Database::GetUsesForWhile(first, second, synonymFirstSelect, synonymSecondSelect); }
				else if (entity == "call") { pass = Database::GetUsesForCall(first, second, synonymFirstSelect, synonymSecondSelect); }
				else if (entity == "procedure") { pass = Database::GetUsesForProcedure(first, second, synonymFirstSelect, synonymSecondSelect); }
				if (pass) { sqlResultPass.push_back(sqlResulTemp); }
			}
		}
		sqlResultStore.sqlResult = sqlResultPass;
		suchThatStack.pop();
	}
	
	// post process the results to fill in the output vector
	for (SqlResult sqlResult : sqlResultStore.sqlResult) {
		string ouptut;
		for (int i = 0; i < sqlResultStore.column.size(); i++) {
			string col = sqlResult.column.at(i);
			auto test = sqlResult.row.at(col);
		}
		output.push_back(ouptut);
	}

}

