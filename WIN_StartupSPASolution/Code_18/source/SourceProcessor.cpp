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
	string var_use = "[+\\-*/\\(\\)\\=\\d+\\!]";
	//string var_use_2 = "\\d+";
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
			indent++;
		}
		else if (word == "while") { // while(...){...}
			stmtNum++;
			Container* container = new Container();
			container->_type = "while";
			Statement* stmt = new Statement(stmtNum, indent+1);
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
				if (regex_match(tokens.at(i), regex(var_constants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(var_regex))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i), indent));
				}
				
				if (!regex_match(tokens.at(i), regex(var_use))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), indent));
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

			cout << setfill('0') << setw(2) << stmtNum << " | ";
			for (int i = 0; i < indent; i++) { cout << "    "; }
			cout << word << stmt->_stmt << endl;
			indent++;
		}
		else if (word == "if") { // if(...) then {...} else {...}
			stmtNum++;
			Container* container = new Container();
			container->_type = "if";
			IfElseLinker* linker = new IfElseLinker(); // create a IfElseLinker
			linker->_ifPtr = container; // set the IfElseLinker's ifPtr to this "if" container
			ifElseLinkerStack.push(linker); // push it onto the stack for the corresponding "else" container
			Statement* stmt = new Statement(stmtNum, indent+1);
			if (!parentStack.empty()) { // if there's parent container, add current container to parent's child
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container); //set itself as the latest parent container
			vector<Statement> variableStore; // we need to insert statement first before inserting variable due to FK
			vector<Statement> useStore;
			i++ ; // skip the "if" keyword for the while loop below
			while (tokens.at(i) != "then") { // from current index till "then", it's the condition. "if(...) then{"
				container->_condition += tokens.at(i);
				stmt->_stmt += tokens.at(i);
				if (regex_match(tokens.at(i), regex(var_constants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(var_regex))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i), indent));
				}

				if (!regex_match(tokens.at(i), regex(var_use))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), indent));
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

			cout << setfill('0') << setw(2) << stmtNum << " | ";
			for (int i = 0; i < indent; i++) { cout << "    "; }
			cout << word << stmt->_stmt << endl;
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
			cout << setfill('0') << setw(2) << stmtNum+1 << " | ";
			for (int i = 0; i < indent; i++) { cout << "    "; }
			cout << word << " " << endl;
			indent++;
		}
		else if (word == "=") { // for assign
			stmtNum++;
			Statement* stmt = new Statement(stmtNum);
			stmt->_stmt += tokens.at(i - 1);
			vector<Statement> variableStore; // we need to insert statement first before inserting variable due to FK. So, we store the variables here
			vector<Statement> useStore;
			vector<Statement> modifiesStore;
			variableStore.push_back(Statement(stmtNum, tokens.at(i - 1), indent));
			//useStore.push_back(Statement(stmtNum, tokens.at(i + 1), indent));
			string LHS = tokens.at(i - 1);
			while (tokens.at(i) != ";") {
				stmt->_stmt += tokens.at(i);
				if (regex_match(tokens.at(i), regex(var_constants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(var_regex))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i), indent));
				}
				
				if (!regex_match(tokens.at(i), regex(var_use))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), indent));
				}
				
				i++;
			}

			modifiesStore.push_back(Statement(stmtNum, LHS, indent)); //Store LHS variable

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


			cout << setfill('0') << setw(2) << stmtNum << " | ";
			for (int i = 0; i < indent; i++) { cout << "    "; }
			cout << stmt->_stmt << endl;
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
			
			if (word == "print") {
				vector<Statement> useStore;
				i++; // skip the "print" keyword for the while loop below
				while (tokens.at(i) != ";") {
					stmt->_stmt += tokens.at(i);
					if (!regex_match(tokens.at(i), regex(var_use))) {
						useStore.push_back(Statement(stmtNum, tokens.at(i), indent));
					}
					i++;
				}

				for (int i = 0; i < useStore.size(); i++) {
					Database::insertUse(useStore.at(i)._stmtNum, procedures.back()->_name, useStore.at(i)._stmt);
				}

			}

			if (word == "read") {
				vector<Statement> modifiesStore;

				modifiesStore.push_back(Statement(stmtNum, tokens.at(i + 1), indent));

				for (int i = 0; i < modifiesStore.size(); i++) {
					Database::insertModifies(modifiesStore.at(i)._stmtNum, procedures.back()->_name, modifiesStore.at(i)._stmt);
				}
			}
			
			parentStack.top()->_statements.push_back(stmt);
			cout << setfill('0') << setw(2) << stmtNum << " | ";
			for (int i = 0; i < indent; i++) { cout << "    "; }
			cout << stmt->_stmt << endl;

		}
	}
	//procedures.at(0)->printContainerTree(0);
	CFG* p1CFG = createCFG(procedures.at(0));
	p1CFG->printCFG();
}


// call linkStatements for each container, then join them up 
CFG* createCFG(Container* container) {
	unordered_map<int, CFG*> hashMapCFG;
	if (!container) { return nullptr; }
	CFG* ownCFG = container->linkStatements(); // returns a CFG* object even if there's no statements.
	CFG* childCFG = nullptr;
	for (int i = 0; i < container->_childContainers.size(); i++) { // loop through current container's child containers
		cout << "ownCFG: ";
		ownCFG->printCFG();
		cout << endl;
		Container* childContainer = container->_childContainers.at(i);
		childCFG = createCFG(childContainer); // DFS till end of tree, and create CFG
		cout << "childCFG: ";
		childCFG->printCFG();
		cout << endl;
		assert(childCFG != nullptr);
		hashMapCFG.insert(pair<int, CFG*> (childCFG->_head->_stmtPtr->_level, childCFG));
		string childContainerType = container->_childContainers.at(i)->_type;
		if (childContainerType == "if") { // if the current child container is "if" container, the next one will be the "else" container
			i++; // go to next childContainer = go to the corresponding "else" container
			CFG* elseCFG = createCFG(container->_childContainers.at(i)); // create the CFG for the else container
			childCFG->_head->_fJump = elseCFG->_head; // set "if" CFG.fJump to "else" CFG.head = "if" CFG fail condition next stmt points to else's stmt
			//childCFG->_fTail = elseCFG->_sTail; // set "if" CFG.fTail to "else" CFG.sTail to maintain the correct CFG fTail after joining the "if" CFG and "else" CFG
			childCFG->_fTail = elseCFG->_sTail;
			cout << "childCFG else: ";
			childCFG->printCFG();
			cout << endl;
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

		// here, we are slotting in the childCFG to ownCFG
		if (prevNode) { // if prevNode is not null, then childCFG is not the first statement. It could be joined to ownCFG in the middle or end
			CFGNode* tempNode = prevNode->_sJump;

			// if prevNode is part of "if" container, set ownCFG sTail and fTail success jump to the childCFG head = add childCFG to ownCFG
				// wrong. this is only if adding childCFG to the end. If add to middle, is prevNode->_sJump to childCFG->_head, and childCFG->_tail to tempNode
				// and if child is while container, the _tail is the _head.  ** here
			// this works as the previous childCFG would've set the correct ownCFG sTail and fTail
			// issue: iteration2_CFG_Nested_Condition_L1_If_While_Parent_Stmt_Start_End_correct.txt
				// line 6 while container is between ownCFG stmts
				// so, ownCFG sTail and fTail points to the while container head, which by right, shouldn't
			// logic works if prevNode is part of another else container. So need indent level?
				// E.g., if prevNode is part of another sibling container
				//			if sibling container is while, need to find the _head
				//			if sibling container is if, need to find the _sTail and _fTail
				//		 if prevNode is part of parent container, no need do anything
			if (prevNode->_container == "if" || prevNode->_container == "else") {
				if (prevNode->_stmtPtr->_level == childCFG->_head->_stmtPtr->_level) { // prevNode is same nested level as childCFG
					CFG* prevNodeCFG = hashMapCFG.at(prevNode->_stmtPtr->_level);
					prevNodeCFG->_sTail->_sJump = childCFG->_head;
					prevNodeCFG->_fTail->_sJump = childCFG->_head;
				}
				else { // prevNode different nested level as childCFG
					prevNode->_sJump = childCFG->_head; 
				}
			}
			else if (prevNode->_container == "while") {
				if (prevNode->_stmtPtr->_level == childCFG->_head->_stmtPtr->_level) { // prevNode is same nested level as childCFG
					CFG* prevNodeCFG = hashMapCFG.at(prevNode->_stmtPtr->_level);
					prevNodeCFG->_head->_fJump = childCFG->_head;
				}
				else { // prevNode is different nested level as childCFG
					prevNode->_sJump = childCFG->_head;
				}
			}
			else {
				prevNode->_sJump = childCFG->_head;
			}

			// if prevNode is part of "while" container, we need to set the while head fail jump to childCFG head = add childCFG to ownCFG
			// the above logic is wrong?
				// if we're joining childCFG at the middle, then it's prevNode as usual
				// if we're joining childCFG at the end, then it's prevNode as usual. Then, the childCFG sTail's and fTail's sJump will be the while head
			// under what case is this logic correct? So ownCFG will be this container's CFG + previous child CFG
				// So, childCFG will always be at the same level as whatever CFG. If same level, then there's no need to touch the while head fJump cos fJump will be for the previous nested level
			// logic error. What if we have a "while with nested if". the while head fjump should not be the if child container head
			// counter: what if two while containers. prevNode will be while1 last node. If while1 last node sJump is while2 head, wrong
			

			// this part sets the childCFG sTail and/or fTail to the correct node in ownCFG. Then, checks if childCFG is the end CFG, and set the ownCFG sTail and/or fTail to the childCFG nodes
			if (childContainerType == "if" || childContainerType == "else") { // if child is "if" container, we need to link both sTail and fTail success jump to the next node
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