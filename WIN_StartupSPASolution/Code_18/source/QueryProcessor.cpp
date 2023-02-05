#include "QueryProcessor.h"
#include "Tokenizer.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

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
	vector<string> synonymType;

	int selectIndex = 0, suchThatIndex = 0, patternIndex = 0;

	// check for Next, Parent, Uses, Modifies, Call
	string designAbstraction = "";

	string firstStrIdx = "", secondStrIdx = "";
	int firstIntIdx = 0, secondIntIdx = 0;
	bool bracket = false, isNumber = true;

	for (int i = 0; i < tokens.size(); i++) {

		//check what type of synonym is
		if (tokens[i] == "procedure" || tokens[i] == "variable" || tokens[i] == "constant" || tokens[i] == "call" || tokens[i] == "assign" || tokens[i] == "stmt" || tokens[i] == "read" || tokens[i] == "print" || tokens[i] == "while" || tokens[i] == "if") {
			//uppercase to lowercase
			transform(tokens[i].begin(), tokens[i].end(), tokens[i].begin(), [](unsigned char c) { return std::tolower(c); });
			
			synonymType.push_back(tokens[i]);
		}

		if (tokens[i] == "Select") {
			selectIndex = i;
		}
		else if (tokens[i] == "such" && tokens[i + 1] == "that") {
			suchThatIndex = i;
			designAbstraction = tokens[i + 2];

			//uppercase to lowercase
			transform(designAbstraction.begin(), designAbstraction.end(), designAbstraction.begin(), [](unsigned char c) { return std::tolower(c); });

			if (tokens[i + 3] == "*") {
				if (designAbstraction == "parent") {
					designAbstraction = designAbstraction + "T";
				}
				else if (designAbstraction == "calls") {
					designAbstraction = designAbstraction + "T";
				}
				else if (designAbstraction == "next") {
					designAbstraction = designAbstraction + "T";
				}
			}

			while (tokens[i] != ")") {
				if(tokens[i] == "(" || bracket) {			
					if (tokens[i] == ",") {
						cout << "11";
						for (char c : tokens[i+1]) {
							if (!isdigit(c)) {
								cout << "55";
								isNumber = false;
								break;
							}
						}

						if (isNumber) {
							cout << "22";
							secondIntIdx = stoi(tokens[i+1]); //convert string to int
						}
						else 
							cout << "33";
							secondStrIdx = tokens[i+1];
					}
					else {
						for (char c : tokens[i]) {
							if (!isdigit(c)) {
								isNumber = false;
								break;
							}
						}
						if (isNumber)
							firstIntIdx = stoi(tokens[i]); //convert string to int
						else
							firstStrIdx = tokens[i];
					}
					bracket = true;
				}
				i++;
			}

		}
		else if (tokens[i] == "pattern") {
			patternIndex = i;
		}

	 
		

	}

	// create a vector for storing the results from database
	vector<string> databaseResults, synonymResults;

	

	// call the method in database to retrieve the results
	// This logic is highly simplified based on iteration 1 requirements and 
	// the assumption that the queries are valid.

	if (!suchThatIndex && !patternIndex) {
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
	else if (suchThatIndex) {
		if (designAbstraction == "parent") {
			//Relationship between statement
			if(synonymType[0] == "assign" || synonymType[0] == "print" || synonymType[0] == "read" || synonymType[0] == "stmt" || synonymType[0] == "call") {
				Database::getStmt(synonymType[0], synonymResults);
				cout << firstStrIdx << " @@ " << secondIntIdx;
;
			}
		}
		else if (designAbstraction == "parentT") { //parentT = parent*
			//Relationship between statement
		}
		else if (designAbstraction == "next") {
			//Relationship between statement
		}
		else if (designAbstraction == "nextT") { //nextT = next* 
			//Relationship between statement
		}
		else if (designAbstraction == "calls") {
			//Relationship between procedure
		}
		else if (designAbstraction == "callsT") { //callT = call* 
			//Relationship between procedure
		}
		else if (designAbstraction == "use") {
			//Relationship between statement/procedure and variable
		}
		else if (designAbstraction == "modifies") {
			//Relationship between statement/procedure and variable
		}
	}
	else if (patternIndex) {

	}

	// post process the results to fill in the output vector
	for (string databaseResult : databaseResults) {
		output.push_back(databaseResult);
	}
}