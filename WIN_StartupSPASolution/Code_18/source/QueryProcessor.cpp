#include "QueryProcessor.h"

// constructor
QueryProcessor::QueryProcessor() {}

// destructor
QueryProcessor::~QueryProcessor() {}

using namespace std;

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
	vector<string> selectSynonym;
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
						selectSynonym.push_back(tokens.at(i));
					}
					i++;
				}
			}
			else {
				select.synonym.push_back(tokens.at(i));
				selectSynonym.push_back(tokens.at(i));
			}
		}
	}

	ClSelect clSelect = ClSelect(selectSynonym);
	DescriberClSelect describerClSelect = DescriberClSelect(clSelect, synonymEntityMap);
	BuilderFullSqlSelect builderSqlSelect;
	string sqlSelect = builderSqlSelect.GetSql(describerClSelect);

	SqlResultStore selectResultStore;
	Database::ExecuteSql(sqlSelect, selectResultStore);
	set<string> currentResultSetSynonymOld(selectSynonym.begin(), selectSynonym.end());

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
			BuilderSqlColumnUses colBuilder = BuilderSqlColumnUses(relRefTemp);
			colSql = colBuilder.GetSql(relEntDescriber);
			
			BuilderSqlSelectUses queryBuilder = BuilderSqlSelectUses(relRefTemp);
			querySql = queryBuilder.GetSql(relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
			
		}

 		else if (relationship == "Modifies") { // input1 is Stmt Num or Name, input2 is Name or Wildcard
			BuilderSqlColumnModifies colBuilder = BuilderSqlColumnModifies(relRefTemp);
			colSql = colBuilder.GetSql(relEntDescriber);

			BuilderSqlSelectModifies queryBuilder = BuilderSqlSelectModifies(relRefTemp);
			querySql = queryBuilder.GetSql(relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}

		else if (relationship == "Parent") {
			BuilderSqlColumnParent colBuilder = BuilderSqlColumnParent(relRefTemp);
			colSql = colBuilder.GetSql(relEntDescriber);

			BuilderSqlSelectParent queryBuilder = BuilderSqlSelectParent(relRefTemp);
			querySql = queryBuilder.GetSql(relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}

		else if (relationship == "Parent*") { // input1 is Stmt Num, input2 is Stmt Num
			BuilderSqlColumnParent colBuilder = BuilderSqlColumnParent(relRefTemp);
			colSql = colBuilder.GetSql(relEntDescriber);
			
			BuilderSqlSelectParentT queryBuilder = BuilderSqlSelectParentT(relRefTemp);
			querySql = queryBuilder.GetSql(relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}
			
		else if (relationship == "Next") {
			BuilderSqlColumnNext colBuilderNext = BuilderSqlColumnNext(relRefTemp);
			colSql = colBuilderNext.GetSql(relEntDescriber);
			
			BuilderSqlSelectNext queryBuilder = BuilderSqlSelectNext(relRefTemp);
			querySql = queryBuilder.GetSql(relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}
		else if (relationship == "Next*"){
			BuilderSqlColumnNext colBuilderNext = BuilderSqlColumnNext(relRefTemp);
			colSql = colBuilderNext.GetSql(relEntDescriber);
			
			BuilderSqlSelectNextT queryBuilder = BuilderSqlSelectNextT(relRefTemp);
			querySql = queryBuilder.GetSql(relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}
		else if (relationship == "Calls") {
			BuilderSqlColumnCalls colBuilderNext = BuilderSqlColumnCalls(relRefTemp);
			colSql = colBuilderNext.GetSql(relEntDescriber);
			
			BuilderSqlSelectCalls queryBuilder = BuilderSqlSelectCalls(relRefTemp);
			querySql = queryBuilder.GetSql(relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}
		else if (relationship == "Calls*") {
			BuilderSqlColumnCalls colBuilderNext = BuilderSqlColumnCalls(relRefTemp);
			colSql = colBuilderNext.GetSql(relEntDescriber);
			
			BuilderSqlSelectCallsT queryBuilder = BuilderSqlSelectCallsT(relRefTemp);
			querySql = queryBuilder.GetSql(relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}

		SqlResultStore suchThatResultStore;
		Database::ExecuteSql(sql, suchThatResultStore);

		//HelperFunction::PrintRowSet(suchThatResultStore.sqlResultSet);

		//if there's common synonym between the table, perform intersect
		set<RowSet> selectResultSet = selectResultStore.sqlResultSet;
		set<RowSet> suchThatResultSet = suchThatResultStore.sqlResultSet;
		set<string> selectResultSetSyn = HelperFunction::GetSynonymColInResultSet(selectResultSet, synonymEntityMap);
		set<string> suchThatResultSetSyn = HelperFunction::GetSynonymColInResultSet(selectResultSet, synonymEntityMap);
		set<string> intersectSynonym;
		set_intersection(selectResultSetSyn.begin(), selectResultSetSyn.end(), suchThatResultSetSyn.begin(), suchThatResultSetSyn.end(), inserter(intersectSynonym, intersectSynonym.begin()));
		if(!intersectSynonym.empty()){
			selectResultStore.sqlResultSet = HelperFunction::CommonColumnIntersect(selectResultSet, suchThatResultSet);
		}
		else {
			selectResultStore.sqlResultSet = HelperFunction::CartesianProduct(selectResultSet, suchThatResultSet);
		}
		//HelperFunction::PrintRowSet(selectResultStore.sqlResultSet);
		relRefStack.pop();
	}

	while (!patternStack.empty()) {
		ClPattern patternTemp = patternStack.top();
		DescriberClPattern patternDescriber = DescriberClPattern(patternTemp, synonymEntityMap);
		BuilderSqlColumnPattern colBuilderPattern = BuilderSqlColumnPattern(patternTemp);
		string colSql = colBuilderPattern.GetSql(patternDescriber);

		BuilderSqlSelectPattern queryBuilder = BuilderSqlSelectPattern(patternTemp);
		string querySql = queryBuilder.GetSql(patternDescriber);

		string sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);

		set<string> ptSynonym;
		ptSynonym.insert(patternTemp.GetSynonym());
		if (patternDescriber.Input1IsSyn()) {
			ptSynonym.insert(patternTemp.GetInput1());
		}

		SqlResultStore patternResultStore;
		Database::ExecuteSql(sql, patternResultStore);
		set<RowSet> selectResultSet = selectResultStore.sqlResultSet;
		set<RowSet> patternResultSet = patternResultStore.sqlResultSet;
		set<string> selectResultSetSyn = HelperFunction::GetSynonymColInResultSet(selectResultSet, synonymEntityMap);
		set<string> patternResultSetSyn = HelperFunction::GetSynonymColInResultSet(selectResultSet, synonymEntityMap);
		set<string> intersectSynonym;
		set_intersection(selectResultSetSyn.begin(), selectResultSetSyn.end(), patternResultSetSyn.begin(), patternResultSetSyn.end(), inserter(intersectSynonym, intersectSynonym.begin()));
		if(!intersectSynonym.empty()){
			selectResultStore.sqlResultSet = HelperFunction::CommonColumnIntersect(selectResultSet, patternResultSet);
		}
		else {
			selectResultStore.sqlResultSet = HelperFunction::CartesianProduct(selectResultSet, patternResultSet);
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

