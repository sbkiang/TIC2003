#include "CFG.h"

CFG::CFG(CFGNode* head) {
	_head = head;
	_sTail = nullptr;
	_fTail = nullptr;
}

CFG::CFG() {
	_head = nullptr;
	_sTail = nullptr;
	_fTail = nullptr;
}


CFG::~CFG() {}

CFGNode* CFG::getNode(int stmtNum) {
	if (_head == nullptr) { return nullptr; }
	set<int> visited;
	return _getNode(stmtNum, _head, visited);
}

CFGNode* CFG::_getNode(int stmtNum, CFGNode* node, set<int> visited) {
	if (!node) { return nullptr; }
	int nodeStmtNum = node->_stmtPtr->_stmtNum;
	if (visited.find(nodeStmtNum) != visited.end()) { // if node is already visited, skip. This is to avoid the While CFG cycle
		return nullptr;
	}
	CFGNode* found = nullptr;
	if (nodeStmtNum == stmtNum) {
		return node;
	}
	visited.insert(nodeStmtNum);
	found = _getNode(stmtNum, node->_sJump, visited); // traverse the _sJump to find the node
	if (!found) {
		found = _getNode(stmtNum, node->_fJump, visited); // traverse the _fJump to find the node if can't find it in _sJump
	}
	return found;
}

void CFG::addFTailSJump(CFGNode* node) { 
	if (!_fTail) { _head->_sJump = node; } // we want to maintain _fTail as NULL if there's no statement for it instead of setting _head as the tail
	else { _fTail->_sJump = node; } // if _fTail is not NULL, add the new CFG node to it
	_fTail = node;
}

void CFG::addFTailFJump(CFGNode* node) { 
	if (!_fTail) { _head->_fJump = node; } // we want to maintain _fTail as NULL if there's no statement for it
	else { _fTail->_fJump = node; }
	_fTail = node;
}

void CFG::addSTailSJump(CFGNode* node) { 
	if (!_sTail) { _head->_sJump = node; } // we want to maintain _sTail as NULL if there's no statement for it
	else { _sTail->_sJump = node; } // if _sTail is not NULL, add the new CFG node to it
	_sTail = node;
}

void CFG::addSTailFJump(CFGNode* node) {  
	if (!_sTail) { _head->_fJump = node; } // we want to maintain _sTail as NULL if there's no statement for it
	else { _sTail->_fJump = node; } // if _sTail is not NULL, add the new CFG node to it
	_sTail = node;
}

CFGNode* CFG::getSTail() {
	set<int> visited;
	return _getSTail(_head, visited);
}

CFGNode* CFG::_getSTail(CFGNode* node, set<int> visited) {
	if (!(node->_sJump)) { return node; } // if next node is null, this node is last node. return
	if (visited.find(node->_sJump->_stmtPtr->_stmtNum) != visited.end()) { return node; } // if next node is a visited node, then this node is also last node. For while CFG only
	visited.insert(node->_stmtPtr->_stmtNum);
	return _getSTail(node->_sJump, visited);
}

CFGNode* CFG::getFTail() {
	set<int> visited;
	return _getFTail(_head, visited);
}

CFGNode* CFG::_getFTail(CFGNode* node, set<int> visited) {
	if (!(node->_fJump)) { return node; }
	if (visited.find(node->_fJump->_stmtPtr->_stmtNum) != visited.end()) { return node; } // if next node is a visited node, then this node is also last node. For while CFG only
	visited.insert(node->_stmtPtr->_stmtNum);
	return _getFTail(node->_sJump, visited);
}

void CFG::printCFG() {
	set<int> visited;
	_printCFG(_head, visited, 0); 
}

void CFG::_printCFG(CFGNode* node, set<int> visited, int spaces) {
	if (!node) {
		cout << endl;
		return;
	} // if node is null, return as we're at the end
	if(visited.find(node->_stmtPtr->_stmtNum) != visited.end()){ // if node is already visited, skip. This is to avoid the While CFG cycle
		cout << setfill('0') << setw(2) << node->_stmtPtr->_stmtNum << endl;
		return;
	}
	visited.insert(node->_stmtPtr->_stmtNum);
	cout << setfill('0') << setw(2) << node->_stmtPtr->_stmtNum << "->";
	spaces += to_string(node->_stmtPtr->_stmtNum).length() + 2;
	_printCFG(node->_sJump, visited, spaces);
	if (node->_fJump) {
		cout << setfill(' ') << setw(spaces) << " ";
		cout << setfill('0') << setw(2) << node->_stmtPtr->_stmtNum << "->";
		_printCFG(node->_fJump, visited, spaces);
	}
}

