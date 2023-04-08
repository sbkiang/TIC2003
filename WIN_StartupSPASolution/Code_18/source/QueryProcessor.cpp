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
	stack<ClRelRef> relRefStack;
	stack<ClPattern> patternStack;
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
			ClRelRef re = ClRelRef(relationship, input1, input2);
			relRefStack.push(re);
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
			ClPattern pt = ClPattern(synonym, input1, input2);
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
	while (!relRefStack.empty()) {
		ClRelRef relRefTemp = relRefStack.top();
		set<string> stSynonym;
		string relationship = relRefTemp.GetRelationship();
		string colSql, querySql, sql;
		DescriberClRelRef relEntDescriber = DescriberClRelRef(relRefTemp, synonymEntityMap);
		
		if (relEntDescriber.Input1IsSyn()) { // if input1 is part of declared synonym, we get the entity that it belongs
			stSynonym.insert(relRefTemp.GetInput1());
		}
		if (relEntDescriber.Input2IsSyn()) { // if input1 is part of declared synonym, we get the entity that it belongs
			stSynonym.insert(relRefTemp.GetInput2());
		}

		if (relationship == "Uses") { // input1 is Stmt Num or Name, input2 is Name
			ColumnBuilderSqlUses colBuilder = ColumnBuilderSqlUses(relRefTemp);
			colSql = colBuilder.GetSqlQuery(relEntDescriber);
			
			QueryBuilderSqlUses queryBuilder = QueryBuilderSqlUses(relRefTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);
		}

 		else if (relationship == "Modifies") { // input1 is Stmt Num or Name, input2 is Name or Wildcard
			ColumnBuilderSqlModifies colBuilder = ColumnBuilderSqlModifies(relRefTemp);
			colSql = colBuilder.GetSqlQuery(relEntDescriber);

			QueryBuilderSqlModifies queryBuilder = QueryBuilderSqlModifies(relRefTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);
		}

		else if (relationship == "Parent") {
			ColumnBuilderSqlParent colBuilder = ColumnBuilderSqlParent(relRefTemp);
			colSql = colBuilder.GetSqlQuery(relEntDescriber);

			QueryBuilderSqlParent queryBuilder = QueryBuilderSqlParent(relRefTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);
		}

		else if (relationship == "Parent*") { // input1 is Stmt Num, input2 is Stmt Num
			ColumnBuilderSqlParent colBuilder = ColumnBuilderSqlParent(relRefTemp);
			colSql = colBuilder.GetSqlQuery(relEntDescriber);
			
			QueryBuilderSqlParentT queryBuilder = QueryBuilderSqlParentT(relRefTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);
		}
			
		else if (relationship == "Next") {
			ColumnBuilderSqlNext colBuilderNext = ColumnBuilderSqlNext(relRefTemp);
			colSql = colBuilderNext.GetSqlQuery(relEntDescriber);
			
			QueryBuilderSqlNext queryBuilder = QueryBuilderSqlNext(relRefTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);
		}
		else if (relationship == "Next*"){
			ColumnBuilderSqlNext colBuilderNext = ColumnBuilderSqlNext(relRefTemp);
			colSql = colBuilderNext.GetSqlQuery(relEntDescriber);
			
			QueryBuilderSqlNextT queryBuilder = QueryBuilderSqlNextT(relRefTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);
		}
		else if (relationship == "Calls") {
			ColumnBuilderSqlCalls colBuilderNext = ColumnBuilderSqlCalls(relRefTemp);
			colSql = colBuilderNext.GetSqlQuery(relEntDescriber);
			
			QueryBuilderSqlCalls queryBuilder = QueryBuilderSqlCalls(relRefTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);
		}
		else if (relationship == "Calls*") {
			ColumnBuilderSqlCalls colBuilderNext = ColumnBuilderSqlCalls(relRefTemp);
			colSql = colBuilderNext.GetSqlQuery(relEntDescriber);
			
			QueryBuilderSqlCallsT queryBuilder = QueryBuilderSqlCallsT(relRefTemp);
			querySql = queryBuilder.GetSqlQuery(relEntDescriber);

			sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);
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
		relRefStack.pop();
	}

	while (!patternStack.empty()) {
		ClPattern patternTemp = patternStack.top();
		DescriberClPattern patternDescriber = DescriberClPattern(patternTemp, synonymEntityMap);
		ColumnBuilderSqlPattern colBuilderPattern = ColumnBuilderSqlPattern(patternTemp);
		string colSql = colBuilderPattern.GetSqlQuery(patternDescriber);

		QueryBuilderSqlPattern queryBuilder = QueryBuilderSqlPattern(patternTemp);
		string querySql = queryBuilder.GetSqlQuery(patternDescriber);

		string sql = HelperFunction::RelRefSqlBuilder(colSql, querySql);

		set<string> ptSynonym;
		ptSynonym.insert(patternTemp.GetSynonym());
		if (patternDescriber.Input1IsSyn()) {
			ptSynonym.insert(patternTemp.GetInput1());
		}

		SqlResultStore patternResultStore;
		Database::ExecuteSql(sql, patternResultStore);
		set<RowSet> selectResultSet = selectResultStore.sqlResultSet;
		set<RowSet> patternResultSet = patternResultStore.sqlResultSet;
		set<string> intersectSynonym;
		set_intersection(currentResultSetSynonym.begin(), currentResultSetSynonym.end(), ptSynonym.begin(), ptSynonym.end(), inserter(intersectSynonym, intersectSynonym.begin()));
		if(!intersectSynonym.empty()){
			set<RowSet> intersection;
			selectResultStore.sqlResultSet = HelperFunction::CommonColumnIntersect(selectResultSet, patternResultSet);
			currentResultSetSynonym.insert(ptSynonym.begin(), ptSynonym.end());
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

