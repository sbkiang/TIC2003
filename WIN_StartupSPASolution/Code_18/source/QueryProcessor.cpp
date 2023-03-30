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

void QueryProcessor::EvaluateSelect(Select& select, map<string,string> synEntMap) {
	string columnName = "", stmtTable = "statement";
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
		else if (entity == "constant") {
			if (tableCountMap.find(entity) == tableCountMap.end()) { tableCountMap.insert(pair<string, int>(entity, 0)); }
			else { tableCountMap.at(entity)++; }
			columnName = "value";
			sprintf_s(sqlBuf, "c%s", to_string(tableCountMap.at(entity)).c_str());
			tableAlias = sqlBuf;
			sprintf_s(sqlBuf, "%s %s", entity.c_str(), tableAlias.c_str()); // E.g., constant c0, constant c1
		}
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
	map<string, string> synonymEntityMap; // map the synonym to entity
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

	// need to determine if the input to such that is generic or specific
	//	generic when it's (a synonym and not part of select) or (wildcard)
	//  specific when it's (a synonym and part of select) or ((not a synonym) and (not wildcard))
	vector<SqlResult> sqlResultPass;
	while (!suchThatStack.empty()) {
		SuchThat suchThatTemp = suchThatStack.top();
		bool input1IsSynonym = (synonymEntityMap.find(suchThatTemp.input1) != synonymEntityMap.end()); // first input is a synonym
		bool input2IsSynonym = (synonymEntityMap.find(suchThatTemp.input2) != synonymEntityMap.end()); // second input is a synonym		
		bool input1IsWildcard = (suchThatTemp.input1 == "_") ? true : false;
		bool input2IsWildcard = (suchThatTemp.input2 == "_") ? true : false;
		bool input1IsAny = (input1IsSynonym || input1IsWildcard);
		bool input2IsAny = (input2IsSynonym || input2IsWildcard);
		string entityInput1 = "", entityInput2 = "", first = suchThatTemp.input1, second = suchThatTemp.input2;
		if (input1IsSynonym) { // if input1 is part of declared synonym, we get the entity that it belongs
			entityInput1 = synonymEntityMap.at(suchThatTemp.input1);
		}
		if (input2IsSynonym) { // if input1 is part of declared synonym, we get the entity that it belongs
			entityInput2 = synonymEntityMap.at(suchThatTemp.input2);
		};
		SqlResultStore rs;
		string relationship = suchThatTemp.relationship;
		string frontSql = "";
		if (relationship == "Uses") { // input1 is Stmt Num or Name, input2 is Name
			if (input1IsSynonym && input2IsSynonym) {
				if (regex_match(entityInput1, regex(regexStmtNumEntity))) {
					frontSql = Uses::GetUsesConstruct_StatementSynonym_Synonym(suchThatTemp.input1, suchThatTemp.input2);
				}
				else if (entityInput1 == "procedure") {
					frontSql = Uses::GetUsesConstruct_NameSynonym_Synonym(suchThatTemp.input1, suchThatTemp.input2);
				}
			}
			else if (!input1IsSynonym && input2IsSynonym) {
				if (isdigit(suchThatTemp.input1[0])) {
					frontSql = Uses::GetUsesConstruct_StatementNotSynonym_Synonym(suchThatTemp.input2);
				}
				else {
					frontSql = Uses::GetUsesConstruct_NameNotSynonym_Synonym(suchThatTemp.input2);
				}
			}
			else if (input1IsSynonym && !input2IsSynonym) {
				if (regex_match(entityInput1, regex(regexStmtNumEntity))) {
					frontSql = Uses::GetUsesConstruct_StatementSynonym_NotSynonym(suchThatTemp.input1);
				}
				else if (entityInput1 == "procedure") {
					frontSql = Uses::GetUsesConstruct_NameSynonym_NotSynonym(suchThatTemp.input1);
				}
			}
			else if (!input1IsSynonym && !input2IsSynonym) {
				if (isdigit(suchThatTemp.input1[0])) {
					frontSql = Uses::GetUsesConstruct_StatementNotSynonym_NotSynonym();
				}
				else {
					frontSql = Uses::GetUsesConstruct_NameNotSynonym_NotSynonym();
				}
			}
			string sql = "";
			if (!input1IsAny && input2IsAny) { // Pattern("main"/5, variable/_)
				if (isdigit(suchThatTemp.input1[0])) { // for any stmt num, we need to get the entity first
					entityInput1 = DatabaseHelper::GetEntityByStatement(suchThatTemp.input1);
				}
				else {
					sql = Uses::GetUses_SpecificProcedure_Any(frontSql, suchThatTemp.input1);
				}
			}
			if (input1IsAny && input2IsAny) { // Pattern(entity/_, variable/_)
				if (entityInput1 == "procedure") { // Pattern(procedure, variable/_)
					sql = Uses::GetUses_AnyProcedure_Any(frontSql);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Pattern(print/assign, variable/_)
					sql = Uses::GetUses_AnyPrintAssign_Any(frontSql, entityInput1);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sql = Uses::GetUses_AnyWhileIf_Any(frontSql, entityInput1);
				}
				else if (entityInput1 == "call") {
					sql = Uses::GetUses_AnyCall_Any(frontSql);
				}
			}
			else if (!input1IsAny && input2IsAny) {
				if (entityInput1 == "procedure") { // Pattern(procedure, variable/_)
					sql = Uses::GetUses_SpecificProcedure_Any(frontSql, suchThatTemp.input1);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Pattern(print/assign, variable/_)
					sql = Uses::GetUses_SpecificPrintAssign_Any(frontSql, suchThatTemp.input1);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sql = Uses::GetUses_SpecificWhileIf_Any(frontSql, suchThatTemp.input1);
				}
				else if (entityInput1 == "call") {
					sql = Uses::GetUses_SpecificCall_Any(frontSql, suchThatTemp.input1);
				}
			}
			else if (input1IsAny && !input2IsAny) {
				if (entityInput1 == "procedure") { // Pattern(procedure, variable/_)
					sql = Uses::GetUses_AnyProcedure_Specific(frontSql, suchThatTemp.input2);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Pattern(print/assign, variable/_)
					sql = Uses::GetUses_AnyPrintAssign_Specific(frontSql, entityInput1, suchThatTemp.input2);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sql = Uses::GetUses_AnyWhileIf_Specific(frontSql, entityInput1, suchThatTemp.input2);
				}
				else if (entityInput1 == "call") {
					sql = Uses::GetUses_AnyCall_Specific(frontSql, suchThatTemp.input2);
				}
			}
			else if (!input1IsAny && !input2IsAny) {
				if (entityInput1 == "procedure") { // Pattern(procedure, variable/_)
					sql = Uses::GetUses_SpecificProcedure_Specific(frontSql, suchThatTemp.input1, suchThatTemp.input2);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Pattern(print/assign, variable/_)
					sql = Uses::GetUses_SpecificPrintAssign_Specific(frontSql, suchThatTemp.input1, suchThatTemp.input2);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sql = Uses::GetUses_SpecificWhileIf_Specific(frontSql, suchThatTemp.input1, suchThatTemp.input2);
				}
				else if (entityInput1 == "call") {
					sql = Uses::GetUses_SpecificCall_Specific(frontSql, suchThatTemp.input1, suchThatTemp.input2);
				}
			}
		}
		else if (relationship == "Modifies") { // input1 is Stmt Num or Name, input2 is Name or Wildcard
		}
		else if (relationship == "Parent") { // input1 is Stmt Num, input2 is Stmt Num
			//pass = Database::GetParent(first, second, input1IsSpecific, input2IsSpecific, entityInput1, entityInput2, rs);
		}
		else if (relationship == "Parent*") { // input1 is Stmt Num, input2 is Stmt Num
			//pass = Database::GetParentT(first, second, input1IsSpecific, input2IsSpecific, entityInput1, entityInput2, rs);
		}
		else if (relationship == "Next") {

		}
		else if (relationship == "Next*"){

		}
		else if (relationship == "Calls") {

		}
		else if (relationship == "Calls*") {

		}

		sqlResultStore.sqlResult = sqlResultPass;
		suchThatStack.pop();
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
			bool endWildCard = (second[second.length()-1] == '_');
			second = (startWildCard) ? second.substr(1, second.length()) : second;
			second = (endWildCard) ? second.substr(0, second.length() - 1) : second;
			second = infixToPostfix(second);
			second = (startWildCard) ? ("_" + second) : second;
			second = (endWildCard) ? (second + "_") : second;
		}
		string first = patternTemp.input1;
		second = ClausePattern::ConvertPqlPatternOprtToSqlPatternOprt(second);
		string sqlFront = input1IsSynonym ? ClausePattern::PatternConstruct_Synonym_NotSynonym(first) : ClausePattern::PatternConstruct_NotSynonym_NotSynonym();
		string sqlFull = (input1IsSynonym || first == "_") ? ClausePattern::GetPattern_Any_Expr(sqlFront, second) : ClausePattern::GetPattern_NotAny_Expr(sqlFront, first, second);
		SqlResultStore patternResult;
		Database::ExecuteSql(sqlFull, patternResult);
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
