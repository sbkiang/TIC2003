#include "SourceProcessor.h"

// method for processing the source program
// This method currently only inserts the procedure name into the database
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all the required syntax.

map<int, CFGNode*> _buildStatements(Container* container);
CFG* buildStatements(Container* container);
CFGNode* findNextStmt(stack<Container*> parentStack, int startStmtNum, map<int, CFGNode*> stmts);
void printStmt(map<int, CFGNode*> stmts);

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
	int elseStmtNumOffset = 0;
	int stmtNum = 0;
	int nestedLevel = 0;
	for (int i = 0; i < tokens.size(); i++) {
		string word = tokens.at(i);
		if (word == "}") { // "}" indicates the end of a container
			if (!parentStack.empty()) {
				nestedLevel--;
				parentStack.top()->_endStmtNum = stmtNum;
				if (parentStack.top()->_type == "else") { elseStmtNumOffset--; }
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
			Statement* stmt = new Statement(stmtNum, nestedLevel, container);
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
					variableStore.push_back(Statement(stmtNum, tokens.at(i)));
				}
				
				if (!regex_match(tokens.at(i), regex(regexUse))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), container)); 
				}

				i++;
			}

			container->_statements.push_back(stmt);
			Database::insertStatement(stmtNum, procedures.back()->_name, word, stmt->_stmt);
			for (int i = 0; i < variableStore.size(); i++) { // insert the variable after inserting the statement due to FK
				Database::insertVariable(variableStore.at(i)._stmt, variableStore.at(i)._stmtNum);
			}

			for (int i = 0; i < useStore.size(); i++) {
				Database::insertUse(useStore.at(i)._stmtNum, procedures.back()->_name, useStore.at(i)._stmt);
			}
		}
		else if (word == "if") { // if(...) then {...} else {...}
			stmtNum++;
			nestedLevel++;
			Container* container = new Container();
			container->_type = "if";
			container->_startStmtNum = stmtNum;
			container->_level = nestedLevel;
			Statement* stmt = new Statement(stmtNum, nestedLevel, container);
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
					variableStore.push_back(Statement(stmtNum, tokens.at(i)));
				}

				// ** !regex_match(..), yet push to useStore. Mistake?
				if (!regex_match(tokens.at(i), regex(regexUse))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), container));
				}

				i++;
			}
			container->_statements.push_back(stmt);
			Database::insertStatement(stmtNum, procedures.back()->_name, word, stmt->_stmt);
			for (int i = 0; i < variableStore.size(); i++) {
				Database::insertVariable(variableStore.at(i)._stmt, variableStore.at(i)._stmtNum);
			}

			for (int i = 0; i < useStore.size(); i++) {
				Database::insertUse(useStore.at(i)._stmtNum, procedures.back()->_name, useStore.at(i)._stmt);
			}
		}
		else if (word == "else") { // for else container
			stmtNum++;
			nestedLevel++;
			elseStmtNumOffset++;
			Container* container = new Container();
			container->_type = "else";
			Statement* stmt = new Statement(stmtNum, nestedLevel, container);
			stmt->_stmt = "else";
			container->_statements.push_back(stmt);
			container->_startStmtNum = stmtNum;
			container->_level = nestedLevel;
			if (!parentStack.empty()) { // if there's parent container, add current container to parent's child
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container); // we push the current "else" container to the parentStack for future statements
		}
		else if (word == "=") { // for assign
			stmtNum++;
			Statement* stmt = new Statement(stmtNum, nestedLevel, parentStack.top());
			stmt->_stmt += tokens.at(i - 1);
			vector<Statement> variableStore; // we need to insert statement first before inserting variable due to FK. So, we store the variables here
			variableStore.push_back(Statement(stmtNum, tokens.at(i - 1)));
			vector<Statement> useStore;
			vector<Statement> modifiesStore;
			variableStore.push_back(Statement(stmtNum, tokens.at(i - 1), parentStack.top()));
			//useStore.push_back(Statement(stmtNum, tokens.at(i + 1), nestedLevel));
			string LHS = tokens.at(i - 1);
			while (tokens.at(i) != ";") {
				stmt->_stmt += tokens.at(i);
				if (regex_match(tokens.at(i), regex(regexConstants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(regexVariables))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i), nestedLevel, parentStack.top()));
				}
				
				if (!regex_match(tokens.at(i), regex(regexUse))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), parentStack.top()));
				}
				
				i++;
			}
			modifiesStore.push_back(Statement(stmtNum, LHS, parentStack.top())); //Store LHS variable
			parentStack.top()->_statements.push_back(stmt);
			Database::insertStatement(stmtNum, procedures.back()->_name, "assign", stmt->_stmt);
			for (int i = 0; i < variableStore.size(); i++) {
				Database::insertVariable(variableStore.at(i)._stmt, variableStore.at(i)._stmtNum);
			}

			for (int i = 0; i < useStore.size(); i++) {
				Database::insertUse(useStore.at(i)._stmtNum, procedures.back()->_name, useStore.at(i)._stmt);
			}

			for (int i = 0; i < modifiesStore.size(); i++) {
				Database::insertModifies(modifiesStore.at(i)._stmtNum, procedures.back()->_name, modifiesStore.at(i)._stmt);
			}
		}
		else if (word == "read" || word == "print" || word == "call") {
			stmtNum++;
			Statement* stmt = new Statement(stmtNum, nestedLevel, parentStack.top());
			stmt->_stmt += tokens.at(i);
			stmt->_stmt += tokens.at(i + 1);
			parentStack.top()->_statements.push_back(stmt);
			Database::insertStatement(stmtNum, procedures.back()->_name, word, stmt->_stmt);
			if (word == "read" || word == "print") {
				Database::insertVariable(tokens.at(i + 1), stmtNum);
			}
			if (word == "print") {
				vector<Statement> useStore;
				i++; // skip the "print" keyword for the while loop below
				while (tokens.at(i) != ";") {
					stmt->_stmt += tokens.at(i);
					if (!regex_match(tokens.at(i), regex(regexUse))) {
						useStore.push_back(Statement(stmtNum, tokens.at(i), parentStack.top()));
					}
					i++;
				}
				for (int i = 0; i < useStore.size(); i++) {
					Database::insertUse(useStore.at(i)._stmtNum, procedures.back()->_name, useStore.at(i)._stmt);
				}
			}

			if (word == "read") {
				vector<Statement> modifiesStore;
				modifiesStore.push_back(Statement(stmtNum, tokens.at(i + 1), parentStack.top()));
				for (int i = 0; i < modifiesStore.size(); i++) {
					Database::insertModifies(modifiesStore.at(i)._stmtNum, procedures.back()->_name, modifiesStore.at(i)._stmt);
				}
			}
		}
	}
	vector<CFG*> CFGs;
	for (int i = 0; i < procedures.size(); i++) {
		procedures.at(i)->printContainerTree(0);
		CFG* cfg = CFGBuilder::buildCFG(procedures.at(i));
		cfg->printCFG();
	}
}

CFG* buildStatements(Container* container) {
	stack<CFGNode*> whileHeads;
	stack<CFGNode*> ifHeads;
	stack<Container*> parentStack;
	stack<Container*> tempParentStack;
	Container* tempContainer = container;
	map<int, CFGNode*> stmts = _buildStatements(container);
	printStmt(stmts);
	int startIndex = stmts.begin()->first;
	CFG* cfg = new CFG(stmts.at(startIndex));
	int loopStart = 0, loopEnd = 0;
	for (int i = startIndex; i < stmts.size() + startIndex; i++) {
		CFGNode* node = stmts.at(i);
		if (i == node->_stmtPtr->_container->_startStmtNum) {  // if current stmt is the container head, save to a tempContainer as it's the start of a new container
			if (tempContainer->_level < node->_stmtPtr->_container->_level) { // if parent container nestedLevel is lower than current node, then tempContainer is a parent container
				parentStack.push(tempContainer);
				tempContainer = node->_stmtPtr->_container;
			}
		}
		if (!(node->_stmtPtr->_containerHead || node->_stmtPtr->_containerTail)) {
			node->_sJump = stmts.at(i + 1);
			cout << "node " << node->_stmtPtr->_stmtNum << " sJump : " << node->_sJump->_stmtPtr->_stmtNum << endl;
			continue;
		}
 		if (node->_stmtPtr->_container->_type == "procedure") {
			if (stmts.find(i + 1) != stmts.end()) {
				node->_sJump = stmts.at(i + 1);
			}
		}
		else if (node->_stmtPtr->_container->_type == "while") {
			if (!node->_stmtPtr->_containerHead && node->_stmtPtr->_containerTail) { // if stmt is while container tail only, sJump back to head
				node->_sJump = stmts.at(node->_stmtPtr->_container->_startStmtNum);
			}
			else { // if stmt is while container head and tail, or only head, set sJump and sTail
				node->_sJump = stmts.at(i + 1);
				node->_fJump = findNextStmt(parentStack, node->_stmtPtr->_container->_endStmtNum + 1, stmts);
			} 
		}
		else if (node->_stmtPtr->_container->_type == "if") {
			if (!node->_stmtPtr->_containerHead && node->_stmtPtr->_containerTail) { // if stmt is if container tail only
				node->_sJump = findNextStmt(parentStack, node->_stmtPtr->_stmtNum + 1, stmts);
			}
			else { // if stmt is both if container head and tail, or head only, sJump = next stmt, fJump = else stmt
				node->_sJump = stmts.at(i + 1);
				loopStart = node->_stmtPtr->_container->_endStmtNum + 1; // skip self stmt
				loopEnd = parentStack.top()->_endStmtNum;
				for (int j = loopStart; j < loopEnd; j++) { // fJump is the first stmt in else container of same nestedLevel
					if (stmts.at(j)->_stmtPtr->_level != node->_stmtPtr->_level) {
						continue;
					}
					if (stmts.at(j)->_stmtPtr->_container->_type != "else") {
						continue;
					}
					node->_fJump = stmts.at(j);
					break;
				}
			} 
 		}
		else if (node->_stmtPtr->_container->_type == "else") {
			if (node->_stmtPtr->_containerHead && !node->_stmtPtr->_containerTail) { // if stmt is else container head only, sJump is next stmt
				node->_sJump = stmts.at(i + 1);
			}
			else{ // if stmt is else container tail only, or head and tail

				node->_sJump = findNextStmt(parentStack, node->_stmtPtr->_stmtNum + 1, stmts);
			}
		}
		while(!parentStack.empty()) {
			if (i == parentStack.top()->_endStmtNum) { // if current stmt is a container end, pop from parentStack. Place it at the end so that the pop occurs after everything have been processed
				parentStack.pop();
			}
			else {
				break;
			}

		}
		//if(node->_sJump){ cout << "node " << node->_stmtPtr->_stmtNum << " sJump : " << node->_sJump->_stmtPtr->_stmtNum << endl; }
		//if (node->_fJump) { cout << "node " << node->_stmtPtr->_stmtNum << " fJump : " << node->_fJump->_stmtPtr->_stmtNum << endl; }
	}
	return cfg;	
}

map<int, CFGNode*> _buildStatements(Container* container) {
	map<int, CFGNode*> myMap;
	for (int i = 0; i < container->_statements.size(); i++) {
		Statement* stmt = container->_statements.at(i);
		if (stmt->_stmtNum == (container->_startStmtNum)) {
			stmt->_containerHead = true;
		}
		if (stmt->_stmtNum == (container->_endStmtNum)) {
			stmt->_containerTail = true;
		}
		CFGNode* node = new CFGNode();
		node->_stmtPtr = stmt;
		myMap.insert(pair<int, CFGNode*>(stmt->_stmtNum, node));
	}
	for (int i = 0; i < container->_childContainers.size(); i++) {
		Container* childContainer = container->_childContainers.at(i);
		map<int, CFGNode*> childMap = _buildStatements(childContainer);
		myMap.insert(childMap.begin(), childMap.end());
	}
	return myMap;
}

// this function finds the next statement from startStmtNum to endStmtNum. StartStmtNum is normally the end of the current block, endStmtNum is normally the endStmtNum of parent block
//	1) if encounter else stmt, skip the block
//	2) if can't find the stmt,
//		2a) if parent container is while, loop back
//		2b) if parent container is not while, search from current block endStmtNum + 1 to parentBlock endStmtNum
CFGNode* findNextStmt(stack<Container*> parentStack, int startStmtNum, map<int, CFGNode*> stmts) {
	Container* currContainer = nullptr;
	CFGNode* nextStmt = nullptr;
	if (parentStack.empty()) { return nullptr; }
	int endStmtNum = parentStack.top()->_endStmtNum; // ** run createCFG 5 times to get to the 6th test
	for (int j = startStmtNum; j < endStmtNum + 1; j++) { // frind the next stmt that is a sibling or parent stmt
		if (stmts.at(j)->_stmtPtr->_container->_type == "else") { // if encounter and else stmt, it's going to be the else head. Skip the entire block
			if (j == stmts.at(j)->_stmtPtr->_container->_startStmtNum) {
				j = stmts.at(j)->_stmtPtr->_container->_endStmtNum;
				continue;
			}
		}
		if (stmts.at(startStmtNum)->_stmtPtr->_level >= stmts.at(j)->_stmtPtr->_level) { // siblings container = same nestedLevel level. parent container = lower nestedLevel level. So, >=
			nextStmt = stmts.at(j);
			break;
		}
	}
	if (!nextStmt) { // if can't find stmt
		if (parentStack.top()->_type == "while") { // if parent container is a while, then next stmt will be the while parent head
			return stmts.at(parentStack.top()->_startStmtNum);
		}
		else { // if parent container is not while, we continue to the higher parent
			currContainer = parentStack.top();
			parentStack.pop();
			nextStmt = findNextStmt(parentStack, currContainer->_endStmtNum + 1, stmts);
			parentStack.push(currContainer);
		}
	}
	return nextStmt;
}

void printStmt(map<int, CFGNode*> stmts) {
	cout << "   | H | T | " << endl;
	int startIndex = stmts.begin()->first;
	for (map<int, CFGNode*>::iterator it = stmts.begin(); it != stmts.end(); it++) {
		CFGNode* node = it->second;
		cout << setfill('0') << setw(2) << node->_stmtPtr->_stmtNum << " |";
		if (node->_stmtPtr->_containerHead) {
			cout << " Y |";
		}
		else {
			cout << "   |";
		}
		if (node->_stmtPtr->_containerTail) {
			cout << " Y | ";
		}
		else {
			cout << "   | ";
		}
		for (int i = 0; i < node->_stmtPtr->_level; i++) { cout << "  "; }
		if (node->_stmtPtr->_containerHead) {
			cout << node->_stmtPtr->_container->_type << " ";
		}
		cout << node->_stmtPtr->_stmt << endl;
	}
}