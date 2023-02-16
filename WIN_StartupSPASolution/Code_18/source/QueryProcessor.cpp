#include "QueryProcessor.h"
#include <iostream>
#include <algorithm>

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
	int idx, selectIdx;
	bool comma = false;

	for (int i = 0; i < tokens.size(); i++) {
		//change any uppercase char to lowercase
		transform(tokens[i].begin(), tokens[i].end(), tokens[i].begin(), [](unsigned char c) { return std::tolower(c); });

		if (tokens[i] == "procedure" || tokens[i] == "variable" || tokens[i] == "constant" || tokens[i] == "call" || tokens[i] == "assign" || tokens[i] == "stmt" || tokens[i] == "read" || tokens[i] == "print" || tokens[i] == "while" || tokens[i] == "if") {
			synonymType.push_back(tokens[i]);
			synonymVar.push_back(tokens[i + 1]);
		}
		else if (tokens[i] == "such" && tokens[i + 1] == "that") {
			suchThatIdx.push_back(i);
		}
		else if (tokens[i] == "pattern") {
			patternIdx.push_back(i);
		}
		else if (tokens[i] == "select") {
			selectIdx = i;
		}
	}

	if (suchThatIdx.empty() && patternIdx.empty()) { //Select Cause (Single)
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
	else { //Select Cause (Multiple)
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
			else if (designAbstract == "use") {
				//Relationship between statement/procedure and variable
			}
			else if (designAbstract == "modifies") {
				//Relationship between statement/procedure and variable
			}
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

