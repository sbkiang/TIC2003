#include "Procedure.h"

// constructor
Procedure::Procedure(string name) {
	_name = name;
}

// destructor
Procedure::~Procedure() {}

void Procedure::print() {
	cout << "Procedure Name: " << _name << endl;
	cout << "Statements " << endl;
	for (int i = 0; i < _statements.size(); i++) {
		cout << _statements.at(i)->_stmtNum << "\t|" << _statements.at(i)->_stmt << endl;
	}
}

void Procedure::_print() {}