#include "CFGBuilder.h"

CFG* CFGBuilder::buildCFG(Container* procedure) {
	stack<Container*> parentStack;
	Container* tempContainer = procedure;
	map<int, CFGNode*> stmts;
	_createStmtMap(procedure, stmts);
	//_printStmt(stmts);
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
		if (!(node->_stmtPtr->_containerHead || node->_stmtPtr->_containerTail)) { // if node is body, just set sJump to next statement
			node->_sJump = stmts.at(i + 1);
			cout << "node " << node->_stmtPtr->getStmtNum() << " sJump : " << node->_sJump->_stmtPtr->getStmtNum() << endl;
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
				node->_fJump = _findNextStmt(parentStack, node->_stmtPtr->_container->_endStmtNum + 1, stmts);
			}
		}
		else if (node->_stmtPtr->_container->_type == "if") {
			if (!node->_stmtPtr->_containerHead && node->_stmtPtr->_containerTail) { // if stmt is if container tail only
				node->_sJump = _findNextStmt(parentStack, node->_stmtPtr->getStmtNum() + 1, stmts);
			}
			else { // if stmt is both if container head and tail, or head only, sJump = next stmt, fJump = else stmt
				node->_sJump = stmts.at(i + 1);
				loopStart = node->_stmtPtr->_container->_endStmtNum + 1;
				loopEnd = parentStack.top()->_endStmtNum;
				for (int j = loopStart; j < loopEnd; j++) { // fJump is the first stmt in else container of same nestedLevel
					if (stmts.at(j)->_stmtPtr->getLevel() != node->_stmtPtr->getLevel()) {
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
			else { // if stmt is else container tail only, or head and tail

				node->_sJump = _findNextStmt(parentStack, node->_stmtPtr->getStmtNum() + 1, stmts);
			}
		}
		while (!parentStack.empty()) {
			if (i == parentStack.top()->_endStmtNum) { // if current stmt is a container end, pop from parentStack. Place it at the end so that the pop occurs after everything have been processed
				parentStack.pop();
			}
			else {
				break;
			}

		}
		//if(node->_sJump){ cout << "node " << node->_stmtPtr->getStmtNum() << " sJump : " << node->_sJump->_stmtPtr->getStmtNum() << endl; }
		//if (node->_fJump) { cout << "node " << node->_stmtPtr->getStmtNum() << " fJump : " << node->_fJump->_stmtPtr->getStmtNum() << endl; }
	}
	return cfg;
}

void CFGBuilder::_createStmtMap(Container* container, map<int, CFGNode*> &stmtMap) {
	for (int i = 0; i < container->_statements.size(); i++) {
		Statement* stmt = container->_statements.at(i);
		if (stmt->getStmtNum() == (container->_startStmtNum)) {
			stmt->_containerHead = true;
		}
		if (stmt->getStmtNum() == (container->_endStmtNum)) {
			stmt->_containerTail = true;
		}
		CFGNode* node = new CFGNode();
		node->_stmtPtr = stmt;
		pair<int, CFGNode*> *mypair = new pair<int, CFGNode*>(stmt->getStmtNum(), node);
		stmtMap.insert(*mypair);
	}
	for (int i = 0; i < container->_childContainers.size(); i++) {
		Container* childContainer = container->_childContainers.at(i);
		_createStmtMap(childContainer, stmtMap);
	}
}

// this function finds the next statement from startStmtNum to endStmtNum. StartStmtNum is normally the end of the current block, endStmtNum is normally the endStmtNum of parent block
//	1) if encounter else stmt, skip the block
//	2) if can't find the stmt,
//		2a) if parent container is while, loop back
//		2b) if parent container is not while, search from current block endStmtNum + 1 to parentBlock endStmtNum
CFGNode* CFGBuilder::_findNextStmt(stack<Container*> parentStack, int startStmtNum, map<int, CFGNode*> stmts) {
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
		if (stmts.at(startStmtNum)->_stmtPtr->getLevel() >= stmts.at(j)->_stmtPtr->getLevel()) { // siblings container = same nestedLevel level. parent container = lower nestedLevel level. So, >=
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
			nextStmt = _findNextStmt(parentStack, currContainer->_endStmtNum + 1, stmts);
			parentStack.push(currContainer);
		}
	}
	return nextStmt;
}

void CFGBuilder::_printStmt(map<int, CFGNode*> stmts) {
	cout << "   | H | T | " << endl;
	int startIndex = stmts.begin()->first;
	for (map<int, CFGNode*>::iterator it = stmts.begin(); it != stmts.end(); it++) {
		CFGNode* node = it->second;
		cout << setfill('0') << setw(2) << node->_stmtPtr->getStmtNum() << " |";
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
		for (int i = 0; i < node->_stmtPtr->getLevel(); i++) { cout << "  "; }
		if (node->_stmtPtr->_containerHead) {
			cout << node->_stmtPtr->_container->_type << " ";
		}
		cout << node->_stmtPtr->getStmt() << endl;
	}
}