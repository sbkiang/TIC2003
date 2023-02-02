#include "SourceProcessor.h"
#include "..\SPA\Procedure.h"
#include "..\SPA\Container.h"

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
	bool elseFlag = false;
	int stmtNum = 0;
	for (int i = 0; i < tokens.size(); i++) {
		string word = tokens.at(i);
		if (word == "}") { // "}" indicates the end of a container
			if (!parentStack.empty()) { parentStack.pop(); }
		}
		else if (word == "procedure") {
			i++;
			Procedure* procedure = new Procedure(tokens.at(i));
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
			/*
			int brackets = 0;
			do { // Anything enclosed between the brackets is the if condition
				i++; // skip the "while" keyword
				if (tokens.at(i) == "(") {
					brackets++;
				}
				else if (tokens.at(i) == ")") {
					brackets--;
				}
				container->_condition += tokens.at(i);
				stmt->_stmt += tokens.at(i);
				if (regex_match(tokens.at(i), regex(var_constants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(var_regex))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i)));
				}
			} while (brackets != 0);
			*/
			while (tokens.at(i) != "{") {
				i++; // skip the "while" keyword
				container->_condition += tokens.at(i);
				stmt->_stmt += tokens.at(i);
				if (regex_match(tokens.at(i), regex(var_constants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(var_regex))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i)));
				}
			}
			container->_statements.push_back(stmt);
			Database::insertStatement(stmtNum, procedures.back()->_name, word, stmt->_stmt);
			for (int i = 0; i < variableStore.size(); i++) { // insert the variable after inserting the statement due to FK
				Database::insertVariable(variableStore.at(i)._stmt, variableStore.at(i)._stmtNum);
			}

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
		}
		else if (word == "else") { // set the flag for encountering "else"
			elseFlag = true;
			Container* container = new Container();
			container->_type = "else";
			ifElseLinkerStack.top()->_elsePtr = container; // the top stack will be the corresponding "if" container. We set this "else" container to the pointer
			parentStack.top()->_ifElseLinker.push_back(ifElseLinkerStack.top()); // parentStack.top() is this if-else container's parent. We add the this linker to it
			ifElseLinkerStack.pop(); // we pop the IfElseLinker as we've already processed the if-else
			if (!parentStack.empty()) { // if there's parent container, add current container to parent's child
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container); // we push the current "else" container to the parentStack for future statements			
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
		}
	}
}


// call linkStatements for each container, then join them up 
CFG* createCFG(Container* container) {
	if (!container) { return nullptr; }
	CFG* ownCFG = container->linkStatements();
	CFG* childCFG = nullptr;
	for (int i = 0; i < container->_childContainers.size(); i++) { // loop through current container's child containers
		Container* childContainer = container->_childContainers.at(i);
		childCFG = createCFG(childContainer); //DFS till end of tree, and create CFG
		if (container->_childContainers.at(i)->_type == "if") { // if the current child container is "if" container, the next one will be the "else" container
			i++;
			CFG* elseCFG = createCFG(container->_childContainers.at(i)); // create the CFG for the else 
			childCFG->_head->_fJump = elseCFG->_head; // set if CFG.fJump to else CFG.head
		}
		CFGNode* prevNode = ownCFG->getNode(childCFG->_head->_stmtPtr->_stmtNum - 1);
		CFGNode* temp = prevNode->_sJump;
		prevNode->_sJump = childCFG->_head;
		childCFG->_sTail->_sJump = temp;
		childCFG->_fTail->_sJump = temp; // is the _fTail pointing to the correct CFGNode?
		// Get the current container's CFGNode that needs to be linked with the Child's CFG
		/*
		*	if(...){	 --> stmt1
		*		stmt2
		*	}
		*	else{
		*		if(...){ --> stmt3
		*			stmt4
		*		}
		*		else{
		*			stmt5
		*		}
		*	}
		*	stmt1.sJump = stmt2
		*	stmt1.fJump = stmt3
		*	stmt3.sJump = stmt4
		*	stmt3.fJump = stmt5
		*
		*	childCFG = 3->4->5
		*	prevNode = 3-1 = 2
		*	But stmt2.sJump is not 3->4->5. Correct one is stmt1.fJump. But given my classes, I can't determine if stmt3 is under the if block or else block
		*	Seems to be an issue only for if-else because I need to consider the else block. For while, this works
		*   Maybe use a successJump stmtNum and failJump stmtNum to denote the first successJump statement and failJump statement?
		* 
		*	What if I put else as a container under if?

		*/
		//CFGNode* prevNode = ownCFG->getNode(childCFG->_head->_stmtPtr->_stmtNum - 1);
		if (prevNode) { // if able to find the CFGNode
			//prevNode->_sJump =
		}

		if (container->_type == "if") { // if current container is "if" container

		}
		if (childCFG) { // if there's a CFG from the child container
			if (childContainer->_type == "while") {

			}
		}
	}
}