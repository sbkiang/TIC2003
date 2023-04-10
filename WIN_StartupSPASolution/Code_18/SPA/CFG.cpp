#include "CFG.h"


CFG::CFG(CFGNode* head) {
	_head = head;
}

CFG::CFG() {
	_head = nullptr;
}

CFG::~CFG() {}

CFGNode* CFG::GetNode(int stmtNum) {
	if (_head == nullptr) { return nullptr; }
	set<int> visited;
	return _GetNode(stmtNum, _head, visited);
}

CFGNode* CFG::_GetNode(int stmtNum, CFGNode* node, set<int> visited) {
	if (!node) { return nullptr; }
	int nodeStmtNum = node->_stmtPtr->GetStmtNum();
	if (visited.find(nodeStmtNum) != visited.end()) { // if node is already visited, skip. This is to avoid the While CFG cycle
		return nullptr;
	}
	CFGNode* found = nullptr;
	if (nodeStmtNum == stmtNum) {
		return node;
	}
	visited.insert(nodeStmtNum);
	found = _GetNode(stmtNum, node->_sJump, visited); // traverse the _sJump to find the node
	if (!found) {
		found = _GetNode(stmtNum, node->_fJump, visited); // traverse the _fJump to find the node if can't find it in _sJump
	}
	return found;
}

vector<CFGNode*> CFG::GetAllCFGNodes() {
	set<int> visited;
	vector<CFGNode*> nodes;
	_GetAllCFGNodes(_head, visited, 0, nodes);
	return nodes;
}

void CFG::_GetAllCFGNodes(CFGNode* node, set<int> visited, int spaces, vector<CFGNode*> &nodes) {
	if (!node) {
		cout << endl;
		return;
	} // if node is null, return as we're at the end
	if(visited.find(node->_stmtPtr->GetStmtNum()) != visited.end()){ // if node is already visited, skip. This is to avoid the While CFG cycle
		cout << setfill('0') << setw(2) << node->_stmtPtr->GetStmtNum() << "(" << node->_stmtPtr->GetAdjustedStmtNum() << ")" << "->";
		return;
	}
	visited.insert(node->_stmtPtr->GetStmtNum());
	cout << setfill('0') << setw(2) << node->_stmtPtr->GetStmtNum() << "(" << node->_stmtPtr->GetAdjustedStmtNum() << ")" << "->";
	spaces += to_string(node->_stmtPtr->GetStmtNum()).length() + 4;
	if (find(nodes.begin(), nodes.end(), node) == nodes.end()) { // if node not in nodes
		nodes.push_back(node);
	}
	_GetAllCFGNodes(node->_sJump, visited, spaces, nodes);
	if (node->_fJump) {
		cout << endl;
		cout << setfill(' ') << setw(spaces) << " ";
		cout << setfill('0') << setw(2) << node->_stmtPtr->GetStmtNum() << "(" << node->_stmtPtr->GetAdjustedStmtNum() << ")" << "->";
		_GetAllCFGNodes(node->_fJump, visited, spaces, nodes);
	}
}