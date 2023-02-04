#include "QueryProcessor.h"
#include "Tokenizer.h"
#include <iostream>

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
	string designAbstraction;


	for (int i = 0; i < tokens.size(); i++) {

		//check what type of synonym is
		if (tokens[i] == "procedure" || tokens[i] == "variable" || tokens[i] == "constant" || tokens[i] == "call" || tokens[i] == "assign" || tokens[i] == "stmt" || tokens[i] == "read" || tokens[i] == "print" || tokens[i] == "while" || tokens[i] == "if") {
			synonymType.push_back(tokens[i]);
		}

		if (tokens[i] == "Select") {
			selectIndex = i;
		}
		else if (tokens[i] == "such" && tokens[i + 1] == "that") {
			suchThatIndex = i;
			designAbstraction = tokens[i + 2];
		}
		else if (tokens[i] == "pattern") {
			patternIndex = i;
		}
	}

	// create a vector for storing the results from database
	vector<string> databaseResults;

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
		if (designAbstraction == "Parent") {
			//Relationship between statement
		}
		else if (designAbstraction == "ParentT") { //parentT = parent*
			//Relationship between statement
		}
		else if (designAbstraction == "Next") {
			//Relationship between statement
		}
		else if (designAbstraction == "NextT") { //nextT = next* 
			//Relationship between statement
		}
		else if (designAbstraction == "Calls") {
			//Relationship between procedure
		}
		else if (designAbstraction == "Calls") { //callT = call* 
			//Relationship between procedure
		}
		else if (designAbstraction == "Use") {
			//Relationship between statement/procedure and variable
		}
		else if (designAbstraction == "Modifies") {
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