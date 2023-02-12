#include "SourceProcessor.h"

// method for processing the source program
// This method currently only inserts the procedure name into the database
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all the required syntax.

void SourceProcessor::process(string program) {
	// initialize the database
	Database::initialize();

	// tokenize the program
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(program, tokens);

	string regexVariables = "^((?!(procedure|while|if|then|else|call|read|print)$)[A-Za-z][A-Za-z0-9]*)";
	string regexConstants = "^[0-9]+$";
	string regexUse = "[+\\-*/\\(\\)\\=\\d+\\!]";
	//string regexUse_2 = "\\d+";
	// This logic is highly simplified based on iteration 1 requirements and 
	// the assumption that the programs are valid.

	vector<Procedure*> procedures;
	stack<Container*> parentStack;
	int stmtNumSubtract = 0;
	int stmtNum = 0;
	int nestedLevel = 0;
	for (int i = 0; i < tokens.size(); i++) {
		string word = tokens.at(i);
		if (word == "}") { // "}" indicates the end of a container
			if (!parentStack.empty()) {
				nestedLevel--;
				parentStack.top()->_endStmtNum = stmtNum;
				parentStack.pop();
			}
		}
		else if (word == "procedure") {
			i++;
			Procedure* procedure = new Procedure(tokens.at(i));
			procedure->_type = "procedure";
			procedure->_startStmtNum = stmtNum + 1;
			procedure->_level = nestedLevel;
			procedures.push_back(procedure);
			parentStack.push(procedure);
			Database::insertProcedure(procedure->_name);
			nestedLevel++;
		}
		else if (word == "while") { // while(...){...}
			stmtNum++;
			nestedLevel++;
			Container* container = new Container();
			container->_type = "while";
			container->_startStmtNum = stmtNum;
			container->_level = nestedLevel;
			Statement* stmt = new Statement(stmtNum, nestedLevel, container, stmtNumSubtract);
			if (!parentStack.empty()) {
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container);
			vector<Statement> variableStore;
			vector<Statement> useStore;
			i++; // skip the "while" keyword
			while (tokens.at(i) != "{") {
				container->_condition += tokens.at(i);
				stmt->_stmt += tokens.at(i);
				if (regex_match(tokens.at(i), regex(regexConstants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(regexVariables))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i), container, stmtNumSubtract));
				}
				
				if (!regex_match(tokens.at(i), regex(regexUse))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), container, stmtNumSubtract));
				}
				i++;
			}
			container->_statements.push_back(stmt);
			Database::insertStatement(stmt->getAdjustedStmtNum(), procedures.back()->_name, word, stmt->_stmt);
			for (int i = 0; i < variableStore.size(); i++) { // insert the variable after inserting the statement due to FK
				Database::insertVariable(variableStore.at(i)._stmt, variableStore.at(i).getAdjustedStmtNum());
			}

			for (int i = 0; i < useStore.size(); i++) {
				Database::insertUse(useStore.at(i).getAdjustedStmtNum(), procedures.back()->_name, useStore.at(i)._stmt);
			}
		}
		else if (word == "if") { // if(...) then {...} else {...}
			stmtNum++;
			nestedLevel++;
			Container* container = new Container();
			container->_type = "if";
			container->_startStmtNum = stmtNum;
			container->_level = nestedLevel;
			Statement* stmt = new Statement(stmtNum, nestedLevel, container, stmtNumSubtract);
			if (!parentStack.empty()) { // if there's parent container, add current container to parent's child
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container); //set itself as the latest parent container
			vector<Statement> variableStore; // we need to insert statement first before inserting variable due to FK
			vector<Statement> useStore;
			i++; // skip the "if" keyword for the while loop below
			while (tokens.at(i) != "then") { // from current index till "then", it's the condition. "if(...) then{"
				container->_condition += tokens.at(i);
				stmt->_stmt += tokens.at(i);
				if (regex_match(tokens.at(i), regex(regexConstants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(regexVariables))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i), container, stmtNumSubtract));
				}

				// ** !regex_match(..), yet push to useStore. Mistake?
				if (!regex_match(tokens.at(i), regex(regexUse))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), container, stmtNumSubtract));
				}

				i++;
			}
			container->_statements.push_back(stmt);
			Database::insertStatement(stmt->getAdjustedStmtNum(), procedures.back()->_name, word, stmt->_stmt);
			for (int i = 0; i < variableStore.size(); i++) {
				Database::insertVariable(variableStore.at(i)._stmt, variableStore.at(i).getAdjustedStmtNum());
			}

			for (int i = 0; i < useStore.size(); i++) {
				Database::insertUse(useStore.at(i).getAdjustedStmtNum(), procedures.back()->_name, useStore.at(i)._stmt);
			}
		}
		else if (word == "else") { // for else container
			stmtNum++;
			nestedLevel++;
			stmtNumSubtract++;
			Container* container = new Container();
			container->_type = "else";
			Statement* stmt = new Statement(stmtNum, nestedLevel, container, stmtNumSubtract);
			stmt->_stmt = "else";
			container->_statements.push_back(stmt);
			container->_startStmtNum = stmtNum;
			container->_level = nestedLevel;
			stack<Container*> tempParentStack;
			if (!parentStack.empty()) { // if there's parent container, add current container to parent's child
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container); // we push the current "else" container to the parentStack for future statements
		}
		else if (word == "=") { // for assign
			stmtNum++;
			Statement* stmt = new Statement(stmtNum, nestedLevel, parentStack.top(), stmtNumSubtract);
			stmt->_stmt += tokens.at(i - 1);
			vector<Statement> variableStore; // we need to insert statement first before inserting variable due to FK. So, we store the variables here
			variableStore.push_back(Statement(stmtNum, tokens.at(i - 1), parentStack.top(), stmtNumSubtract));
			vector<Statement> useStore;
			vector<Statement> modifiesStore;
			variableStore.push_back(Statement(stmtNum, tokens.at(i - 1), parentStack.top(), stmtNumSubtract));
			//useStore.push_back(Statement(stmtNum, tokens.at(i + 1), nestedLevel));
			string LHS = tokens.at(i - 1);
			while (tokens.at(i) != ";") {
				stmt->_stmt += tokens.at(i);
				if (regex_match(tokens.at(i), regex(regexConstants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(regexVariables))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i), nestedLevel, parentStack.top(), stmtNumSubtract));
				}
				
				if (!regex_match(tokens.at(i), regex(regexUse))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), parentStack.top(), stmtNumSubtract));
				}
				i++;
			}
			modifiesStore.push_back(Statement(stmtNum, LHS, parentStack.top(), stmtNumSubtract)); //Store LHS variable
			parentStack.top()->_statements.push_back(stmt);
			Database::insertStatement(stmt->getAdjustedStmtNum(), procedures.back()->_name, "assign", stmt->_stmt);
			for (int i = 0; i < variableStore.size(); i++) {
				Database::insertVariable(variableStore.at(i)._stmt, variableStore.at(i).getAdjustedStmtNum());
			}

			for (int i = 0; i < useStore.size(); i++) {
				Database::insertUse(useStore.at(i).getAdjustedStmtNum(), procedures.back()->_name, useStore.at(i)._stmt);
			}

			for (int i = 0; i < modifiesStore.size(); i++) {
				Database::insertModifies(modifiesStore.at(i).getAdjustedStmtNum(), procedures.back()->_name, modifiesStore.at(i)._stmt);
			}
		}
		else if (word == "read" || word == "print" || word == "call") {
			stmtNum++;
			Statement* stmt = new Statement(stmtNum, nestedLevel, parentStack.top(), stmtNumSubtract);
			stmt->_stmt += tokens.at(i);
			stmt->_stmt += tokens.at(i + 1);
			parentStack.top()->_statements.push_back(stmt);
			Database::insertStatement(stmt->getAdjustedStmtNum(), procedures.back()->_name, word, stmt->_stmt);
			if (word == "read" || word == "print") {
				Database::insertVariable(tokens.at(i + 1), stmt->getAdjustedStmtNum());
			}
			if (word == "print") {
				vector<Statement> useStore;
				i++; // skip the "print" keyword for the while loop below
				while (tokens.at(i) != ";") {
					stmt->_stmt += tokens.at(i);
					if (!regex_match(tokens.at(i), regex(regexUse))) {
						useStore.push_back(Statement(stmt->getAdjustedStmtNum(), tokens.at(i), parentStack.top(), stmtNumSubtract));
					}
					i++;
				}
				for (int i = 0; i < useStore.size(); i++) {
					Database::insertUse(useStore.at(i).getAdjustedStmtNum(), procedures.back()->_name, useStore.at(i)._stmt);
				}
			}

			if (word == "read") {
				vector<Statement> modifiesStore;
				modifiesStore.push_back(Statement(stmtNum, tokens.at(i + 1), parentStack.top(), stmtNumSubtract));
				for (int i = 0; i < modifiesStore.size(); i++) {
					Database::insertModifies(modifiesStore.at(i).getAdjustedStmtNum(), procedures.back()->_name, modifiesStore.at(i)._stmt);
				}
			}
		}
	}
	vector<CFG*> CFGs;
	for (int i = 0; i < procedures.size(); i++) {
		procedures.at(i)->printContainerTree(0);
		CFG* cfg = CFGBuilder::buildCFG(procedures.at(i));
		vector<CFGNode*> nodes = cfg->getAllCFGNodes();
		for (int i = 0; i < nodes.size(); i++) {
			CFGNode* node = nodes.at(i);
			int nodeStmtNum = node->_stmtPtr->getAdjustedStmtNum();
			if (node->_sJump) {
				int nextStmtNum = node->_sJump->_stmtPtr->getAdjustedStmtNum();
				if (nodeStmtNum > nextStmtNum) { // only while loop back would have this occurence
					Database::insertNext(nodeStmtNum, nextStmtNum, "y");
				}
				else {
					Database::insertNext(nodeStmtNum, nextStmtNum, "n");
				}
			}
			if (node->_fJump) {
				int nextStmtNum = node->_fJump->_stmtPtr->getAdjustedStmtNum();
				if (nodeStmtNum > nextStmtNum) { // only while loop back would have this occurence
					Database::insertNext(nodeStmtNum, nextStmtNum, "y");
				}
				else {
					Database::insertNext(nodeStmtNum, nextStmtNum, "n");
				}
			}
		}
	}
}
