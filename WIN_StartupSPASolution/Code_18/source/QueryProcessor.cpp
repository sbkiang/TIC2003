#include "QueryProcessor.h"
#include <chrono>

// constructor
QueryProcessor::QueryProcessor() {}

// destructor
QueryProcessor::~QueryProcessor() {}

using namespace std;
using namespace chrono;

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

	vector<string> constrainSql;

	while (!relRefStack.empty()) {
		ClRelRef relRefTemp = relRefStack.top();
		set<string> stSynonym;
		string relationship = relRefTemp.GetRelRefRelation();
		string colSql, querySql, sql;
		DescriberClRelRef relEntDescriber = DescriberClRelRef(relRefTemp, synonymEntityMap);
		
		if (relEntDescriber.Input1IsSyn()) { // if input1 is part of declared synonym, we get the entity that it belongs
			stSynonym.insert(relRefTemp.GetInput1());
		}
		if (relEntDescriber.Input2IsSyn()) { // if input1 is part of declared synonym, we get the entity that it belongs
			stSynonym.insert(relRefTemp.GetInput2());
		}


		if (relationship == "Uses") { // input1 is Stmt Num or Name, input2 is Name
			BuilderSqlColumnUses colBuilder = BuilderSqlColumnUses();
			colSql = colBuilder.GetSql(relRefTemp, relEntDescriber);
			
			BuilderSqlSelectUses queryBuilder = BuilderSqlSelectUses();
			querySql = queryBuilder.GetSql(relRefTemp, relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
			
		}

 		else if (relationship == "Modifies") { // input1 is Stmt Num or Name, input2 is Name or Wildcard
			BuilderSqlColumnModifies colBuilder = BuilderSqlColumnModifies();
			colSql = colBuilder.GetSql(relRefTemp, relEntDescriber);

			BuilderSqlSelectModifies queryBuilder = BuilderSqlSelectModifies();
			querySql = queryBuilder.GetSql(relRefTemp, relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}

		else if (relationship == "Parent") {
			BuilderSqlColumnParent colBuilder = BuilderSqlColumnParent();
			colSql = colBuilder.GetSql(relRefTemp, relEntDescriber);

			BuilderSqlSelectParent queryBuilder = BuilderSqlSelectParent();
			querySql = queryBuilder.GetSql(relRefTemp, relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}

		else if (relationship == "Parent*") { // input1 is Stmt Num, input2 is Stmt Num
			BuilderSqlColumnParent colBuilder = BuilderSqlColumnParent();
			colSql = colBuilder.GetSql(relRefTemp, relEntDescriber);
			
			BuilderSqlSelectParentT queryBuilder = BuilderSqlSelectParentT();
			querySql = queryBuilder.GetSql(relRefTemp, relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}
			
		else if (relationship == "Next") {
			BuilderSqlColumnNext colBuilderNext = BuilderSqlColumnNext();
			colSql = colBuilderNext.GetSql(relRefTemp, relEntDescriber);
			
			BuilderSqlSelectNext queryBuilder = BuilderSqlSelectNext();
			querySql = queryBuilder.GetSql(relRefTemp, relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}
		else if (relationship == "Next*"){
			BuilderSqlColumnNext colBuilderNext = BuilderSqlColumnNext();
			colSql = colBuilderNext.GetSql(relRefTemp, relEntDescriber);
			
			BuilderSqlSelectNextT queryBuilder = BuilderSqlSelectNextT();
			querySql = queryBuilder.GetSql(relRefTemp, relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}
		else if (relationship == "Calls") {
			BuilderSqlColumnCalls colBuilderNext = BuilderSqlColumnCalls();
			colSql = colBuilderNext.GetSql(relRefTemp, relEntDescriber);
			
			BuilderSqlSelectCalls queryBuilder = BuilderSqlSelectCalls();
			querySql = queryBuilder.GetSql(relRefTemp, relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}
		else if (relationship == "Calls*") {
			BuilderSqlColumnCalls colBuilderNext = BuilderSqlColumnCalls();
			colSql = colBuilderNext.GetSql(relRefTemp, relEntDescriber);
			
			BuilderSqlSelectCallsT queryBuilder = BuilderSqlSelectCallsT();
			querySql = queryBuilder.GetSql(relRefTemp, relEntDescriber);

			sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		}
		constrainSql.push_back(sql);
		relRefStack.pop();
	}

	while (!patternStack.empty()) {
		ClPattern patternTemp = patternStack.top();
		DescriberClPattern patternDescriber = DescriberClPattern(patternTemp, synonymEntityMap);
		BuilderSqlColumnPattern colBuilder = BuilderSqlColumnPattern();
		string colSql = colBuilder.GetSql(patternTemp, patternDescriber);

		BuilderSqlSelectPattern queryBuilder = BuilderSqlSelectPattern();
		string querySql = queryBuilder.GetSql(patternTemp, patternDescriber);

		string sql = BuilderFullSqlRelRefAndPattern::BuildSql(colSql, querySql);
		constrainSql.push_back(sql);

		set<string> ptSynonym;
		ptSynonym.insert(patternTemp.GetPatternSynonym());
		if (patternDescriber.Input1IsSyn()) {
			ptSynonym.insert(patternTemp.GetInput1());
		}
		patternStack.pop();
	}
	string sqlFinal = " ( " + sqlSelect + " ) "; // (select xxx from xxx)
	for (string sql : constrainSql) {
		sqlFinal += " natural join ( " + sql + " ) "; // (select xxx from xxx) natural join (select yyy from yyy)
	}
	string selectSynonymSql;
	for (string synonym : select.synonym) {
		selectSynonymSql += synonym + ",";
	}
	selectSynonymSql.pop_back(); // remove trailing ","
	selectSynonymSql = " select distinct " + selectSynonymSql + " from "; // select distinct x,y,z from
	sqlFinal = selectSynonymSql + " ( " + sqlFinal + " ) "; // select distinct x,y,z from ((select xxx from xxx) natural join (select yyy from yyy)) 
	SqlResultStore finalResultStore;
	Database::ExecuteSql(sqlFinal, finalResultStore);

	for (RowSet rs : finalResultStore.sqlResultSet) {
		string rowCombined;
		for (string syn : selectSynonym) {
			for (pair<string, string> pair : rs.row) {
				if (pair.first == syn) {
					rowCombined += pair.second + " ";
				}
			}
		}
		rowCombined.pop_back(); // remove trailing whitespace
		output.push_back(rowCombined);
	}

	//cout << "MY OUTPUT: ";
	//for (int i = 0; i < output.size(); i++) {
	//	cout << output.at(i) << ",";
	//}
	//cout << endl;
}

