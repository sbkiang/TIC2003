#include "SourceProcessor.h"
#include "..\SPA\Procedure.h"
#include "..\SPA\Container.h"

// method for processing the source program
// This method currently only inserts the procedure name into the database
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all the required syntax.

CFG* createCFG(Container* container);

void SourceProcessor::process(string program) {
	// initialize the database
	Database::initialize();

	// tokenize the program
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(program, tokens);

	string var_regex = "^((?!(procedure|while|if|then|else|call|read|print)$)[A-Za-z][A-Za-z0-9]*)";
	string var_constants = "^[0-9]+$";
	// This logic is highly simplified based on iteration 1 requirements and 
	// the assumption that the programs are valid.

	//string procedureName = tokens.at(1);

	// insert the procedure into the database
	//Database::insertProcedure(procedureName);
	if (true) { {} {string test = "true"; } }

	vector<Procedure*> procedures;
	stack<Container*> parentStack;
	stack<IfElseLinker*> ifElseLinkerStack;
	int stmtNum = 0;
	int indent = 0;
	for (int i = 0; i < tokens.size(); i++) {
		string word = tokens.at(i);
		if (word == "}") { // "}" indicates the end of a container
			if (!parentStack.empty()) {
				indent--;
				parentStack.pop(); 
			}
		}
		else if (word == "procedure") {
			i++;
			Procedure* procedure = new Procedure(tokens.at(i));
			procedure->_type = "procedure";
			procedures.push_back(procedure);
			parentStack.push(procedure);
			Database::insertProcedure(procedure->_name);
		}
		else if (word == "while") { // while(...){...}
			stmtNum++;
			Container* container = new Container();
			container->_type = "while";
			Statement* stmt = new Statement(stmtNum);
			if (!parentStack.empty()) {
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container);
			vector<Statement> variableStore;
			i++; // skip the "while" keyword
			while (tokens.at(i) != "{") {
				container->_condition += tokens.at(i);
				stmt->_stmt += tokens.at(i);
				if (regex_match(tokens.at(i), regex(var_constants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(var_regex))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i)));
				}
				i++;
			}
			container->_statements.push_back(stmt);
			Database::insertStatement(stmtNum, procedures.back()->_name, word, stmt->_stmt);
			for (int i = 0; i < variableStore.size(); i++) { // insert the variable after inserting the statement due to FK
				Database::insertVariable(variableStore.at(i)._stmt, variableStore.at(i)._stmtNum);
			}
			for (int i = 0; i < indent; i++) { cout << "    "; }
			cout << stmtNum << " : " << word << stmt->_stmt << endl;
			indent++;
		}
		else if (word == "if") { // if(...) then {...} else {...}
			stmtNum++;
			Container* container = new Container();
			container->_type = "if";
			IfElseLinker* linker = new IfElseLinker(); // create a IfElseLinker
			linker->_ifPtr = container; // set the IfElseLinker's ifPtr to this "if" container
			ifElseLinkerStack.push(linker); // push it onto the stack for the corresponding "else" container
			Statement* stmt = new Statement(stmtNum);
			if (!parentStack.empty()) { // if there's parent container, add current container to parent's child
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container); //set itself as the latest parent container
			vector<Statement> variableStore; // we need to insert statement first before inserting variable due to FK
			i++ ; // skip the "if" keyword for the while loop below
			while (tokens.at(i) != "then") { // from current index till "then", it's the condition. "if(...) then{"
				container->_condition += tokens.at(i);
				stmt->_stmt += tokens.at(i);
				if (regex_match(tokens.at(i), regex(var_constants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(var_regex))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i)));
				}
				i++;
			}
			container->_statements.push_back(stmt);
			Database::insertStatement(stmtNum, procedures.back()->_name, word, stmt->_stmt);
			for (int i = 0; i < variableStore.size(); i++) {
				Database::insertVariable(variableStore.at(i)._stmt, variableStore.at(i)._stmtNum);
			}
			for (int i = 0; i < indent; i++) { cout << "    "; }
			cout << stmtNum << " : " << word << stmt->_stmt << endl;
			indent++;
		}
		else if (word == "else") { // for else container
			Container* container = new Container();
			container->_type = "else";
			ifElseLinkerStack.top()->_elsePtr = container; // the top stack will be the corresponding "if" container. We set this "else" container to the pointer
			parentStack.top()->_ifElseLinker.push_back(ifElseLinkerStack.top()); // parentStack.top() is this if-else container's parent. We add the this linker to it
			ifElseLinkerStack.pop(); // we pop the IfElseLinker as we've already processed the if-else
			if (!parentStack.empty()) { // if there's parent container, add current container to parent's child
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container); // we push the current "else" container to the parentStack for future statements
			for (int i = 0; i < indent; i++) { cout << "    "; }
			cout << stmtNum+1 << " : " << word << " " << endl;
			indent++;
		}
		else if (word == "=") { // for assign
			stmtNum++;
			Statement* stmt = new Statement(stmtNum);
			stmt->_stmt += tokens.at(i - 1);
			vector<Statement> variableStore; // we need to insert statement first before inserting variable due to FK. So, we store the variables here
			variableStore.push_back(Statement(stmtNum, tokens.at(i - 1)));
			string LHS = tokens.at(i - 1);
			while (tokens.at(i) != ";") {
				stmt->_stmt += tokens.at(i);
				if (regex_match(tokens.at(i), regex(var_constants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(var_regex))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i)));
				}
				i++;
			}
			parentStack.top()->_statements.push_back(stmt);
			Database::insertStatement(stmtNum, procedures.back()->_name, "assign", stmt->_stmt);
			for (int i = 0; i < variableStore.size(); i++) {
				Database::insertVariable(variableStore.at(i)._stmt, variableStore.at(i)._stmtNum);
			}
			for (int i = 0; i < indent; i++) { cout << "    "; }
			cout << stmtNum << " : " << stmt->_stmt << endl;
		}
		else if (word == "read" || word == "print" || word == "call") {
			stmtNum++;
			Statement* stmt = new Statement(stmtNum);
			stmt->_stmt += tokens.at(i);
			stmt->_stmt += tokens.at(i + 1);
			Database::insertStatement(stmtNum, procedures.back()->_name, word, stmt->_stmt);
			if (word == "read" || word == "print") {
				Database::insertVariable(tokens.at(i + 1), stmtNum);
			}
			parentStack.top()->_statements.push_back(stmt);
			for (int i = 0; i < indent; i++) { cout << "    "; }
			cout << stmtNum << " : " << stmt->_stmt << endl;
		}
	}
	//procedures.at(0)->printContainerTree(0);
	CFG* p1CFG = createCFG(procedures.at(0));
	p1CFG->printCFG();
}


// call linkStatements for each container, then join them up 
CFG* createCFG(Container* container) {
	if (!container) { return nullptr; }
	CFG* ownCFG = container->linkStatements(); // returns a CFG* object even if there's no statements.
	CFG* childCFG = nullptr;
	for (int i = 0; i < container->_childContainers.size(); i++) { // loop through current container's child containers
		ownCFG->printCFG();
		Container* childContainer = container->_childContainers.at(i);
		childCFG = createCFG(childContainer); // DFS till end of tree, and create CFG
		assert(childCFG != nullptr);
		string childContainerType = container->_childContainers.at(i)->_type;
		if (childContainerType == "if") { // if the current child container is "if" container, the next one will be the "else" container
			i++; // go to next childContainer = go to the corresponding "else" container
			CFG* elseCFG = createCFG(container->_childContainers.at(i)); // create the CFG for the else container
			childCFG->_head->_fJump = elseCFG->_head; // set "if" CFG.fJump to "else" CFG.head = "if" CFG fail condition next stmt points to else's stmt
			childCFG->_fTail = elseCFG->_sTail; // set "if" CFG.fTail to "else" CFG.sTail to maintain the correct CFG fTail after joining the "if" CFG and "else" CFG
		}
		if (!(ownCFG->_head)) { // if ownCFG head is null, then the ownCFG is empty = no statement in this container. We replace it with child CFG, and free the allocated memory for ownCFG
			delete(ownCFG);
			ownCFG = childCFG;
			continue;
		}
		CFGNode* prevNode = ownCFG->getNode(childCFG->_head->_stmtPtr->_stmtNum - 1); // find the node to link ownCFG with child CFG
		if (!prevNode) { // if prevNode is null, then the child CFG is the first statement. We append child CFG to this ownCFG
			CFGNode* tempOwnCFGHead = ownCFG->_head;
			ownCFG->_head = childCFG->_head;
			if (childContainerType == "if") { // for "if" container, we need to link childCFG sTail and fTail success jump to ownCFG head node
				childCFG->_sTail->_sJump = tempOwnCFGHead;
				childCFG->_fTail->_sJump = tempOwnCFGHead;
				
			}
			if (childContainerType == "while") { // for "while" container, we need to link childCFG head success jump to ownCFG head node
				childCFG->_head->_fJump = tempOwnCFGHead;
			}
		}
		if (prevNode) { // if prevNode is not null, then childCFG is not the first statement. It could be joined to ownCFG in the middle or end
			CFGNode* tempNode = prevNode->_sJump;

			// if prevNode is part of "if" container, set ownCFG sTail and fTail success jump to the childCFG head = add childCFG to ownCFG
			// this works as the previous childCFG would've set the correct ownCFG sTail and fTail
			prevNode->_sJump = childCFG->_head;
			if (prevNode->_container == "if" || prevNode->_container == "else") {
				ownCFG->_sTail->_sJump = childCFG->_head; // set sTail success jump to childCFG head because prevNode would be fTail since statements in else is always after statements in if
			}

			// if prevNode is part of "while" container, we need to set the while head fail jump to childCFG head = add childCFG to ownCFG
			else if (prevNode->_container == "while") {
				prevNode->_sJump->_fJump = childCFG->_head;
			}
			if (childContainerType == "if") { // if child is "if" container, we need to link both sTail and fTail success jump to the next node
				childCFG->_sTail->_sJump = tempNode;
				childCFG->_fTail->_sJump = tempNode;
				if (!tempNode) { // if the prevNode is the last node, then tempNode will be null. After appending childCFG, we need to set ownCFG sTail and fTail to the chidCFG fTail and sTail
					ownCFG->_fTail = childCFG->_fTail;
					ownCFG->_sTail = childCFG->_sTail;
				}
			}
			else if (childContainerType == "while") { // if child is "while" container, we need to link the childCFG head fail jump to the next node
				childCFG->_head->_fJump = tempNode;
				if (!tempNode) { // if the prevNode is the last node, then tempNode will be null. After appending childCFG, we need to set the ownCFG sTail to the correct node
					ownCFG->_sTail = childCFG->_head;
				}
			}

		}
	}
	return ownCFG;
}