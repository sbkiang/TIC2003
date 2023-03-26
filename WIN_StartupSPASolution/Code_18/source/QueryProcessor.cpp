#include "QueryProcessor.h"
#include <iostream>
#include <algorithm>
#include <stack>
#include <map>
#include <set>

// constructor
QueryProcessor::QueryProcessor() {}

// destructor
QueryProcessor::~QueryProcessor() {}

set<string> QueryProcessor::SqlResultStoreToSet(SqlResultStore rs, string col) {
	set<string> setRes;
	for (int i = 0; i < rs.sqlResult.size(); i++) {
		SqlResult sqlRes = rs.sqlResult.at(i);
		setRes.insert(sqlRes.row.at(col));
	}
	return setRes;
}

void QueryProcessor::EvaluateSelect(Select& select, map<string,string> synEntMap) {
	string regexStmtNumEntity = "(stmt|read|print|assign|while|if|call)", regexNameEntity = "(variable|procedure|constant)", columnName = "", stmtTable = "statement";
	map<string, int> tableCountMap;
	int counter = 0;
	char sqlBuf[1024] = {};
	for (auto it = select.synonym.begin(); it != select.synonym.end(); it++) {
		string synonym = (*it);
		string entity = synEntMap.at(synonym);
		string tableAlias = "";
		if (regex_match(entity, regex(regexStmtNumEntity))) { // for entity that return statement number, we can get them from statement table by filtering based on entity type
			if (tableCountMap.find(stmtTable) == tableCountMap.end()) { tableCountMap.insert(pair<string, int>(stmtTable, 0)); }
			else { tableCountMap.at(stmtTable)++; }
			columnName = "line_num";
			sprintf_s(sqlBuf, "s%s", to_string(tableCountMap.at(stmtTable)).c_str());
			tableAlias = sqlBuf;
			sprintf_s(sqlBuf, "%s %s", stmtTable.c_str(), tableAlias.c_str()); // E.g., statement s0, statement s1
		}
		else if (regex_match(entity, regex(regexNameEntity))) { // for entity that returns name, we need to get it from their respective table
			if (tableCountMap.find(entity) == tableCountMap.end()) { tableCountMap.insert(pair<string, int>(entity, 0)); }
			else { tableCountMap.at(entity)++; }
			if (entity == "constant") { columnName = "value"; }
			else { columnName = "name"; }
			sprintf_s(sqlBuf, "%c%s", entity[0], to_string(tableCountMap.at(entity)).c_str());
			tableAlias = sqlBuf;
			sprintf_s(sqlBuf, "%s %s", entity.c_str(), tableAlias.c_str()); // E.g., procedure p0, variable v0, constant c0
		}
		/*else if (entity == "constant") {
			if (tableCountMap.find(entity) == tableCountMap.end()) { tableCountMap.insert(pair<string, int>(entity, 0)); }
			else { tableCountMap.at(entity)++; }
			columnName = "value";
			sprintf_s(sqlBuf, "c%s", to_string(tableCountMap.at(entity)).c_str());
			tableAlias = sqlBuf;
			sprintf_s(sqlBuf, "%s %s", entity.c_str(), tableAlias.c_str()); // E.g., constant c0, constant c1
		}*/
		select.tableSql.push_back(sqlBuf);
		sprintf_s(sqlBuf, "%s.%s", tableAlias.c_str(), columnName.c_str()); // E.g., s0.line_num as a, s1.line_num as b, p0.name as p
		select.columnSql.push_back(sqlBuf);
		sprintf_s(sqlBuf, "AS %s", synonym.c_str());
		select.asSql.push_back(sqlBuf);
		if (entity != "stmt" && !regex_match(entity, regex(regexNameEntity))) { // if we are getting all the statement number entity, we need the "WHERE" SQL to filter based on entity
			sprintf_s(sqlBuf, "%s.entity='%s'", tableAlias.c_str(), entity.c_str()); // E.g., s0.type='while', s1.type='if'
			select.whereSql.push_back(sqlBuf);
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

	string regexWord = "\\w+";
	string regexQuote = "\\\"";
	string regexRelation = "(Parent|Next|Calls|Modifies|Uses)";
	string regexEntity = "(procedure|variable|constant|call|assign|stmt|read|print|while|if)";
	map<string, string> synonymEntityMap; // map the synonym to entity
	map<string, set<string>> variableSynonymSetMap; // map the variable synonym to set of SQL data
	stack<SuchThat> suchThatStack;
	stack<Pattern> patternStack;
	Select select;	
	for (int i = 0; i < tokens.size(); i++) {
		if (regex_match(tokens.at(i), regex(regexEntity))){
			string entityType = tokens.at(i);
			i++;
			while (tokens.at(i) != ";") {
				if (tokens.at(i) != ",") {  // skip the ","
					synonymEntityMap.insert(pair<string, string>(tokens.at(i), entityType));
					if (entityType == "variable") {
						set<string> varSet;
						Database::GetVariable(varSet);
						variableSynonymSetMap.insert(pair<string,set<string>>(tokens.at(i), varSet));
					}
				}
				i++;
			}
			
		}
		else if (tokens[i] == "such" && tokens[i + 1] == "that") { // such that relationship(x,y)
			i += 2;
			SuchThat st;
			st.relationship = tokens.at(i);
			if (tokens.at(i+1) == "*") {
				i++;
				st.relationship += tokens.at(i);
			}
			int brackets = 0;
			string word = "";
			do {
				i++;
				if (tokens.at(i) == "(") { // starting bracket
					brackets++;
				}
				else if (tokens.at(i) == ")") { // closing bracket
					brackets--;
					
				}
				else if (!regex_match(tokens.at(i), regex(regexQuote))) {
					word += tokens.at(i);
				}
			} while (brackets > 0);
			int comma = word.find(",");
			st.input1 = word.substr(0, comma);
			st.input2 = word.substr(comma + 1, word.length());
			suchThatStack.push(st);
		}
		else if (tokens[i] == "pattern") {
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
					continue;
				}

				if (!regex_match(tokens.at(i), regex(regexQuote))) {
					word += tokens.at(i);
				}

				i++;
			} while (brackets > 0);

			int comma = word.find(",");
			pt.input1 = word.substr(0, comma);

			string input2 = word.substr(comma + 1, word.length());
			//if (input2.size() > 1) { input2.erase(remove(input2.begin(), input2.end(), '_'), input2.end()); }
			pt.input2 = input2;
			patternStack.push(pt);
		}
		else if (tokens[i] == "Select") {
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
			}
		}
	}
	SqlResultStore sqlResultStore;
	EvaluateSelect(select, synonymEntityMap);
	//vector<SqlResult> sqlResultPass;
	Database::SelectPql(select, sqlResultStore);

	// stores mapping of synonym to declared variable synoynms
	// one variable can be mapped by multiple synonym. E.g., variable v. Uses(3,v) and parent(6,v). 
	// correct: store variable synonym letter to variable set. variable set populated in uses, modifies
	map<string, set<string>> synonymVariableMap;
	for (auto it = synonymEntityMap.begin(); it != synonymEntityMap.end(); it++) {
		if (it->second == "variable") {
			set<string> varset;
			synonymVariableMap.insert(pair<string, set<string>>(it->first, varset));
		}
	}

	// need to determine if the input to such that is generic or specific
	//	generic when it's (a synonym and not part of select) or (wildcard)
	//  specific when it's (a synonym and part of select) or ((not a synonym) and (not wildcard))
	vector<SqlResult> sqlResultPass;
	while (!suchThatStack.empty()) {
		SuchThat suchThatTemp = suchThatStack.top();
		bool input1IsWildcard = (suchThatTemp.input1 == "_") ? true : false;
		bool input2IsWildcard = (suchThatTemp.input2 == "_") ? true : false;
		bool input1IsSynonym = (synonymEntityMap.find(suchThatTemp.input1) != synonymEntityMap.end()); // first input is a synonym
		bool input2IsSynonym = (synonymEntityMap.find(suchThatTemp.input2) != synonymEntityMap.end()); // second input is a synonym
		bool input1InSelect = (find(select.synonym.begin(), select.synonym.end(), suchThatTemp.input1) != select.synonym.end()) && !input1IsWildcard; // first input is part of select
		bool input2InSelect = (find(select.synonym.begin(), select.synonym.end(), suchThatTemp.input2) != select.synonym.end()) && !input2IsWildcard; // second input is part of select
		bool input1IsSpecific = ((input1IsSynonym && input1InSelect) || (!input1IsSynonym && !input1IsWildcard));
		bool input2IsSpecific = ((input2IsSynonym && input2InSelect) || (!input2IsSynonym && !input2IsWildcard));
		
		string entityInput1 = "", entityInput2 = "", first = suchThatTemp.input1, second = suchThatTemp.input2;
		if (input1IsSynonym) { // if input1 is part of declared synonym, we get the entity that it belongs
			entityInput1 = synonymEntityMap.at(suchThatTemp.input1);

		}
		if (input2IsSynonym) { // if input1 is part of declared synonym, we get the entity that it belongs
			entityInput2 = synonymEntityMap.at(suchThatTemp.input2);
		}
		set<string> assignVariableName;
		SqlResultStore resultStoreVariableName;
		string relationship = suchThatTemp.relationship;

		// This step is to get the variables if we have a Uses or Modifies relationship
		if (relationship == "Uses") {			
			if (entityInput1 == "assign") {
				Database::GetUsesForAssignVar(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
			else if (entityInput1 == "print") {
				Database::GetUsesForPrintVar(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
			else if (entityInput1 == "while") {
				Database::GetUsesForWhileVar(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
			else if (entityInput1 == "if") {
				Database::GetUsesForIfVar(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
			else if (entityInput1 == "call") {
				Database::GetUsesForCallVar(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
			else if (entityInput1 == "procedure") {
				Database::GetUsesForProcedureVar(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
			else {
				Database::GetUsesForUnknownInput1Var(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
		}
		else if (relationship == "Modifies") {
			if (entityInput1 == "assign") {
				Database::GetModifiesForAssignVar(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
			else if (entityInput1 == "print") {
				Database::GetModifiesForPrintVar(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
			else if (entityInput1 == "while") {
				Database::GetModifiesForWhileVar(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
			else if (entityInput1 == "if") {
				Database::GetModifiesForIfVar(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
			else if (entityInput1 == "call") {
				Database::GetModifiesForCallVar(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
			else if (entityInput1 == "procedure") {
				//Database::GetModifiesForProcedureVar(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
			else {
				Database::GetModifiesForUnknownInput1Var(suchThatTemp.input1, suchThatTemp.input2, input1IsSynonym, input2IsSynonym, resultStoreVariableName);
				assignVariableName = SqlResultStoreToSet(resultStoreVariableName, "variable_name");
			}
		}
		for(SqlResult sqlResult : sqlResultStore.sqlResult){
		//for (int i = 0; i < sqlResultStore.sqlResult.size(); i++) {
			//SqlResult sqlResulTemp = sqlResultStore.sqlResult.at(i);
			bool pass = false;
			if (input1InSelect) { // if the first input is a synonym, and is part of select, get the input 
				first = sqlResult.row.at(suchThatTemp.input1);
			}
			if (input2InSelect) { // if the second input is a synonym, and is part of select, get the input 
				second = sqlResult.row.at(suchThatTemp.input2);
			}
			if (relationship == "Uses") { // input1 is Stmt Num or Name, input2 is Name
				if (entityInput1 == "assign") { 
					pass = Database::GetUsesForAssign(first, second, input1IsSpecific, input2IsSpecific); }
				else if (entityInput1 == "print") { 
					pass = Database::GetUsesForPrint(first, second, input1IsSpecific, input2IsSpecific); }
				else if (entityInput1 == "while") { 
					pass = Database::GetUsesForWhile(first, second, input1IsSpecific, input2IsSpecific); }
				else if (entityInput1 == "if") { 
					pass = Database::GetUsesForIf(first, second, input1IsSpecific, input2IsSpecific); }
				else if (entityInput1 == "call") { 
					pass = Database::GetUsesForCall(first, second, input1IsSpecific, input2IsSpecific); }
				else if (entityInput1 == "procedure") { 
					pass = Database::GetUsesForProcedure(first, second, input1IsSpecific, input2IsSpecific); }
				else { 
					pass = Database::GetUsesForUnknownInput1(first, second, input1IsSpecific, input2IsSpecific); } // e.g., uses(10,v) or uses("main",v). just pass in here even
				// add in code to check if synonym used in query. If not, then just need to test once for first row, and result applies to all row
			}
			else if (relationship == "Modifies") { // input1 is Stmt Num or Name, input2 is Name or Wildcard
				if (entityInput1 == "assign") { 
					pass = Database::GetModifiesForAssign(first, second, input1IsSpecific, input2IsSpecific); }
				else if (entityInput1 == "read") { 
					pass = Database::GetModifiesForRead(first, second, input1IsSpecific, input2IsSpecific); }
				else if (entityInput1 == "while") { 
					pass = Database::GetModifiesForWhile(first, second, input1IsSpecific, input2IsSpecific); }
				else if (entityInput1 == "if") { 
					pass = Database::GetModifiesForIf(first, second, input1IsSpecific, input2IsSpecific); }
				else if (entityInput1 == "call") { 
					pass = Database::GetModifiesForCall(first, second, input1IsSpecific, input2IsSpecific); }
				else if (entityInput1 == "procedure") { 
					pass = Database::GetModifiesForProcedure(first, second, input1IsSpecific, input2IsSpecific); }
				else { 
					pass = Database::GetModifiesForUnknownInput1(first, second, input1IsSpecific, input2IsSpecific); } // e.g., uses(10,v) or uses("main",v). just pass in here even
			}
			else if (relationship == "Parent") { // input1 is Stmt Num, input2 is Stmt Num
				pass = Database::GetParent(first, second, input1IsSpecific, input2IsSpecific, entityInput1, entityInput2);
			}
			else if (relationship == "Parent*") { // input1 is Stmt Num, input2 is Stmt Num
				pass = Database::GetParentT(first, second, input1IsSpecific, input2IsSpecific, entityInput1, entityInput2);
			}
			else if (relationship == "Next") {

			}
			else if (relationship == "Next*"){

			}
			else if (relationship == "Calls") {

			}
			else if (relationship == "Calls*") {

			}
			if (pass) {
				sqlResultPass.push_back(sqlResult);
			}
		}
		sqlResultStore.sqlResult = sqlResultPass;
		suchThatStack.pop();
		if (input2IsSynonym && (relationship == "Modifies" || relationship == "Uses")) {
			set<string> synonymVariable = variableSynonymSetMap.at(suchThatTemp.input2);
			set<string> intersect;
			set_intersection(assignVariableName.begin(), assignVariableName.end(), synonymVariable.begin(), synonymVariable.end(), inserter(intersect, intersect.begin()));
			variableSynonymSetMap.at(suchThatTemp.input2) = intersect;
		}
	}

	while (!patternStack.empty()) {
		Pattern patternTemp = patternStack.top();
		string entity = synonymEntityMap.at(patternTemp.synonym), first = patternTemp.input1, second = patternTemp.input2, line = "";
		bool input2IsWildcard = (patternTemp.input2 == "_") ? true : false;
		bool input1IsSynonym = (synonymEntityMap.find(patternTemp.input1) != synonymEntityMap.end());

		// if input2 isn't wildcard, convert to postfix. input2 is either wildcard or pattern expr
		if (!input2IsWildcard) {
			second = patternTemp.input2;
			bool startWildCard = (second[0] == '_');
			bool endWildCard = (second[second.length() - 1] == '_');
			second = (startWildCard) ? second.substr(1, second.length()) : second;
			second = (endWildCard) ? second.substr(0, second.length() - 1) : second;
			second = infixToPostfix(second);
			second = (startWildCard) ? ("_" + second) : second;
			second = (endWildCard) ? (second + "_") : second;
		}

		// if input1 synonym, get from synoynm. else, it'll be wildcard or specific variable name
		SqlResultStore patternResult;

		// get input from variable storage that's linked to assign synonym
		if (input1IsSynonym) {
			set<string> varNameSet = variableSynonymSetMap.at(patternTemp.input1);
			char varNameSql[128] = {};
			string first = "";
			for (auto it = varNameSet.begin(); it != varNameSet.end(); it++) {
				sprintf_s(varNameSql, "\'%s\',", string(*it).c_str());
				first += string(varNameSql);
			}
			if (first.length() > 0) { first.pop_back(); } // remove last comma 
			Database::GetPatternIn(first, second, patternResult);
		}
		else {
			string first = patternTemp.input1;
			Database::GetPatternLike(first, second, patternResult);
		}

		// patternResult contains line_num and LHS
		// LHS to update the variableSynonymSetMap. line_num is to update the sqlResultSet, if it's a line_num entity
		set<string> patternLhsSet;
		vector<string> patternLineNum;
		for (int i = 0; i < patternResult.sqlResult.size(); i++) {
			patternLhsSet.insert(patternResult.sqlResult.at(i).row.at("lhs"));
			patternLineNum.push_back(patternResult.sqlResult.at(i).row.at("line_num"));
		}

		// this is to update the variables that passed the pattern check in variableSynonymSetMap
		if (input1IsSynonym) { // what is input1 is fixed? means no update
			set<string> intersect;
			set<string> variableSynonymSet = variableSynonymSetMap.at(patternTemp.input1);
			set_intersection(variableSynonymSet.begin(), variableSynonymSet.end(), patternLhsSet.begin(), patternLhsSet.end(), inserter(intersect, intersect.begin()));
			variableSynonymSetMap.at(patternTemp.input1) = intersect;
		}

		// ** For assign entity only **
		// this step is to update the sqlResultStore if the pattern entity is part of select
		// we do this because the pattern clause returns result instead of true/false
		// so, we need to find the common variables or line_num between the sqlResultStore and pattern clause return value
		// quite messy way to do this

		vector<SqlResult> sqlResultPass;
		for (string lineNum : patternLineNum) {
			for (SqlResult sqlResult : sqlResultStore.sqlResult) {
				for (auto it = sqlResult.row.begin(); it != sqlResult.row.end(); it++) {
					string entity = synonymEntityMap.at(it->first);
					if (entity == "assign" && lineNum == it->second) {
						sqlResultPass.push_back(sqlResult);
					}
				}
			}
		}
		sqlResultStore.sqlResult = sqlResultPass;

		vector<SqlResult> sqlResultPass;
		bool selectSynonymIsAssignOrStmt = false;
		bool selectSynonymIsVariable = false;
		for (string synonym : select.synonym) {
			if (synonymEntityMap.at(synonym) == "assign") { selectSynonymIsAssignOrStmt = true; break; }
			else if (synonymEntityMap.at(synonym) == "stmt") { selectSynonymIsAssignOrStmt = true; break; }
			else if (synonymEntityMap.at(synonym) == "variable") { selectSynonymIsVariable = true; break; }
		}
		if (selectSynonymIsAssignOrStmt) {
			for (SqlResult sqlResult : sqlResultStore.sqlResult) { // loop thru each row
				for (auto it = sqlResult.row.begin(); it != sqlResult.row.end(); it++) { // loop thru each row's column
					if (synonymEntityMap.at(it->first) == "assign" || synonymEntityMap.at(it->first) == "stmt") {

					}
				}
			}
		}
		bool patternInput1InSelect = (find(select.synonym.begin(), select.synonym.end(), patternTemp.input1) != select.synonym.end());
		//string linkingEntitySynonym = patternSynonymInSelect ? synonymEntityMap.at(patternTemp.synonym) : "";
		//string linkingEntityInput1 = patternInput1InSelect ? synonymEntityMap.at(patternTemp.input1) : "";
		// if valid query, then pattern will always be tied to assign entity. Then, the sqlResultStore will have "line_num" column
		if (patternSynonymInSelect) { 

		}

		for (string lineNum : patternLineNum) {
			
			if(!patternSynonymInSelect || !patternInput1InSelect) { break; } // exit if pattern not related to select
			for (SqlResult sqlResult : sqlResultStore.sqlResult) {
				for (auto it = sqlResult.row.begin(); it != sqlResult.row.end(); it++) {
					
					if (linkingEntity == "assign" && lineNum == it->second) {
						sqlResultPass.push_back(sqlResult);
					}
					else if(linkingEntity == "variable" && )
				}
			}
		}
		patternStack.pop();
	}

	// post process the results to fill in the output vector
	for (SqlResult sqlResult : sqlResultStore.sqlResult) {
		string result;
		for (auto it = sqlResult.row.begin(); it != sqlResult.row.end(); it++) {
			result += (*it).second + " ";
		}
		while (result.back() == ' ') { result.pop_back(); }
		output.push_back(result);
	}
	
	cout << "MY OUTPUT: ";
	for (int i = 0; i < output.size(); i++) {
		cout << output.at(i) << ",";
	}
	
	cout << endl;
}

int QueryProcessor::prec(char c) {
	if (c == '^')
		return 3;
	else if (c == '/' || c == '*')
		return 2;
	else if (c == '+' || c == '-')
		return 1;
	else
		return -1;
}

string QueryProcessor::infixToPostfix(string s) {

	stack<char> st;
	string result;

	for (int i = 0; i < s.length(); i++) {
		char c = s[i];
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
			result += c;
		else if (c == '(')
			st.push('(');
		else if (c == ')') {
			while (st.top() != '(')
			{
				result += st.top();
				st.pop();
			}
			st.pop();
		}
		else {
			while (!st.empty() && prec(s[i]) <= prec(st.top())) {
				result += st.top();
				st.pop();
			}
			st.push(c);
		}
	}

	while (!st.empty()) {
		result += st.top();
		st.pop();
	}

	return result;
}
