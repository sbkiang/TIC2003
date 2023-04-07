#include "QueryProcessor.h"

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
	stack<RelEnt> relEntStack;
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
			string relationship = tokens.at(i);
			if (tokens.at(i+1) == "*") {
				i++;
				relationship += tokens.at(i);
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
			string input1 = word.substr(0, comma);
			string input2 = word.substr(comma + 1, word.length());
			RelEnt re = RelEnt(relationship, input1, input2);
			relEntStack.push(re);
		}
		else if (tokens[i] == "pattern") {
			i++;
			string synonym = tokens.at(i);
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
			string input1 = word.substr(0, comma);
			string input2 = word.substr(comma + 1, word.length());
			Pattern pt = Pattern(synonym, input1, input2);
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
	while (!relEntStack.empty()) {
		RelEnt relEntTemp = relEntStack.top();
		set<string> stSynonym;
		string entityInput1 = "", entityInput2 = "", input1 = relEntTemp.GetInput1Unquoted(), input2 = relEntTemp.GetInput2Unquoted();
		bool input1Quoted = (relEntTemp.GetInput1()[0] == '"');
		bool input2Quoted = (relEntTemp.GetInput2()[0] == '"');
		bool input1IsSynonym = (synonymEntityMap.find(input1) != synonymEntityMap.end() && !input1Quoted);
		bool input2IsSynonym = (synonymEntityMap.find(input2) != synonymEntityMap.end() && !input2Quoted);		
		bool input1IsWildcard = (input1 == "_") ? true : false;
		bool input2IsWildcard = (input2 == "_") ? true : false;
		
		if (input1IsSynonym) { // if input1 is part of declared synonym, we get the entity that it belongs
			entityInput1 = synonymEntityMap.at(input1);
			stSynonym.insert(input1);
		}
		if (input2IsSynonym) { // if input1 is part of declared synonym, we get the entity that it belongs
			entityInput2 = synonymEntityMap.at(input2);
			stSynonym.insert(input2);
		}
		string sqlOld;
 		bool input1IsStmtOrWildCard = (entityInput1 == "stmt" || input1IsWildcard);
		bool input2IsStmtOrWildCard = (entityInput2 == "stmt" || input2IsWildcard);

		string relationship = relEntTemp.GetRelationship();
		string colSql, querySql, sql;
		RelEntDescriber relEntDescriber = RelEntDescriber(relEntTemp, synonymEntityMap);
		if (relationship == "Uses") { // input1 is Stmt Num or Name, input2 is Name
			ColumnBuilderSqlUses colBuilder = ColumnBuilderSqlUses(relEntTemp);
			colSql = colBuilder.GetSqlQuery(relEntDescriber);
			
			QueryBuilderSqlUses queryBuilder = QueryBuilderSqlUses(relEntTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);

			bool input1IsAny = (input1IsSynonym || input1IsWildcard);
			bool input2IsAny = (input2IsSynonym || input2IsWildcard);

			if (!input1IsAny) { // Uses("main"/5, variable/_)
				if (isdigit(input1[0])) { // for stmt num, we need to get the entity input1
					entityInput1 = HelperFunction::GetEntityByStatement(input1);
				}
			}
			if (input1IsAny && input2IsAny) { // Uses(entity, variable/_)
				if (entityInput1 == "procedure") { // Uses(procedure, variable/_)
					sqlOld = Uses::GetUses_AnyProcedure_Any(colSql);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Uses(print/assign, variable/_)
					sqlOld = Uses::GetUses_AnyPrintAssign_Any(colSql, entityInput1);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sqlOld = Uses::GetUses_AnyWhileIf_Any(colSql, entityInput1);
				}
				else if (entityInput1 == "call") {
					sqlOld = Uses::GetUses_AnyCall_Any(colSql);
				}
				else if (input1IsStmtOrWildCard) {
					sqlOld = Uses::GetUses_Any_Any(colSql);
				}
			}
			else if (!input1IsAny && input2IsAny) { // entityInput1 here is already determine at the top
				if (entityInput1 == "procedure") { // Uses(procedure, variable/_)
					sqlOld = Uses::GetUses_SpecificProcedure_Any(colSql, input1);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Uses(print/assign, variable/_)
					sqlOld = Uses::GetUses_SpecificPrintAssign_Any(colSql, input1);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sqlOld = Uses::GetUses_SpecificWhileIf_Any(colSql, input1);
				}
				else if (entityInput1 == "call") {
					sqlOld = Uses::GetUses_SpecificCall_Any(colSql, input1);
				}
			}
			else if (input1IsAny && !input2IsAny) {
				if (entityInput1 == "procedure") { // Uses(procedure, variable/_)
					sqlOld = Uses::GetUses_AnyProcedure_Specific(colSql, input2);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Uses(print/assign, variable/_)
					sqlOld = Uses::GetUses_AnyPrintAssign_Specific(colSql, entityInput1, input2);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sqlOld = Uses::GetUses_AnyWhileIf_Specific(colSql, entityInput1, input2);
				}
				else if (entityInput1 == "call") {
					sqlOld = Uses::GetUses_AnyCall_Specific(colSql, input2);
				}
				else if (input1IsStmtOrWildCard) {
					sqlOld = Uses::GetUses_Any_Specific(colSql, input2);
				}
			}
			else if (!input1IsAny && !input2IsAny) { // entityInput1 here is already determine at the top
				if (isalpha(input1[0])) { // Uses("main", "x")
					sqlOld = Uses::GetUses_SpecificProcedure_Specific(colSql, input1, input2);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Uses(print/assign stmt, "x")
					sqlOld = Uses::GetUses_SpecificPrintAssign_Specific(colSql, input1, input2);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) { // Uses(while/if stmt, "x")
					sqlOld = Uses::GetUses_SpecificWhileIf_Specific(colSql, input1, input2);
				} 
				else if (entityInput1 == "call") { // Uses(call stmt, "x")
					sqlOld = Uses::GetUses_SpecificCall_Specific(colSql, input1, input2);
				}
			}
		}

 		else if (relationship == "Modifies") { // input1 is Stmt Num or Name, input2 is Name or Wildcard
			ColumnBuilderSqlModifies colBuilder = ColumnBuilderSqlModifies(relEntTemp);
			colSql = colBuilder.GetSqlQuery(relEntDescriber);

			QueryBuilderSqlModifies queryBuilder = QueryBuilderSqlModifies(relEntTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);

			bool input1IsAny = (input1IsSynonym || input1IsWildcard);
			bool input2IsAny = (input2IsSynonym || input2IsWildcard);
			
			if (!input1IsAny) { // Modifies("main"/5, variable/_)
				if (isdigit(input1[0])) { // for stmt num, we need to get the entity input1
					entityInput1 = HelperFunction::GetEntityByStatement(input1);
				}
			}
			if (input1IsAny && input2IsAny) { // Modifies(entity, variable/_)
				if (entityInput1 == "procedure") { // Modifies(procedure, variable/_)
					sqlOld = Modifies::GetModifies_AnyProcedure_Any(colSql);
				}
				else if (regex_match(entityInput1, regex(regexAssignRead))) { // Modifies(print/assign, variable/_)
					sqlOld = Modifies::GetModifies_AnyReadAssign_Any(colSql, entityInput1);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sqlOld = Modifies::GetModifies_AnyWhileIf_Any(colSql, entityInput1);
				}
				else if (entityInput1 == "call") {
					sqlOld = Modifies::GetModifies_AnyCall_Any(colSql);
				}
				else if (input1IsStmtOrWildCard){
					sqlOld = Modifies::GetModifies_Any_Any(colSql);
				}
			}
			else if (!input1IsAny && input2IsAny) { // entityInput1 here is already determine at the top
				if (entityInput1 == "procedure") { // Modifies(procedure, variable/_)
					sqlOld = Modifies::GetModifies_SpecificProcedure_Any(colSql, input1);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Modifies(print/assign, variable/_)
					sqlOld = Modifies::GetModifies_SpecificReadAssign_Any(colSql, input1);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sqlOld = Modifies::GetModifies_SpecificWhileIf_Any(colSql, input1);
				}
				else if (entityInput1 == "call") {
					sqlOld = Modifies::GetModifies_SpecificCall_Any(colSql, input1);
				}
			}
			else if (input1IsAny && !input2IsAny) {
				if (entityInput1 == "procedure") { // Modifies(procedure, variable/_)
					sqlOld = Modifies::GetModifies_AnyProcedure_Specific(colSql, input2);
				}
				else if (regex_match(entityInput1, regex(regexAssignRead))) { // Modifies(print/assign, variable/_)
					sqlOld = Modifies::GetModifies_AnyReadAssign_Specific(colSql, entityInput1, input2);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) {
					sqlOld = Modifies::GetModifies_AnyWhileIf_Specific(colSql, entityInput1, input2);
				}
				else if (entityInput1 == "call") {
					sqlOld = Modifies::GetModifies_AnyCall_Specific(colSql, input2);
				}
				else if (input1IsStmtOrWildCard){
					sqlOld = Modifies::GetModifies_Any_Specific(colSql, input2);
				}
			}
			else if (!input1IsAny && !input2IsAny) { // entityInput1 here is already determine at the top
				if (isalpha(input1[0])) { // Modifies("main", "x")
					sqlOld = Modifies::GetModifies_SpecificProcedure_Specific(colSql, input1, input2);
				}
				else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Modifies(print/assign stmt, "x")
					sqlOld = Modifies::GetModifies_SpecificReadAssign_Specific(colSql, input1, input2);
				}
				else if (regex_match(entityInput1, regex(regexWhileIf))) { // Modifies(while/if stmt, "x")
					sqlOld = Modifies::GetModifies_SpecificWhileIf_Specific(colSql, input1, input2);
				}
				else if (entityInput1 == "call") { // Modifies(call stmt, "x")
					sqlOld = Modifies::GetModifies_SpecificCall_Specific(colSql, input1, input2);
				}
			}
		}

		else if (relationship == "Parent") {
			ColumnBuilderSqlParent colBuilder = ColumnBuilderSqlParent(relEntTemp);
			colSql = colBuilder.GetSqlQuery(relEntDescriber);

			QueryBuilderSqlParent queryBuilder = QueryBuilderSqlParent(relEntTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);

			bool input1IsGeneric = (input1IsStmtOrWildCard || input1IsSynonym);
			bool input2IsGeneric = (input2IsStmtOrWildCard || input2IsSynonym);
			
			if (input1IsGeneric && input2IsGeneric) { // Parent(entity/_, entity/_)
				if (input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Parent(stmt/_, stmt/_)
					sqlOld = Parent::GetParent_Any_Any(colSql);
				}
				else if (input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Parent(stmt/_, entity_excld_stmt)
					sqlOld = Parent::GetParent_Any_Synonym(colSql, entityInput2);
				}
				else if (!input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Parent(entity_excld_stmt, stmt/_)
					sqlOld = Parent::GetParent_Synonym_Any(colSql, entityInput1);
				}
				else if (!input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Parent(entity_excld_stmt, entity_excld_stmt)
					sqlOld = Parent::GetParent_Synonym_Synonym(colSql, entityInput1, entityInput2);
				}
			}
			else if (input1IsGeneric && !input2IsGeneric) { // Parent(entity/_, 10)
				if (input1IsStmtOrWildCard) { // Parent(stmt/_, 10)
					sqlOld = Parent::GetParent_Any_Specific(colSql, input2);
				}
				else if (!input1IsStmtOrWildCard) { // Parent(entity_excld_stmt, 10)
					sqlOld = Parent::GetParent_Synonym_Specific(colSql, entityInput1, input2);
				}
			}
			else if (!input1IsGeneric && input2IsGeneric) { // Parent(10, entity/_)
				if (input2IsStmtOrWildCard) { // Parent(10, stmt/_)
					sqlOld = Parent::GetParent_Specific_Any(colSql, input1);
				}
				else if (!input2IsStmtOrWildCard) { // Parent(10, entity_excld_stmt)
					sqlOld = Parent::GetParent_Specific_Synonym(colSql, input1, entityInput2);
				}
			}
			else if (!input1IsGeneric && !input2IsGeneric) {// Parent(10,11)
				sqlOld = Parent::GetParent_Specific_Specific(colSql, input1, input2);
			}
		}

		else if (relationship == "Parent*") { // input1 is Stmt Num, input2 is Stmt Num
			ColumnBuilderSqlParent colBuilder = ColumnBuilderSqlParent(relEntTemp);
			colSql = colBuilder.GetSqlQuery(relEntDescriber);
			
			QueryBuilderSqlParentT queryBuilder = QueryBuilderSqlParentT(relEntTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);

			bool input1IsGeneric = (input1IsStmtOrWildCard || input1IsSynonym);
			bool input2IsGeneric = (input2IsStmtOrWildCard || input2IsSynonym);
			if (input1IsGeneric && input2IsGeneric) { // Parent(entity/_, entity/_)
				if (input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Parent(stmt/_, stmt/_)
					sqlOld = Parent::GetParentT_Any_Any(colSql);
				}
				else if (input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Parent(stmt/_, entity_excld_stmt)
					sqlOld = Parent::GetParentT_Any_Synonym(colSql, entityInput2);
				}
				else if (!input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Parent(entity_excld_stmt, stmt/_)
					sqlOld = Parent::GetParentT_Synonym_Any(colSql, entityInput1);
				}
				else if (!input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Parent(entity_excld_stmt, entity_excld_stmt)
					sqlOld = Parent::GetParentT_Synonym_Synonym(colSql, entityInput1, entityInput2);
				}
			}
			else if (input1IsGeneric && !input2IsGeneric) { // Parent(entity/_, 10)
				if (input1IsStmtOrWildCard) { // Parent(stmt/_, 10)
					sqlOld = Parent::GetParentT_Any_Specific(colSql, input2);
				}
				else if (!input1IsStmtOrWildCard) { // Parent(entity_excld_stmt, 10)
					sqlOld = Parent::GetParentT_Synonym_Specific(colSql, entityInput1, input2);
				}
			}
			else if (!input1IsGeneric && input2IsGeneric) { // Parent(10, entity/_)
				if (input2IsStmtOrWildCard) { // Parent(10, stmt/_)
					sqlOld = Parent::GetParentT_Specific_Any(colSql, input1);
				}
				else if (!input2IsStmtOrWildCard) { // Parent(10, entity_excld_stmt)
					sqlOld = Parent::GetParentT_Specific_Synonym(colSql, input1, entityInput2);
				}
			}
			else if (!input1IsGeneric && !input2IsGeneric) {// Parent(10,11)
				sqlOld = Parent::GetParentT_Specific_Specific(colSql, input1, input2);
			}
		}
			
		else if (relationship == "Next") {
			ColumnBuilderSqlNext colBuilderNext = ColumnBuilderSqlNext(relEntTemp);
			colSql = colBuilderNext.GetSqlQuery(relEntDescriber);
			
			QueryBuilderSqlNext queryBuilder = QueryBuilderSqlNext(relEntTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);

			bool input1IsGeneric = (input1IsStmtOrWildCard || input1IsSynonym);
			bool input2IsGeneric = (input2IsStmtOrWildCard || input2IsSynonym);
			if (input1IsGeneric && input2IsGeneric) { // Next(entity/_, entity/_)
				if (input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Next(stmt/_, stmt/_)
					sqlOld = Next::NextConstructQuery_Any_Any(colSql);
				}
				else if (input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Next(stmt/_, stmtRef_excld_stmt)
					sqlOld = Next::NextConstructQuery_Any_Synonym(colSql, entityInput2);
				}
				else if (!input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Next(stmtRef_excld_stmt, stmt/_)
					sqlOld = Next::NextConstructQuery_Synonym_Any(colSql, entityInput1);
				}
				else if (!input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Next(stmtRef_excld_stmt, stmtRef_excld_stmt)
					sqlOld = Next::NextConstructQuery_Synonym_Synonym(colSql, entityInput1, entityInput2);
				}
			}
			else if (input1IsGeneric && !input2IsGeneric) { // Next(entity/_, 10)
				if (input1IsStmtOrWildCard) { // Next(stmt/_, 10)
					sqlOld = Next::NextConstructQuery_Any_Specific(colSql, input2);
				}
				else { // Next(stmtRef_excld_stmt, 10)
					sqlOld = Next::NextConstructQuery_Synonym_Specific(colSql, entityInput1, input2);
				}
			}
			else if (!input1IsGeneric && input2IsGeneric) { // Next(10, entity/_)
				if (input2IsStmtOrWildCard) { // Next(10, stmt/_)
					sqlOld = Next::NextConstructQuery_Specific_Any(colSql, input1);
				}
				else { // Next(10, stmtRef_excld_stmt)
					sqlOld = Next::NextConstructQuery_Specific_Synonym(colSql, input1, entityInput2);
				}
			}
			else if (!input1IsGeneric && !input2IsGeneric) { // Next(10, 11)
				sqlOld = Next::NextConstructQuery_Specific_Specific(colSql, input1, input2);
			}
		}
		else if (relationship == "Next*"){
			ColumnBuilderSqlNext colBuilderNext = ColumnBuilderSqlNext(relEntTemp);
			sql = colBuilderNext.GetSqlQuery(relEntDescriber);
			
			QueryBuilderSqlNextT queryBuilder = QueryBuilderSqlNextT(relEntTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);

			bool input1IsGeneric = (input1IsStmtOrWildCard || input1IsSynonym);
			bool input2IsGeneric = (input2IsStmtOrWildCard || input2IsSynonym);
			if (input1IsGeneric && input2IsGeneric) { // Next(entity/_, entity/_)
				if (input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Next(stmt/_, stmt/_)
					sqlOld = Next::NextTConstructQuery_Any_Any(colSql);
				}
				else if (input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Next(stmt/_, stmtRef_excld_stmt)
					sqlOld = Next::NextTConstructQuery_Any_Synonym(colSql, entityInput2);
				}
				else if (!input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Next(stmtRef_excld_stmt, stmt/_)
					sqlOld = Next::NextTConstructQuery_Synonym_Any(colSql, entityInput1);
				}
				else if (!input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Next(stmtRef_excld_stmt, stmtRef_excld_stmt)
					sqlOld = Next::NextTConstructQuery_Synonym_Synonym(colSql, entityInput1, entityInput2);
				}
			}
			else if (input1IsGeneric && !input2IsGeneric) { // Next(entity/_, 10)
				if (input1IsStmtOrWildCard) { // Next(stmt/_, 10)
					sqlOld = Next::NextTConstructQuery_Any_Specific(colSql, input2);
				}
				else { // Next(stmtRef_excld_stmt, 10)
					sqlOld = Next::NextTConstructQuery_Synonym_Specific(colSql, entityInput1, input2);
				}
			}
			else if (!input1IsGeneric && input2IsGeneric) { // Next(10, entity/_)
				if (input2IsStmtOrWildCard) { // Next(10, stmt/_)
					sqlOld = Next::NextTConstructQuery_Specific_Any(colSql, input1);
				}
				else { // Next(10, stmtRef_excld_stmt)
					sqlOld = Next::NextTConstructQuery_Specific_Synonym(colSql, input1, entityInput2);
				}
			}
			else if (!input1IsGeneric && !input2IsGeneric) { // Next(10, 11)
				sqlOld = Next::NextTConstructQuery_Specific_Specific(colSql, input1, input2);
			}
		}
		else if (relationship == "Calls") {
			ColumnBuilderSqlCalls colBuilderNext = ColumnBuilderSqlCalls(relEntTemp);
			sql = colBuilderNext.GetSqlQuery(relEntDescriber);
			
			QueryBuilderSqlCalls queryBuilder = QueryBuilderSqlCalls(relEntTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);
			
			bool input1IsGeneric = (input1IsStmtOrWildCard || input1IsSynonym);
			bool input2IsGeneric = (input2IsStmtOrWildCard || input2IsSynonym);

			if (input1IsGeneric && input2IsGeneric) { // Call(procedure/_, procedure/_)
				if (input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Call(_,_)
					sqlOld = Call::GetCall_Any_Any(colSql);
				}
				else if (input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Call(_, procedure)
					sqlOld = Call::GetCall_Any_Synonym(colSql);
				}
				else if (!input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Call(procedure, _)
					sqlOld = Call::GetCall_Synonym_Any(colSql);
				}
				else if (!input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Call(procedure, procedure)
					sqlOld = Call::GetCall_Synonym_Synonym(colSql);
				}
			}
			else if (input1IsGeneric && !input2IsGeneric) { // Call(procedure/_, "Second")
				if (input1IsStmtOrWildCard) { // Call(_, "Second")
					sqlOld = Call::GetCall_Any_Specific(colSql, input2);
				}
				else { // Call(procedure, "Second")
					sqlOld = Call::GetCall_Synonym_Specific(colSql, input2);
				}
			}
			else if (!input1IsGeneric && input2IsGeneric) { // Call("First", procedure/_)
				if (input2IsStmtOrWildCard) { // Call("First", _)
					sqlOld = Call::GetCall_Specific_Any(colSql, input1);
				}
				else { // Call("First", procedure)
					sqlOld = Call::GetCall_Specific_Synonym(colSql, input1);
				}
			}
			else if (!input1IsGeneric && !input2IsGeneric) { // Call("First", "Second")
				sqlOld = Call::GetCall_Specific_Specific(colSql, input1, input2);
			}
		}
		else if (relationship == "Calls*") {
			ColumnBuilderSqlCalls colBuilderNext = ColumnBuilderSqlCalls(relEntTemp);
			sql = colBuilderNext.GetSqlQuery(relEntDescriber);
			
			QueryBuilderSqlCallsT queryBuilder = QueryBuilderSqlCallsT(relEntTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);

			bool input1IsGeneric = (input1IsStmtOrWildCard || input1IsSynonym);
			bool input2IsGeneric = (input2IsStmtOrWildCard || input2IsSynonym);

			if (input1IsGeneric && input2IsGeneric) { // Call(procedure/_, procedure/_)
				if (input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Call(_,_)
					sqlOld = Call::GetCallT_Any_Any(colSql);
				}
				else if (input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Call(_, procedure)
					sqlOld = Call::GetCallT_Any_Synonym(colSql);
				}
				else if (!input1IsStmtOrWildCard && input2IsStmtOrWildCard) { // Call(procedure, _)
					sqlOld = Call::GetCallT_Synonym_Any(colSql);
				}
				else if (!input1IsStmtOrWildCard && !input2IsStmtOrWildCard) { // Call(procedure, procedure)
					sqlOld = Call::GetCallT_Synonym_Synonym(colSql);
				}
			}
			else if (input1IsGeneric && !input2IsGeneric) { // Call(procedure/_, "Second")
				if (input1IsStmtOrWildCard) { // Call(_, "Second")
					sqlOld = Call::GetCallT_Any_Specific(colSql, input2);
				}
				else { // Call(procedure, "Second")
					sqlOld = Call::GetCallT_Synonym_Specific(colSql, input2);
				}
			}
			else if (!input1IsGeneric && input2IsGeneric) { // Call("First", procedure/_)
				if (input2IsStmtOrWildCard) { // Call("First", _)
					sqlOld = Call::GetCallT_Specific_Any(colSql, input1);
				}
				else { // Call("First", procedure)
					sqlOld = Call::GetCallT_Specific_Synonym(colSql, input1);
				}
			}
			else if (!input1IsGeneric && !input2IsGeneric) { // Call("First", "Second")
				sqlOld = Call::GetCallT_Specific_Specific(colSql, input1, input2);
			}
		}

		SqlResultStore suchThatResultStore;
		Database::ExecuteSql(sql, suchThatResultStore);

		SqlResultStore suchThatResultStoreOld;
		Database::ExecuteSql(sqlOld, suchThatResultStoreOld);
		//HelperFunction::PrintRowSet(suchThatResultStore.sqlResultSet);

		//if there's common synonym between the table, perform intersect
		set<RowSet> selectResultSet = selectResultStore.sqlResultSet;
		set<RowSet> suchThatResultSet = suchThatResultStore.sqlResultSet;
		set<string> intersectSynonym;
		set_intersection(currentResultSetSynonym.begin(), currentResultSetSynonym.end(), stSynonym.begin(), stSynonym.end(), inserter(intersectSynonym, intersectSynonym.begin()));
		if(!intersectSynonym.empty()){
			selectResultStore.sqlResultSet = HelperFunction::CommonColumnIntersect(selectResultSet, suchThatResultSet);
			currentResultSetSynonym.insert(stSynonym.begin(), stSynonym.end());
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
		relEntStack.pop();
	}

	while (!patternStack.empty()) {
		Pattern patternTemp = patternStack.top();
		string synonym = patternTemp.GetSynonym(), input1 = patternTemp.GetInput1Unquoted(), input2 = patternTemp.GetInput2Unquoted();
		bool input1Quoted = (patternTemp.GetInput1()[0] == '"');
		bool input1IsSynonym = (synonymEntityMap.find(input1) != synonymEntityMap.end() && !input1Quoted);
		bool patternSynonymInSelect = (find(select.synonym.begin(), select.synonym.end(), patternTemp.GetSynonym()) != select.synonym.end());
		string sql;
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
			//currentResultSetSynonym = intersectSynonym;
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
	/*
	for (RowSet rs : final) {
		for (string synonym : select.synonym) {
			output.push_back(rs.row.at(synonym));
		}
	}
	*/
	for (RowSet rs : final) {
		string result = "";
		for (string synonym : select.synonym) {
			result += (rs.row.at(synonym) + " ");
		}
		while (result.back() == ' ') {
			result.pop_back();
		}
		output.push_back(result);
	}

	cout << "MY OUTPUT: ";
	for (int i = 0; i < output.size(); i++) {
		cout << output.at(i) << ",";
	}
	
	cout << endl;
	
}

