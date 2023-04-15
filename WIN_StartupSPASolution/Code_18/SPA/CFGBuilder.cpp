#include "CFGBuilder.h"

CFG CFGBuilder::BuildCFG(Container* procedure) {
	stack<Container*> parentStack;
	Container* tempContainer = procedure;
	map<int, CFGNode*> stmts;
	_CreateStmtMap(procedure, stmts);
	//_printStmt(stmts);
	int startIndex = stmts.begin()->first;
	//CFG* cfg = new CFG(stmts.at(startIndex));
	CFG cfg = CFG(stmts.at(startIndex));
	int loopStart = 0, loopEnd = 0;
	for (int i = startIndex; i < stmts.size() + startIndex; i++) {
		CFGNode* node = stmts.at(i);

		// if current stmt is the container head, save to a tempContainer as it's the start of a new container
		if (i == node->_stmtPtr->_container->_startStmtNum) {  

			// if tempContainer nestedLevel < current node container level, then tempContainer is a parent container of current container
			// we push the tempContainer to the stack
			if (tempContainer->_level < node->_stmtPtr->_container->_level) {
				parentStack.push(tempContainer);
				tempContainer = node->_stmtPtr->_container;
			}

			// if tempContainer level = current node container level, then both containers are siblings
			// set the tempContainer to the current container
			else if(tempContainer->_level == node->_stmtPtr->_container->_level){
				tempContainer = node->_stmtPtr->_container;
			}
		}

		// if node is body, just set sJump to next statement
		if (!(node->_stmtPtr->_containerHead || node->_stmtPtr->_containerTail)) {
			node->_sJump = stmts.at(i + 1);
			//cout << "node " << node->_stmtPtr->GetStmtNum() << " sJump : " << node->_sJump->_stmtPtr->GetStmtNum() << endl;
			continue;
		}
		if (node->_stmtPtr->_container->_type == "procedure") {
			if (stmts.find(i + 1) != stmts.end()) {
				node->_sJump = stmts.at(i + 1);
			}
		}
		else if (node->_stmtPtr->_container->_type == "while") {

			// if stmt is while container tail only, sJump back to head
			if (!node->_stmtPtr->_containerHead && node->_stmtPtr->_containerTail) { 
				node->_sJump = stmts.at(node->_stmtPtr->_container->_startStmtNum);
			}

			// if stmt is while container head and tail, or only head, set sJump to next, and fJump to _FindNextStmt
			else { 
				node->_sJump = stmts.at(i + 1);
				node->_fJump = _FindNextStmt(parentStack, node->_stmtPtr->_container->_endStmtNum + 1, stmts);
			}
		}
		else if (node->_stmtPtr->_container->_type == "if") {

			// if stmt is "if" container tail only
			if (!node->_stmtPtr->_containerHead && node->_stmtPtr->_containerTail) { 
				node->_sJump = _FindNextStmt(parentStack, node->_stmtPtr->GetStmtNum() + 1, stmts);
			}

			// if stmt is both "if" container head and tail, or head only, sJump = next stmt, fJump = else stmt
			else { 
				node->_sJump = stmts.at(i + 1);
				loopStart = node->_stmtPtr->_container->_endStmtNum + 1;
				loopEnd = parentStack.top()->_endStmtNum;

				// fJump is the first stmt in else container of same nestedLevel
				for (int j = loopStart; j < loopEnd; j++) { 
					if (stmts.at(j)->_stmtPtr->GetLevel() != node->_stmtPtr->GetLevel()) {
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

			// if stmt is else container head only, sJump is next stmt
			if (node->_stmtPtr->_containerHead && !node->_stmtPtr->_containerTail) { 
				node->_sJump = stmts.at(i + 1);
			}

			// if stmt is else container tail only, or head and tail, need to find sJump
			else { 
				node->_sJump = _FindNextStmt(parentStack, node->_stmtPtr->GetStmtNum() + 1, stmts);
			}
		}
		while (!parentStack.empty()) {

			// if current stmt is a container end, pop from parentStack. Place it at the end so that the pop occurs after everything have been processed
			if (i == parentStack.top()->_endStmtNum) { 
				parentStack.pop();
			}
			else {
				break;
			}
		}
		//if(node->_sJump){ cout << "node " << node->_stmtPtr->GetStmtNum() << " sJump : " << node->_sJump->_stmtPtr->GetStmtNum() << endl; }
		//if(node->_fJump) { cout << "node " << node->_stmtPtr->GetStmtNum() << " fJump : " << node->_fJump->_stmtPtr->GetStmtNum() << endl; }
	}
	return cfg;
}

void CFGBuilder::_CreateStmtMap(Container* container, map<int, CFGNode*> &stmtMap) {
	for (int i = 0; i < container->_statements.size(); i++) {
		Statement* stmt = container->_statements.at(i);
		if (stmt->GetStmtNum() == (container->_startStmtNum)) {
			stmt->_containerHead = true;
		}
		if (stmt->GetStmtNum() == (container->_endStmtNum)) {
			stmt->_containerTail = true;
		}
		CFGNode* node = new CFGNode();
		node->_stmtPtr = stmt;
		pair<int, CFGNode*> *mypair = new pair<int, CFGNode*>(stmt->GetStmtNum(), node);
		stmtMap.insert(*mypair);
	}
	for (int i = 0; i < container->_childContainers.size(); i++) {
		Container* childContainer = container->_childContainers.at(i);
		_CreateStmtMap(childContainer, stmtMap);
	}
}

// this function finds the next statement from startStmtNum to endStmtNum. StartStmtNum is normally the end of the current block, endStmtNum is normally the endStmtNum of parent block
//	1) if encounter else stmt, skip the block
//	2) if can't find the stmt,
//		2a) if parent container is while, loop back
//		2b) if parent container is not while, search from current block endStmtNum + 1 to parentBlock endStmtNum
CFGNode* CFGBuilder::_FindNextStmt(stack<Container*> parentStack, int startStmtNum, map<int, CFGNode*> stmts) {
	Container* currContainer = nullptr;
	CFGNode* nextStmt = nullptr;
	if (parentStack.empty()) { return nullptr; }
	int endStmtNum = parentStack.top()->_endStmtNum; // ** run createCFG 5 times to get to the 6th test

	// find the next stmt that is a sibling or parent stmt
	for (int j = startStmtNum; j < endStmtNum + 1; j++) { 

		// if encounter else stmt, it's going to be the else head. Skip the entire block
		if (stmts.at(j)->_stmtPtr->_container->_type == "else") { 
			if (j == stmts.at(j)->_stmtPtr->_container->_startStmtNum) {
				j = stmts.at(j)->_stmtPtr->_container->_endStmtNum;
				continue;
			}
		}

		// siblings container = same nestedLevel level. parent container = lower nestedLevel level. So, >=
		if (stmts.at(startStmtNum)->_stmtPtr->GetLevel() >= stmts.at(j)->_stmtPtr->GetLevel()) { 
			nextStmt = stmts.at(j);
			break;
		}
	}

	// if can't find next stmt
	if (!nextStmt) {

		// if parent container is a while, then next stmt will be the while parent head
		if (parentStack.top()->_type == "while") { 
			return stmts.at(parentStack.top()->_startStmtNum);
		}

		// if parent container is not while, we pop the stack to find the higher parent. Need to push back after that
		else { 
			currContainer = parentStack.top();
			parentStack.pop();
			nextStmt = _FindNextStmt(parentStack, currContainer->_endStmtNum + 1, stmts);
			parentStack.push(currContainer);
		}
	}
	return nextStmt;
}

void CFGBuilder::_PrintStmt(map<int, CFGNode*> stmts) {
	cout << "procedure ";
	//cout << "   | H | T | " << endl;
	int startIndex = stmts.begin()->first;
	for (map<int, CFGNode*>::iterator it = stmts.begin(); it != stmts.end(); it++) {
		CFGNode* node = it->second;
		//cout << setfill('0') << setw(2) << node->_stmtPtr->GetStmtNum() << " |";
		if (node->_stmtPtr->_containerHead) {
			//cout << " Y |";
		}
		else {
			//cout << "   |";
		}
		if (node->_stmtPtr->_containerTail) {
			//cout << " Y | ";
		}
		else {
			//cout << "   | ";
		}
		for (int i = 0; i < node->_stmtPtr->GetLevel(); i++) { cout << "  "; }
		cout << node->_stmtPtr->GetEntity() << " " << node->_stmtPtr->GetStmt() << endl;
	}
}