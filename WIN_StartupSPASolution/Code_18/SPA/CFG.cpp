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
	return _getNode(stmtNum, _head);
}

CFGNode* CFG::_getNode(int stmtNum, CFGNode* node) {
	if (!node) { return nullptr; }
	CFGNode* found = nullptr;
	if (node->_stmtPtr->_stmtNum == stmtNum) {
		return node;
	}
	found = _getNode(stmtNum, node->_sJump); // traverse the _sJump to find the node
	if (!found) {
		found = _getNode(stmtNum, node->_fJump); // traverse the _fJump to find the node if can't find it in _sJump
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

void CFG::printCFG() {
	vector<CFGNode*> visited;
	if (_head) { 
		_printCFG(_head, visited, 0); 
	}
}

void CFG::_printCFG(CFGNode* node, vector<CFGNode*> visited, int spaces) {
	if (!node) {
		cout << endl;
		return;
	} // if node is null, return as we're at the end
	if (find(visited.begin(), visited.end(), node) != visited.end()) { // if node is already visited, skip. This is to avoid the While CFG cycle
		cout << node->_stmtPtr->_stmtNum << endl;
		return;
	}
	visited.push_back(node);
	cout << node->_stmtPtr->_stmtNum << "->";
	spaces += to_string(node->_stmtPtr->_stmtNum).length() + 2;
	_printCFG(node->_sJump, visited, spaces);
	if (node->_fJump) {
		cout << string(" ", spaces);
		cout << node->_stmtPtr->_stmtNum << "->";
		_printCFG(node->_fJump, visited, spaces);
	}
}

