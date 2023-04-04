#include "QueryProcessor.h"
#include <iostream>
#include <algorithm>
#include <stack>
#include <map>
#include <set>
#include <ranges>

// constructor
QueryProcessor::QueryProcessor() {}

// destructor
QueryProcessor::~QueryProcessor() {}

using namespace std;

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
		//if (entity != "stmt" && !regex_match(entity, regex(regexNameEntity))) { // if we are getting all the statement number entity, we need the "WHERE" SQL to filter based on entity
		if (regex_match(entity, regex(regexStmtNumEntityNoStmt))) { // if we are getting all the statement number entity, we need the "WHERE" SQL to filter based on entity
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
				//else if (!regex_match(tokens.at(i), regex(regexQuote))) {
				else{
					word += tokens.at(i);
				}
			} while (brackets > 0);
			int comma = word.find(",");
			st.input1 = word.substr(0, comma);
			st.input2 = word.substr(comma + 1, word.length());
			st.input1Quoted = st.input1[0] == '"';
			st.input2Quoted = st.input2[0] == '"';
			st.input1 = HelperFunction::RemoveQuote(st.input1);
			st.input2 = HelperFunction::RemoveQuote(st.input2);
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
				//if (!regex_match(tokens.at(i), regex(regexQuote))) {
				else{
					word += tokens.at(i);
				}
				i++;
			} while (brackets > 0);

			int comma = word.find(",");
			pt.input1 = word.substr(0, comma);
			pt.input2 = word.substr(comma + 1, word.length());
			pt.input1Quoted = pt.input1[0] == '"';
			pt.input1 = HelperFunction::RemoveQuote(pt.input1);
			pt.input2 = HelperFunction::RemoveQuote(pt.input2);
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
	SqlResultStore selectResultStore;
	EvaluateSelect(select, synonymEntityMap);
	Database::SelectPql(select, selectResultStore);
	set<string> currentResultSetSynonym(select.synonym.begin(), select.synonym.end());
	while (!suchThatStack.empty()) {
		SuchThat suchThatTemp = suchThatStack.top();
		string entityInput1 = "", entityInput2 = "", input1 = suchThatTemp.input1, input2 = suchThatTemp.input2;
		bool input1IsSynonym = (synonymEntityMap.find(input1) != synonymEntityMap.end() && !suchThatTemp.input1Quoted);
		bool input2IsSynonym = (synonymEntityMap.find(input2) != synonymEntityMap.end() && !suchThatTemp.input2Quoted);		
		bool input1IsWildcard = (input1 == "_") ? true : false;
		bool input2IsWildcard = (input2 == "_") ? true : false;
		bool input1InSelect = find(select.synonym.begin(), select.synonym.end(), input1) != select.synonym.end();
		bool input2InSelect = find(select.synonym.begin(), select.synonym.end(), input2) != select.synonym.end();
		set<string> stSynonym;
		if (input1IsSynonym) { // if input1 is part of declared synonym, we get the entity that it belongs
			entityInput1 = synonymEntityMap.at(input1);
			stSynonym.insert(input1);
		}
		if (input2IsSynonym) { // if input1 is part of declared synonym, we get the entity that it belongs
			entityInput2 = synonymEntityMap.at(input2);
			stSynonym.insert(input2);
		}
 		bool input1IsStmtOrWildCard = (entityInput1 == "stmt" || input1IsWildcard);
		bool input2IsStmtOrWildCard = (entityInput2 == "stmt" || input2IsWildcard);
		string relationship = suchThatTemp.relationship;
		string sql = "";
		if (relationship == "Uses") { // input1 is Stmt Num or Name, input2 is Name
			if (input1IsSynonym && input2IsSynonym) {
				if (regex_match(entityInput1, regex(regexStmtNumEntity))) {
					sql = Uses::GetUsesConstruct_StatementSynonym_Synonym(input1, input2);
				}
				else if (entityInput1 == "procedure") {
					sql = Uses::GetUsesConstruct_NameSynonym_Synonym(input1, input2);
				}
			}
			else if (!input1IsSynonym && input2IsSynonym) {
				if (isdigit(input1[0])) {
					sql = Uses::GetUsesConstruct_StatementNotSynonym_Synonym(input2);
				}
				else {
					sql = Uses::GetUsesConstruct_NameNotSynonym_Synonym(input2);
				}
			}
			else if (input1IsSynonym && !input2IsSynonym) {
				if (regex_match(entityInput1, regex(regexStmtNumEntity))) {
					sql = Uses::GetUsesConstruct_StatementSynonym_NotSynonym(input1);
				}
				else if (entityInput1 == "procedure") {
					sql = Uses::GetUsesConstruct_NameSynonym_NotSynonym(input1);
				}
			}
			else if (!input1IsSynonym && !input2IsSynonym) {
				if (isdigit(input1[0])) {
					sql = Uses::GetUsesConstruct_StatementNotSynonym_NotSynonym();
				}
				else {
					sql = Uses::GetUsesConstruct_NameNotSynonym_NotSynonym();
				}
			}
			bool input1IsAny = (input1IsSynonym || input1IsWildcard);
			bool input2IsAny = (input2IsSynonym || input2IsWildcard);

			if (!input1IsAny && input2IsAny) { // Uses("main"/5, variable/_)
				if (isdigit(input1[0])) { // for stmt num, we need to get the entity input1
					entityInput1 = HelperFunction::GetEntityByStatement(input1);
				}
				else {
					sql = Uses::GetUses_SpecificProcedure_Any(sql, input1);
				}
			}
			if (input1IsAny && input2IsAny) { // Uses(entity, variable/_)
				if (entityInput1 == "procedure") { // Uses(procedure, variable/_)
					sql = Uses::GetUses_AnyProcedure_Any(sql);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Uses(print/assign, variable/_)
					sql = Uses::GetUses_AnyPrintAssign_Any(sql, entityInput1);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sql = Uses::GetUses_AnyWhileIf_Any(sql, entityInput1);
				}
				else if (entityInput1 == "call") {
					sql = Uses::GetUses_AnyCall_Any(sql);
				}
				else if (input1IsStmtOrWildCard) {
					sql = Uses::GetUses_Any_Any(sql);
				}
			}
			else if (!input1IsAny && input2IsAny) { // entityInput1 here is already determine at the top
				if (entityInput1 == "procedure") { // Uses(procedure, variable/_)
					sql = Uses::GetUses_SpecificProcedure_Any(sql, input1);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Uses(print/assign, variable/_)
					sql = Uses::GetUses_SpecificPrintAssign_Any(sql, input1);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sql = Uses::GetUses_SpecificWhileIf_Any(sql, input1);
				}
				else if (entityInput1 == "call") {
					sql = Uses::GetUses_SpecificCall_Any(sql, input1);
				}
			}
			else if (input1IsAny && !input2IsAny) {
				if (entityInput1 == "procedure") { // Uses(procedure, variable/_)
					sql = Uses::GetUses_AnyProcedure_Specific(sql, input2);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Uses(print/assign, variable/_)
					sql = Uses::GetUses_AnyPrintAssign_Specific(sql, entityInput1, input2);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sql = Uses::GetUses_AnyWhileIf_Specific(sql, entityInput1, input2);
				}
				else if (entityInput1 == "call") {
					sql = Uses::GetUses_AnyCall_Specific(sql, input2);
				}
				else if (input1IsStmtOrWildCard) {
					sql = Uses::GetUses_Any_Specific(sql, input2);
				}
			}
			else if (!input1IsAny && !input2IsAny) { // entityInput1 here is already determine at the top
				if (entityInput1 == "procedure") { // Uses(procedure, variable/_)
					sql = Uses::GetUses_SpecificProcedure_Specific(sql, input1, input2);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Uses(print/assign, variable/_)
					sql = Uses::GetUses_SpecificPrintAssign_Specific(sql, input1, input2);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sql = Uses::GetUses_SpecificWhileIf_Specific(sql, input1, input2);
				}
				else if (entityInput1 == "call") {
					sql = Uses::GetUses_SpecificCall_Specific(sql, input1, input2);
				}
			}
		}

 		else if (relationship == "Modifies") { // input1 is Stmt Num or Name, input2 is Name or Wildcard
			if (input1IsSynonym && input2IsSynonym) {
				if (regex_match(entityInput1, regex(regexStmtNumEntity))) {
					sql = Modifies::GetModifiesConstruct_StatementSynonym_Synonym(input1, input2);
				}
				else if (entityInput1 == "procedure") {
					sql = Modifies::GetModifiesConstruct_NameSynonym_Synonym(input1, input2);
				}
			}
			else if (!input1IsSynonym && input2IsSynonym) {
				if (isdigit(input1[0])) {
					sql = Modifies::GetModifiesConstruct_StatementNotSynonym_Synonym(input2);
				}
				else {
					sql = Modifies::GetModifiesConstruct_NameNotSynonym_Synonym(input2);
				}
			}
			else if (input1IsSynonym && !input2IsSynonym) {
				if (regex_match(entityInput1, regex(regexStmtNumEntity))) {
					sql = Modifies::GetModifiesConstruct_StatementSynonym_NotSynonym(input1);
				}
				else if (entityInput1 == "procedure") {
					sql = Modifies::GetModifiesConstruct_NameSynonym_NotSynonym(input1);
				}
			}
			else if (!input1IsSynonym && !input2IsSynonym) {
				if (isdigit(input1[0])) {
					sql = Modifies::GetModifiesConstruct_StatementNotSynonym_NotSynonym();
				}
				else {
					sql = Modifies::GetModifiesConstruct_NameNotSynonym_NotSynonym();
				}
			}
			bool input1IsAny = (input1IsSynonym || input1IsWildcard);
			bool input2IsAny = (input2IsSynonym || input2IsWildcard);

			if (!input1IsAny && input2IsAny) { // Modifies("main"/5, variable/_)
				if (isdigit(input1[0])) { // for stmt num, we need to get the entity input1
					entityInput1 = HelperFunction::GetEntityByStatement(input1);
				}
				else {
					sql = Modifies::GetModifies_SpecificProcedure_Any(sql, input1);
				}
			}
			if (input1IsAny && input2IsAny) { // Modifies(entity, variable/_)
				if (entityInput1 == "procedure") { // Modifies(procedure, variable/_)
					sql = Modifies::GetModifies_AnyProcedure_Any(sql);
				}
				else if (regex_match(entityInput1, regex(regexAssignRead))) { // Modifies(print/assign, variable/_)
					sql = Modifies::GetModifies_AnyReadAssign_Any(sql, entityInput1);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sql = Modifies::GetModifies_AnyWhileIf_Any(sql, entityInput1);
				}
				else if (entityInput1 == "call") {
					sql = Modifies::GetModifies_AnyCall_Any(sql);
				}
				else if (input1IsStmtOrWildCard){
					sql = Modifies::GetModifies_Any_Any(sql);
				}
			}
			else if (!input1IsAny && input2IsAny) { // entityInput1 here is already determine at the top
				if (entityInput1 == "procedure") { // Modifies(procedure, variable/_)
					sql = Modifies::GetModifies_SpecificProcedure_Any(sql, input1);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Modifies(print/assign, variable/_)
					sql = Modifies::GetModifies_SpecificReadAssign_Any(sql, input1);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sql = Modifies::GetModifies_SpecificWhileIf_Any(sql, input1);
				}
				else if (entityInput1 == "call") {
					sql = Modifies::GetModifies_SpecificCall_Any(sql, input1);
				}
			}
			else if (input1IsAny && !input2IsAny) {
				if (entityInput1 == "procedure") { // Modifies(procedure, variable/_)
					sql = Modifies::GetModifies_AnyProcedure_Specific(sql, input2);
				}
				else if (regex_match(entityInput1, regex(regexAssignRead))) { // Modifies(print/assign, variable/_)
					sql = Modifies::GetModifies_AnyReadAssign_Specific(sql, entityInput1, input2);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sql = Modifies::GetModifies_AnyWhileIf_Specific(sql, entityInput1, input2);
				}
				else if (entityInput1 == "call") {
					sql = Modifies::GetModifies_AnyCall_Specific(sql, input2);
				}
				else if (input1IsStmtOrWildCard){
					sql = Modifies::GetModifies_Any_Specific(sql, input2);
				}
			}
			else if (!input1IsAny && !input2IsAny) { // entityInput1 here is already determine at the top
				if (entityInput1 == "procedure") { // Modifies(procedure, variable/_)
					sql = Modifies::GetModifies_SpecificProcedure_Specific(sql, input1, input2);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Modifies(print/assign, variable/_)
					sql = Modifies::GetModifies_SpecificReadAssign_Specific(sql, input1, input2);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sql = Modifies::GetModifies_SpecificWhileIf_Specific(sql, input1, input2);
				}
				else if (entityInput1 == "call") {
					sql = Modifies::GetModifies_SpecificCall_Specific(sql, input1, input2);
				}
			}
		}

		else if (relationship == "Parent") {
			bool input1IsGeneric = (input1IsStmtOrWildCard || input1IsSynonym);
			bool input2IsGeneric = (input2IsStmtOrWildCard || input2IsSynonym);

			if (input1IsSynonym && input2IsSynonym) { // Parent(entity, entity)
				sql = Parent::GetParentConstruct_Synonym_Synonym(input1, input2);
			}
			else if (!input1IsSynonym && input2IsSynonym) { // Parent(10, entity)
				sql = Parent::GetParentConstruct_NotSynonym_Synonym(input2);
			}
			else if (input1IsSynonym && !input2IsSynonym) { // Parent(entity, 10)
				sql = Parent::GetParentConstruct_Synonym_NotSynonym(input1);
			}
			else if (!input1IsSynonym && !input2IsSynonym) { // Parent(10, 11)
				sql = Parent::GetParentConstruct_NotSynonym_NotSynonym();
			}
			if (input1IsGeneric && input2IsGeneric) { // Parent(entity/_, entity/_)
				if (input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Parent(stmt/_, stmt/_)
					sql = Parent::GetParent_Any_Any(sql);
				}
				else if (input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Parent(stmt/_, entity_excld_stmt)
					sql = Parent::GetParent_Any_Synonym(sql, entityInput2);
				}
				else if (!input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Parent(entity_excld_stmt, stmt/_)
					sql = Parent::GetParent_Synonym_Any(sql, entityInput1);
				}
				else if (!input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Parent(entity_excld_stmt, entity_excld_stmt)
					sql = Parent::GetParent_Synonym_Synonym(sql, entityInput1, entityInput2);
				}
			}
			else if (input1IsGeneric && !input2IsGeneric) { // Parent(entity/_, 10)
				if (input1IsStmtOrWildCard) { // Parent(stmt/_, 10)
					sql = Parent::GetParent_Any_Specific(sql, input2);
				}
				else if (!input1IsStmtOrWildCard) { // Parent(entity_excld_stmt, 10)
					sql = Parent::GetParent_Synonym_Specific(sql, entityInput1, input2);
				}
			}
			else if (!input1IsGeneric && input2IsGeneric) { // Parent(10, entity/_)
				if (input2IsStmtOrWildCard) { // Parent(10, stmt/_)
					sql = Parent::GetParent_Specific_Any(sql, input1);
				}
				else if (!input2IsStmtOrWildCard) { // Parent(10, entity_excld_stmt)
					sql = Parent::GetParent_Specific_Synonym(sql, input1, entityInput2);
				}
			}
			else if (!input1IsGeneric && !input2IsGeneric) {// Parent(10,11)
				sql = Parent::GetParent_Specific_Specific(sql, input1, input2);
			}
		}

		else if (relationship == "Parent*") { // input1 is Stmt Num, input2 is Stmt Num
			if (input1IsSynonym && input2IsSynonym) { // Parent(stmt, stmt)
				sql = Parent::GetParentConstruct_Synonym_Synonym(input1, input2);
			}
			else if (!input1IsSynonym && input2IsSynonym) { // Parent(10, stmt)
				sql = Parent::GetParentConstruct_NotSynonym_Synonym(input2);
			}
			else if (input1IsSynonym && !input2IsSynonym) { // Parent(stmt, 10)
				sql = Parent::GetParentConstruct_Synonym_NotSynonym(input1);
			}
			else if (!input1IsSynonym && !input2IsSynonym) { // Parent(10, 11)
				sql = Parent::GetParentConstruct_NotSynonym_NotSynonym();
			}

			bool input1IsGeneric = (input1IsStmtOrWildCard || input1IsSynonym);
			bool input2IsGeneric = (input2IsStmtOrWildCard || input2IsSynonym);
			if (input1IsGeneric && input2IsGeneric) { // Parent(entity/_, entity/_)
				if (input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Parent(stmt/_, stmt/_)
					sql = Parent::GetParentT_Any_Any(sql);
				}
				else if (input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Parent(stmt/_, entity_excld_stmt)
					sql = Parent::GetParentT_Any_Synonym(sql, entityInput2);
				}
				else if (!input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Parent(entity_excld_stmt, stmt/_)
					sql = Parent::GetParentT_Synonym_Any(sql, entityInput1);
				}
				else if (!input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Parent(entity_excld_stmt, entity_excld_stmt)
					sql = Parent::GetParentT_Synonym_Synonym(sql, entityInput1, entityInput2);
				}
			}
			else if (input1IsGeneric && !input2IsGeneric) { // Parent(entity/_, 10)
				if (input1IsStmtOrWildCard) { // Parent(stmt/_, 10)
					sql = Parent::GetParentT_Any_Specific(sql, input2);
				}
				else if (!input1IsStmtOrWildCard) { // Parent(entity_excld_stmt, 10)
					sql = Parent::GetParentT_Synonym_Specific(sql, entityInput1, input2);
				}
			}
			else if (!input1IsGeneric && input2IsGeneric) { // Parent(10, entity/_)
				if (input2IsStmtOrWildCard) { // Parent(10, stmt/_)
					sql = Parent::GetParentT_Specific_Any(sql, input1);
				}
				else if (!input2IsStmtOrWildCard) { // Parent(10, entity_excld_stmt)
					sql = Parent::GetParentT_Specific_Synonym(sql, input1, entityInput2);
				}
			}
			else if (!input1IsGeneric && !input2IsGeneric) {// Parent(10,11)
				sql = Parent::GetParentT_Specific_Specific(sql, input1, input2);
			}
		}
			
		else if (relationship == "Next") {
			if (input1IsSynonym && input2IsSynonym) {
				sql = Next::NextConstructSelect_Synonym_Synonym(input1, input2);
			}
			else if (input1IsSynonym && !input2IsSynonym) {
				sql = Next::NextConstructSelect_Synonym_NotSynonym(input1);
			}
			else if (!input1IsSynonym && input2IsSynonym) {
				sql = Next::NextConstructSelect_NotSynonym_Synonym(input2);
			}
			else if (!input1IsSynonym && !input2IsSynonym){
				sql = Next::NextConstructSelect_NotSynonym_NotSynonym();
			}
			
			bool input1IsGeneric = (input1IsStmtOrWildCard || input1IsSynonym);
			bool input2IsGeneric = (input2IsStmtOrWildCard || input2IsSynonym);
			if (input1IsGeneric && input2IsGeneric) { // Next(entity/_, entity/_)
				if (input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Next(stmt/_, stmt/_)
					sql = Next::NextConstructQuery_Any_Any(sql);
				}
				else if (input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Next(stmt/_, stmtRef_excld_stmt)
					sql = Next::NextConstructQuery_Any_Synonym(sql, entityInput2);
				}
				else if (!input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Next(stmtRef_excld_stmt, stmt/_)
					sql = Next::NextConstructQuery_Synonym_Any(sql, entityInput1);
				}
				else if (!input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Next(stmtRef_excld_stmt, stmtRef_excld_stmt)
					sql = Next::NextConstructQuery_Synonym_Synonym(sql, entityInput1, entityInput2);
				}
			}
			else if (input1IsGeneric && !input2IsGeneric) { // Next(entity/_, 10)
				if (input1IsStmtOrWildCard) { // Next(stmt/_, 10)
					sql = Next::NextConstructQuery_Any_Specific(sql, input2);
				}
				else { // Next(stmtRef_excld_stmt, 10)
					sql = Next::NextConstructQuery_Synonym_Specific(sql, entityInput1, input2);
				}
			}
			else if (!input1IsGeneric && input2IsGeneric) { // Next(10, entity/_)
				if (input2IsStmtOrWildCard) { // Next(10, stmt/_)
					sql = Next::NextConstructQuery_Specific_Any(sql, input1);
				}
				else { // Next(10, stmtRef_excld_stmt)
					sql = Next::NextConstructQuery_Specific_Synonym(sql, input1, entityInput2);
				}
			}
			else if (!input1IsGeneric && !input2IsGeneric) { // Next(10, 11)
				sql = Next::NextConstructQuery_Specific_Specific(sql, input1, input2);
			}
		}
		else if (relationship == "Next*"){
			if (input1IsSynonym && input2IsSynonym) {
				sql = Next::NextConstructSelect_Synonym_Synonym(input1, input2);
			}
			else if (input1IsSynonym && !input2IsSynonym) {
				sql = Next::NextConstructSelect_Synonym_NotSynonym(input1);
			}
			else if (!input1IsSynonym && input2IsSynonym) {
				sql = Next::NextConstructSelect_NotSynonym_Synonym(input2);
			}
			else if (!input1IsSynonym && !input2IsSynonym) {
				sql = Next::NextConstructSelect_NotSynonym_NotSynonym();
			}

			bool input1IsGeneric = (input1IsStmtOrWildCard || input1IsSynonym);
			bool input2IsGeneric = (input2IsStmtOrWildCard || input2IsSynonym);
			if (input1IsGeneric && input2IsGeneric) { // Next(entity/_, entity/_)
				if (input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Next(stmt/_, stmt/_)
					sql = Next::NextTConstructQuery_Any_Any(sql);
				}
				else if (input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Next(stmt/_, stmtRef_excld_stmt)
					sql = Next::NextTConstructQuery_Any_Synonym(sql, entityInput2);
				}
				else if (!input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Next(stmtRef_excld_stmt, stmt/_)
					sql = Next::NextTConstructQuery_Synonym_Any(sql, entityInput1);
				}
				else if (!input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Next(stmtRef_excld_stmt, stmtRef_excld_stmt)
					sql = Next::NextTConstructQuery_Synonym_Synonym(sql, entityInput1, entityInput2);
				}
			}
			else if (input1IsGeneric && !input2IsGeneric) { // Next(entity/_, 10)
				if (input1IsStmtOrWildCard) { // Next(stmt/_, 10)
					sql = Next::NextTConstructQuery_Any_Specific(sql, input2);
				}
				else { // Next(stmtRef_excld_stmt, 10)
					sql = Next::NextTConstructQuery_Synonym_Specific(sql, entityInput1, input2);
				}
			}
			else if (!input1IsGeneric && input2IsGeneric) { // Next(10, entity/_)
				if (input2IsStmtOrWildCard) { // Next(10, stmt/_)
					sql = Next::NextTConstructQuery_Specific_Any(sql, input1);
				}
				else { // Next(10, stmtRef_excld_stmt)
					sql = Next::NextTConstructQuery_Specific_Synonym(sql, input1, entityInput2);
				}
			}
			else if (!input1IsGeneric && !input2IsGeneric) { // Next(10, 11)
				sql = Next::NextTConstructQuery_Specific_Specific(sql, input1, input2);
			}
		}
		else if (relationship == "Calls") {
			if (input1IsSynonym && input2IsSynonym) { // Call(procedure, procedure)
				sql = Call::GetCallConstruct_Synonym_Synonym(input1, input2);
			}
			else if (!input1IsSynonym && input2IsSynonym) { // Call("First"/_, procedure)
				sql = Call::GetCallConstruct_NotSynonym_Synonym(input2);
			}
			else if (input1IsSynonym && !input2IsSynonym) { // Call(procedure, "Second"/_)
				sql = Call::GetCallConstruct_Synonym_NotSynonym(input1);
			}
			else if (!input1IsSynonym && !input2IsSynonym) { // Call("First"/_,"Second"/_)
				if (input1IsStmtOrWildCard) { // Call(_,"Second") 
					sql = Call::GetCallConstruct_Any_Specific();
				}
				else if (input2IsStmtOrWildCard) { // Call("First", _) 
					sql = Call::GetCallConstruct_Specific_Any();
				}
				else { // Call("First","Second") 
					sql = Call::GetCallConstruct_NotSynonym_NotSynonym();
				}
			}

			bool input1IsGeneric = (input1IsStmtOrWildCard || input1IsSynonym);
			bool input2IsGeneric = (input2IsStmtOrWildCard || input2IsSynonym);

			if (input1IsGeneric && input2IsGeneric) { // Call(procedure/_, procedure/_)
				if (input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Call(_,_)
					sql = Call::GetCall_Any_Any(sql);
				}
				else if (input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Call(_, procedure)
					sql = Call::GetCall_Any_Synonym(sql);
				}
				else if (!input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Call(procedure, _)
					sql = Call::GetCall_Synonym_Any(sql);
				}
				else if (!input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Call(procedure, procedure)
					sql = Call::GetCall_Synonym_Synonym(sql);
				}
			}
			else if (input1IsGeneric && !input2IsGeneric) { // Call(procedure/_, "Second")
				if (input1IsStmtOrWildCard) { // Call(_, "Second")
					sql = Call::GetCall_Any_Specific(sql, input2);
				}
				else { // Call(procedure, "Second")
					sql = Call::GetCall_Synonym_Specific(sql, input2);
				}
			}
			else if (!input1IsGeneric && input2IsGeneric) { // Call(10, procedure/_)
				if (input2IsStmtOrWildCard) { // Call("First", _)
					sql = Call::GetCall_Specific_Any(sql, input1);
				}
				else { // Call("First", procedure)
					sql = Call::GetCall_Specific_Synonym(sql, input1);
				}
			}
			else if (!input1IsGeneric && !input2IsGeneric) { // Call("First", "Second")
				sql = Call::GetCall_Specific_Specific(sql, input1, input2);
			}
		}
		else if (relationship == "Calls*") {
		if (input1IsSynonym && input2IsSynonym) { // Call(procedure, procedure)
			sql = Call::GetCallConstruct_Synonym_Synonym(input1, input2);
		}
		else if (!input1IsSynonym && input2IsSynonym) { // Call("First"/_, procedure)
			sql = Call::GetCallConstruct_NotSynonym_Synonym(input2);
		}
		else if (input1IsSynonym && !input2IsSynonym) { // Call(procedure, "Second"/_)
			sql = Call::GetCallConstruct_Synonym_NotSynonym(input1);
		}
		else if (!input1IsSynonym && !input2IsSynonym) { // Call("First"/_,"Second"/_)
			if (input1IsStmtOrWildCard) { // Call(_,"Second") 
				sql = Call::GetCallConstruct_Any_Specific();
			}
			else if (input2IsStmtOrWildCard) { // Call("First", _) 
				sql = Call::GetCallConstruct_Specific_Any();
			}
			else { // Call("First","Second") 
				sql = Call::GetCallConstruct_NotSynonym_NotSynonym();
			}
		}

		bool input1IsGeneric = (input1IsStmtOrWildCard || input1IsSynonym);
		bool input2IsGeneric = (input2IsStmtOrWildCard || input2IsSynonym);

		if (input1IsGeneric && input2IsGeneric) { // Call(procedure/_, procedure/_)
			if (input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Call(_,_)
				sql = Call::GetCallT_Any_Any(sql);
			}
			else if (input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Call(_, procedure)
				sql = Call::GetCallT_Any_Synonym(sql);
			}
			else if (!input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Call(procedure, _)
				sql = Call::GetCallT_Synonym_Any(sql);
			}
			else if (!input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Call(procedure, procedure)
				sql = Call::GetCallT_Synonym_Synonym(sql);
			}
		}
		else if (input1IsGeneric && !input2IsGeneric) { // Call(procedure/_, "Second")
			if (input1IsStmtOrWildCard) { // Call(_, "Second")
				sql = Call::GetCallT_Any_Specific(sql, input2);
			}
			else { // Call(procedure, "Second")
				sql = Call::GetCallT_Synonym_Specific(sql, input2);
			}
		}
		else if (!input1IsGeneric && input2IsGeneric) { // Call(10, procedure/_)
			if (input2IsStmtOrWildCard) { // Call("First", _)
				sql = Call::GetCallT_Specific_Any(sql, input1);
			}
			else { // Call("First", procedure)
				sql = Call::GetCallT_Specific_Synonym(sql, input1);
			}
		}
		else if (!input1IsGeneric && !input2IsGeneric) { // Call("First", "Second")
			sql = Call::GetCallT_Specific_Specific(sql, input1, input2);
			}
		}

		SqlResultStore suchThatResultStore;
		Database::ExecuteSql(sql, suchThatResultStore);
		//HelperFunction::PrintRowSet(suchThatResultStore.sqlResultSet);

		//if there's common synonym between the table, perform intersect
		set<RowSet> selectResultSet = selectResultStore.sqlResultSet;
		set<RowSet> suchThatResultSet = suchThatResultStore.sqlResultSet;
		set<string> intersectSynonym;
		set_intersection(currentResultSetSynonym.begin(), currentResultSetSynonym.end(), stSynonym.begin(), stSynonym.end(), inserter(intersectSynonym, intersectSynonym.begin()));
		if(!intersectSynonym.empty()){
			selectResultStore.sqlResultSet = HelperFunction::CommonColumnIntersect(selectResultSet, suchThatResultSet);
			currentResultSetSynonym = intersectSynonym;
		}
		else {
			if (suchThatResultSet.empty()) {
				set<RowSet> empty;
				selectResultStore.sqlResultSet = empty;
				currentResultSetSynonym.clear();
			}
			else {
				selectResultStore.sqlResultSet = HelperFunction::CartesianProduct(selectResultSet, suchThatResultSet);
				currentResultSetSynonym.insert(stSynonym.begin(), stSynonym.end());
			}
		}
		//HelperFunction::PrintRowSet(selectResultStore.sqlResultSet);
		suchThatStack.pop();
	}

	while (!patternStack.empty()) {
		Pattern patternTemp = patternStack.top();
		string synonym = patternTemp.synonym, input1 = patternTemp.input1, input2 = patternTemp.input2;
		bool input1IsSynonym = (synonymEntityMap.find(input1) != synonymEntityMap.end() && !patternTemp.input1Quoted);
		bool patternSynonymInSelect = (find(select.synonym.begin(), select.synonym.end(), patternTemp.synonym) != select.synonym.end());
		string sql;
		/*
		if (patternSynonymInSelect && input1IsSynonym) {
			sql = ClausePattern::PatternConstruct_AssignInSelect_Synonym(synonym, input1);
		}
		else if (patternSynonymInSelect && !input1IsSynonym){
			sql = ClausePattern::PatternConstruct_AssignInSelect_NotSynonym(synonym);
		}
		else if (!patternSynonymInSelect && input1IsSynonym) {
			sql = ClausePattern::PatternConstruct_AssignNotInSelect_Synonym(input1);
		}
		else if (!patternSynonymInSelect && !input1IsSynonym) {
			sql = ClausePattern::PatternConstruct_AssignNotInSelect_NotSynonym();
		}
		*/
		set<string> ptSynonym;
		ptSynonym.insert(synonym);
		if (input1IsSynonym) {
			sql = ClausePattern::PatternConstruct_Synonym(synonym, input1);
			ptSynonym.insert(input1);
		}
		else {
			sql = ClausePattern::PatternConstruct_NotSynonym(synonym);
		}
		bool input1IsAny = (input1IsSynonym || input1 == "_");
		input1 = HelperFunction::ConvertPqlPatternOprtToSqlPatternOprt(input1);
		input2 = HelperFunction::PatternExprToPostFix(input2);
		input2 = HelperFunction::ConvertPqlPatternOprtToSqlPatternOprt(input2);
		if (input1IsAny) {
			sql = ClausePattern::GetPattern_Any_Expr(sql, input2);
		}
		else {
			sql = ClausePattern::GetPattern_NotAny_Expr(sql, input1, input2);
		}
		SqlResultStore patternResultStore;
		Database::ExecuteSql(sql, patternResultStore);
		set<RowSet> selectResultSet = selectResultStore.sqlResultSet;
		set<RowSet> patternResultSet = patternResultStore.sqlResultSet;
		set<string> intersectSynonym;
		bool patternInput1InSelect = (find(select.synonym.begin(), select.synonym.end(), input1) != select.synonym.end());
		set_intersection(currentResultSetSynonym.begin(), currentResultSetSynonym.end(), ptSynonym.begin(), ptSynonym.end(), inserter(intersectSynonym, intersectSynonym.begin()));
		if(!intersectSynonym.empty()){
			set<RowSet> intersection;
			selectResultStore.sqlResultSet = HelperFunction::CommonColumnIntersect(selectResultSet, patternResultSet);
			currentResultSetSynonym = intersectSynonym;
		}
		else {
			if (patternResultStore.sqlResultSet.empty()) {
				set<RowSet> empty;
				selectResultStore.sqlResultSet = empty;
				currentResultSetSynonym.clear();
			}
			else {
				selectResultStore.sqlResultSet = HelperFunction::CartesianProduct(selectResultSet, patternResultSet);
				currentResultSetSynonym.insert(ptSynonym.begin(), ptSynonym.end());
			}
		}
		//HelperFunction::PrintRowSet(patternResultSet);
		//HelperFunction::PrintRowSet(selectResultStore.sqlResultSet);
		patternStack.pop();
	}

	// post process the results to fill in the output vector
	set<RowSet> final;
	for (RowSet rs : selectResultStore.sqlResultSet) {
		RowSet ins;
		for (string synonym : select.synonym) {
			ins.row.insert(pair<string, string>(synonym, rs.row.at(synonym)));
		}
		final.insert(ins);
	}
	for (RowSet rs : final) {
		for (string synonym : select.synonym) {
			output.push_back(rs.row.at(synonym));
		}
	}
	
	cout << "MY OUTPUT: ";
	for (int i = 0; i < output.size(); i++) {
		cout << output.at(i) << ",";
	}
	
	cout << endl;
	
}

