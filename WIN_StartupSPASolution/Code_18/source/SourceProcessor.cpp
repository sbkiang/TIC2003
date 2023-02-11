#include "SourceProcessor.h"

// method for processing the source program
// This method currently only inserts the procedure name into the database
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all the required syntax.

//CFG* createCFG(Container* container);
map<int, CFGNode*> _buildStatements(Container* container);
CFG* buildStatements(Container* container);
CFGNode* findNextStmt(stack<Container*> parentStack, int startStmtNum, map<int, CFGNode*> stmts);

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

	vector<Procedure*> procedures;
	stack<Container*> parentStack;
	stack<IfElseLinker*> ifElseLinkerStack;
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
			Statement* stmt = new Statement(stmtNum, nestedLevel, true, container);
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
			cout << setfill('0') << setw(2) << stmtNum << " | ";
			for (int i = 0; i < nestedLevel - 1; i++) { cout << "    "; }
			cout << word << stmt->_stmt << endl;
		}
		else if (word == "if") { // if(...) then {...} else {...}
			stmtNum++;
			nestedLevel++;
			Container* container = new Container();
			container->_type = "if";
			container->_startStmtNum = stmtNum;
			container->_level = nestedLevel;
			IfElseLinker* linker = new IfElseLinker(); // create a IfElseLinker
			linker->_ifPtr = container; // set the IfElseLinker's ifPtr to this "if" container
			ifElseLinkerStack.push(linker); // push it onto the stack for the corresponding "else" container
			Statement* stmt = new Statement(stmtNum, nestedLevel, true, container);
			if (!parentStack.empty()) { // if there's parent container, add current container to parent's child
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container); //set itself as the latest parent container
			vector<Statement> variableStore; // we need to insert statement first before inserting variable due to FK
			i++; // skip the "if" keyword for the while loop below
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
			cout << setfill('0') << setw(2) << stmtNum << " | ";
			for (int i = 0; i < nestedLevel - 1; i++) { cout << "    "; }
			cout << word << stmt->_stmt << endl;
		}
		else if (word == "else") { // for else container
			nestedLevel++;
			Container* container = new Container();
			container->_type = "else";
			/*
			Statement* stmt = new Statement(stmtNum + 1, nestedLevel + 1, true, container);
			stmt->_stmt = "else";
			container->_statements.push_back(stmt);
			*/
			container->_startStmtNum = stmtNum + 1;
			container->_level = nestedLevel;
			ifElseLinkerStack.top()->_elsePtr = container; // the top stack will be the corresponding "if" container. We set this "else" container to the pointer
			parentStack.top()->_ifElseLinker.push_back(ifElseLinkerStack.top()); // parentStack.top() is this if-else container's parent. We add the this linker to it
			ifElseLinkerStack.pop(); // we pop the IfElseLinker as we've already processed the if-else
			if (!parentStack.empty()) { // if there's parent container, add current container to parent's child
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container); // we push the current "else" container to the parentStack for future statements
			cout << setfill('0') << setw(2) << stmtNum + 1 << " | ";
			for (int i = 0; i < nestedLevel - 1; i++) { cout << "    "; }
			cout << word << " " << endl;
		}
		else if (word == "=") { // for assign
			stmtNum++;
			Statement* stmt = new Statement(stmtNum, nestedLevel, false, parentStack.top());
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
					variableStore.push_back(Statement(stmtNum, tokens.at(i), nestedLevel, parentStack.top()));
				}
				i++;
			}
			parentStack.top()->_statements.push_back(stmt);
			Database::insertStatement(stmtNum, procedures.back()->_name, "assign", stmt->_stmt);
			for (int i = 0; i < variableStore.size(); i++) {
				Database::insertVariable(variableStore.at(i)._stmt, variableStore.at(i)._stmtNum);
			}
			cout << setfill('0') << setw(2) << stmtNum << " | ";
			for (int i = 0; i < nestedLevel; i++) { cout << "    "; }
			cout << stmt->_stmt << endl;
		}
		else if (word == "read" || word == "print" || word == "call") {
			stmtNum++;
			Statement* stmt = new Statement(stmtNum, nestedLevel, false, parentStack.top());
			stmt->_stmt += tokens.at(i);
			stmt->_stmt += tokens.at(i + 1);
			Database::insertStatement(stmtNum, procedures.back()->_name, word, stmt->_stmt);
			if (word == "read" || word == "print") {
				Database::insertVariable(tokens.at(i + 1), stmtNum);
			}
			parentStack.top()->_statements.push_back(stmt);
			cout << setfill('0') << setw(2) << stmtNum << " | ";
			for (int i = 0; i < nestedLevel; i++) { cout << "    "; }
			cout << stmt->_stmt << endl;
		}
	}
	procedures.at(0)->printContainerTree(0);
	CFG* cfg = buildStatements(procedures.at(0));
	cfg->printCFG();
}


CFG* buildStatements(Container* container) {
	stack<CFGNode*> whileHeads;
	stack<CFGNode*> ifHeads;
	stack<Container*> parentStack;
	stack<Container*> tempParentStack;
	Container* tempContainer = container;
	map<int, CFGNode*> stmts = _buildStatements(container);
	CFG* cfg = new CFG(stmts.at(1));
	int loopStart = 0, loopEnd = 0;
	for (int i = 1; i < stmts.size() + 1; i++) {
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
				/*
				if (parentStack.top()->_type == "while") { // if parent container is while
					stack<Container*> tempStack;
					tempStack.push(parentStack.top());
					node->_fJump = findNextStmt(tempStack, node->_stmtPtr->_container->_endStmtNum + 1, stmts); // find the next sibling or parent stmt in this while parent container
					if (!node->_fJump) { // if can't find, then there's no more further in this while parent container. sJump will go back to while head
						node->_fJump = stmts.at(parentStack.top()->_startStmtNum);
					}
				}
				else { // for all other parent containers, find sibling stmt or parent stmt, and skip all else stmt
					node->_fJump = findNextStmt(parentStack, node->_stmtPtr->_container->_endStmtNum + 1, stmts);
				}*/
				node->_fJump = findNextStmt(parentStack, node->_stmtPtr->_container->_endStmtNum + 1, stmts);
			} 
		}
		else if (node->_stmtPtr->_container->_type == "if") {
			if (!node->_stmtPtr->_containerHead && node->_stmtPtr->_containerTail) { // if stmt is if container tail only
				/*
				if (parentStack.top()->_type == "while") { // if parent container is while
					stack<Container*> tempStack;
					tempStack.push(parentStack.top());
					node->_sJump = findNextStmt(tempStack, node->_stmtPtr->_container->_endStmtNum + 1, stmts); // find the next sibling or parent stmt in this while parent container
					if (!node->_sJump) { // if can't find, then there's no more further in this while parent container. sJump will go back to while head
						node->_sJump = stmts.at(parentStack.top()->_startStmtNum);
					}
				}
				else { // for all other parent containers, find sibling stmt or parent stmt, and skip all else stmt
					node->_sJump = findNextStmt(parentStack, node->_stmtPtr->_stmtNum + 1, stmts);
				}
				*/
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
				/*
				if (parentStack.top()->_type == "while") { // if parent container is while
					stack<Container*> tempStack;
					tempStack.push(parentStack.top());
					node->_sJump = findNextStmt(tempStack, node->_stmtPtr->_container->_endStmtNum + 1, stmts); // find the next sibling or parent stmt in this while parent container
					if (!node->_sJump) { // if can't find, then there's no more further in this while parent container. sJump will go back to while head
						node->_sJump = stmts.at(parentStack.top()->_startStmtNum);
					}
				}
				else { // for all other parent containers, find sibling stmt or parent stmt, and skip all else stmt
					node->_sJump = findNextStmt(parentStack, node->_stmtPtr->_stmtNum + 1, stmts);
				}
				*/
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
		if(node->_sJump){ cout << "node " << node->_stmtPtr->_stmtNum << " sJump : " << node->_sJump->_stmtPtr->_stmtNum << endl; }
		if (node->_fJump) { cout << "node " << node->_stmtPtr->_stmtNum << " fJump : " << node->_fJump->_stmtPtr->_stmtNum << endl; }
	}
	return cfg;	
}

CFGNode* findNextStmt(stack<Container*> parentStack, int startStmtNum, map<int, CFGNode*> stmts) {
	Container* currContainer = nullptr;
	CFGNode* nextStmt = nullptr;
	if (parentStack.empty()) { return nullptr; } // ** ISSUE HERE **. 
	int endStmtNum = parentStack.top()->_endStmtNum;
	for (int j = startStmtNum; j < endStmtNum + 1; j++) { // frind the next stmt that is a sibling or parent stmt
		if (stmts.at(j)->_stmtPtr->_container->_type == "else") {
			continue;
		}
		if (stmts.at(startStmtNum)->_stmtPtr->_level >= stmts.at(j)->_stmtPtr->_level) { // siblings container = same nestedLevel level. parent container = lower nestedLevel level. So, >=
			nextStmt = stmts.at(j);
			break;
		}
	}
	if (!nextStmt) { // if can't find
		if (parentStack.top()->_type == "while") { // if parent container is a while, then next stmt will be the while parent head
			return stmts.at(parentStack.top()->_startStmtNum);
		}
		else { // if not, we continue to the higher parent
			currContainer = parentStack.top();
			parentStack.pop();
			nextStmt = findNextStmt(parentStack, currContainer->_endStmtNum, stmts);
			parentStack.push(currContainer);
		}
	}
	return nextStmt;
}

/*
CFGNode* findNextStmt(stack<Container*> parentStack, int startStmtNum, map<int, CFGNode*> stmts) {
	Container* currContainer = nullptr;
	CFGNode* nextStmt = nullptr;
	if (parentStack.empty()) { return nullptr; }
	if (parentStack.top()->_type == "while") { // if parent container is while, then fJump will be parent container head
		// find the next stmt in while. If can't find, then sJump to while head
		return stmts.at(parentStack.top()->_startStmtNum);
	}
	else { // for all other parent containers, find sibling stmt or parent stmt, and skip all else stmt
		int endStmtNum = parentStack.top()->_endStmtNum; // for the first iter, parentStack will be itself. But loop wont run
		for (int j = startStmtNum; j < endStmtNum + 1; j++) {
			if (stmts.at(j)->_stmtPtr->_container->_type == "else") {
				continue;
			}
			if (stmts.at(startStmtNum)->_stmtPtr->_level >= stmts.at(j)->_stmtPtr->_level) { // siblings container = same nestedLevel level. parent container = lower nestedLevel level. So, >=
				nextStmt = stmts.at(j);
				break;
			}
		}
	}
	if (!nextStmt) { // if from current container end stmt to parent end stmt, unable to find sibling container or parent stmt, go one level higher
		currContainer = parentStack.top();
		parentStack.pop();
		nextStmt = findNextStmt(parentStack, currContainer->_endStmtNum, stmts);
		parentStack.push(currContainer);
	}
	return nextStmt;
}
*/
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
	//myMap.at(container->_statements.at(container->_statements.size()-1)->
	for (int i = 0; i < container->_childContainers.size(); i++) {
		Container* childContainer = container->_childContainers.at(i);
		map<int, CFGNode*> childMap = _buildStatements(childContainer);
		myMap.insert(childMap.begin(), childMap.end());
	}
	return myMap;
}


// call linkStatements for each container, then join them up 
/*
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
			// logic works if prevNode is part of another else container. So need nestedLevel level?
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
*/