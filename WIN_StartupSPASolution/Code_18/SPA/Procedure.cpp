#include "Procedure.h"

// constructor
Procedure::Procedure(string name) {
	_name = name;
}

// destructor
Procedure::~Procedure() {}

void Procedure::_GetAllUses(Procedure* procedure, vector<string>& variable) {
	if (procedure == nullptr) { return; }
	copy(procedure->_uses.begin(), procedure->_uses.end(), back_inserter(variable));
	for (int i = 0; i < procedure->_calls.size(); i++) {
		_GetAllUses(procedure->_calls.at(i), variable);
	}
}

void Procedure::_GetAllModifies(Procedure* procedure, vector<string>& variable) {
	if (procedure == nullptr) { return; }
	copy(procedure->_modifies.begin(), procedure->_modifies.end(), back_inserter(variable));
	for (int i = 0; i < procedure->_calls.size(); i++) {
		_GetAllModifies(procedure->_calls.at(i), variable);
	}
}

vector<string> Procedure::GetAllUses(){
	vector<string> variable;
	variable = _uses;
	for (int i = 0; i < _calls.size(); i++) {
		_GetAllUses(_calls.at(i), variable);
	}
	return variable;
}

vector<string> Procedure::GetAllModifies() {
	vector<string> variable;
	variable = _modifies;
	for (int i = 0; i < _calls.size(); i++) {
		_GetAllModifies(_calls.at(i), variable);
	}
	return variable;
}


vector<Procedure*> Procedure::GetAllCalls() {
	vector<Procedure*> callProc;
	for (int i = 0; i < _calls.size(); i++) {
		_GetAllCalls(_calls.at(i), callProc);
	}
	return callProc;
}

void Procedure::_GetAllCalls(Procedure* proc, vector<Procedure*>& callProc) {
	if (proc == nullptr) { return; }
	for (int i = 0; i < proc->_calls.size(); i++) {
		callProc.push_back(proc->_calls.at(i));
		_GetAllCalls(proc->_calls.at(i), callProc);
	}

}