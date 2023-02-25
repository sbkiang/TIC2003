#include "QueryProcessor.h"
#include <iostream>
#include <algorithm>
#include <stack>

// constructor
QueryProcessor::QueryProcessor() {}

// destructor
QueryProcessor::~QueryProcessor() {}

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
	string regexRelation = "Parent|Next|Calls|Modifies|Uses";
	map<string, string> synonymEntityMap;
	//vector<map<string, string>> synonymEntityMapVector;
	stack<SuchThat> suchThatStack;
	stack<Pattern> patternStack;
	Select select;
	for (int i = 0; i < tokens.size(); i++) {
		//change any uppercase char to lowercase
		//transform(tokens[i].begin(), tokens[i].end(), tokens[i].begin(), [](unsigned char c) { return std::tolower(c); });

		if (tokens[i] == "procedure" || tokens[i] == "variable" || tokens[i] == "constant" || tokens[i] == "call" || tokens[i] == "assign" || tokens[i] == "stmt" || tokens[i] == "read" || tokens[i] == "print" || tokens[i] == "while" || tokens[i] == "if") {
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
		else if (tokens[i] == "select") {
			selectIdx = i;
			i++;
			if (tokens.at(i) == "<") {
				while (tokens.at(i) != ">") {
					if (regex_match(tokens.at(i), regex(regexWord))) {
						select.synonyms.push_back(tokens.at(i));
						//select.synonymEntityMap.insert(pair<string, string>(tokens.at(i),synonymEntityMap.at(tokens.at(i))));
					}
					i++;
				}
			}
			else {
				select.synonyms.push_back(tokens.at(i));
				//select.synonymEntityMap.insert(pair<string, string>(tokens.at(i), synonymEntityMap.at(tokens.at(i))));
			}
		}
	}
	SqlResultSet selectPqlResultSet;
	Database::select(select, &selectPqlResultSet, synonymEntityMap);
	vector<SqlResultSet*> suchThatPqlResultVector;
	
	while (!suchThatStack.empty()) {
		SqlResultSet suchThatPqlResultSet;
	}


	/*
		Generate the database result for "select <synonym>".If is tuple, create SQL to join
		Create a Select function in database. Will take in synonym type, and synonym count
		Will craft SQL statement. If synonym count > 1, will create join SQL statements
			E.g., stmt a,b; Select <a,b> = SELECT s1.stmtNum, s2.stmtNum from statement s1 join statement s2. But hard to craft statement?
			E.g., stmt a,b; procedure p; Select <a,p> = SELECT line_num, proc_name from stmt a join procedure
		
		need to know the entity type for select table. so now, we get either the stmt num or name based on the entity type, joined together if needed
		then, need to know what each synonym's value for all the "such that" and "pattern". We pass in the synonym, and each synonym maps to the value
		can use the sqlRow and sqlRowSet. sqlRow store synonym-to-value map for each row
		
	*/
	if (suchThatIdx.empty() && patternIdx.empty()) { //Select Clause (Single)
		if (synonymType[0] == "procedure") {
			Database::getProcedures(databaseResults);
		}

		else if (synonymType[0] == "variable") {
			Database::getVariable(databaseResults);
		}

		else if (synonymType[0] == "assign" || synonymType[0] == "print" || synonymType[0] == "read" || synonymType[0] == "stmt" || synonymType[0] == "call") {
			Database::getStmt(synonymType[0], databaseResults);
		}

		else if (synonymType[0] == "constant") {
			Database::getConstant(databaseResults);
		}
	}
	else { //Select Clause (Multiple)
		if (suchThatIdx.size() > 0) { //More than 1 of "Such That" cause

			for (int i = 0; i < suchThatIdx.size(); i++) {
				idx = suchThatIdx[i];

				//change any uppercase char to lowercase
				transform(tokens[idx + 2].begin(), tokens[idx + 2].end(), tokens[idx + 2].begin(), [](unsigned char c) { return std::tolower(c); });

				designAbstract = tokens[idx + 2];

				if (tokens[idx + 3] == "*") {
					if (designAbstract == "parent") {
						designAbstract = designAbstract + "t";
						idx++;
					}
					else if (designAbstract == "calls") {
						designAbstract = designAbstract + "t";
						idx++;
					}
					else if (designAbstract == "next") {
						designAbstract = designAbstract + "t";
						idx++;
					}
				}

				idx = idx + 3;
				while (tokens[idx] != ")") {
					if (tokens[idx] == ",") {
						comma = true;
					}
					else if (tokens[idx] != "(" && tokens[idx] != "\"" && comma == false) {
						stmtNum1 = tokens[idx];
					}
					else if (tokens[idx] != "\"" && comma == true) {
						stmtNum2 = tokens[idx];
					}
					idx++;
				}
			}

			if (designAbstract == "parent" || designAbstract == "parentt") {
				//Relationship between statement
				if (!isNumber(stmtNum1) && isNumber(stmtNum2)) {
					Database::getParent(stmtNum1, stmtNum2, databaseResults);
				}
				else if(isNumber(stmtNum1) && !isNumber(stmtNum2)){
					Database::getChildren(stmtNum1, stmtNum2, tokens[0], databaseResults);
				}
				else {
					string resultType, filterType;
					string selectVar = tokens[selectIdx + 1];
					int i = 0;
					for (string _var : synonymVar) {
						if (selectVar == _var) {
							resultType = tokens[i];
						}
						else {
							filterType = tokens[i];
						}
						i = i + 3;
					}

					if (selectVar == stmtNum1) { //find parent
						Database::getParentChildren(1, resultType, filterType, databaseResults);
					}
					else { //find children
						Database::getParentChildren(0, resultType, filterType, databaseResults);
					}
	
				}
			}
			else if (designAbstract == "next") {
				//Relationship between statement
			}
			else if (designAbstract == "nextt") { //nextT = next* 
				//Relationship between statement
				// Use recursive query
			}
			else if (designAbstract == "calls") {
				//Relationship between procedure
			}
			else if (designAbstract == "callst") { //callT = call* 
				//Relationship between procedure
			}
			else if (designAbstract == "uses") {
				//Relationship between statement/procedure and variable

			}
			else if (designAbstract == "modifies") {
				//Relationship between statement/procedure and variable
				
				if (isNumber(stmtNum1) && !isNumber(stmtNum2)) { //For now, only work on variable query
					Database::getModifyStmt(stmtNum1, stmtNum2, 1, databaseResults);
				}
				else if (!isNumber(stmtNum1) && isNumber(stmtNum2)) { //For now, only work on variable query
					Database::getModifyStmt(stmtNum1, stmtNum2, 0, databaseResults);
				}
				else { //WIP
					string resultType, filterType;
					string selectVar = tokens[selectIdx + 1];
					int i = 0;
					for (string _var : synonymVar) {
						if (selectVar == _var) {
							resultType = tokens[i];
						}
						else {
							filterType = tokens[i];
						}
						i = i + 3;
					}

					//WIP

				}
			}
		}
		else if (patternIdx.size() > 0) {

		}
	}

	// call the method in database to retrieve the results
	// This logic is highly simplified based on iteration 1 requirements and 
	// the assumption that the queries are valid.
	/*if (synonymType == "procedure") {
		Database::getProcedures(databaseResults);
	}*/

	// post process the results to fill in the output vector
	for (string databaseResult : databaseResults) {
		output.push_back(databaseResult);
	}

}

