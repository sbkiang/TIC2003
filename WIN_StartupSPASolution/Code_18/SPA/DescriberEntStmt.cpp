#include "DescriberEntStmt.h"

void DescriberEntStmt::ExtractConstant(vector<string> token)
{
	for (string word : token) {
		if (regex_match(word, regex(regexConstants))) {
			_entDataMap["constant"].push_back(word);
		}
	}
}

void DescriberEntStmt::ExtractVariable(vector<string> token)
{
	for (string word : token) {
		if (regex_match(word, regex(regexVariables))) {
			_entDataMap["variable"].push_back(word);
		}
	}
}

void DescriberEntStmt::ExtractPattern(string text)
{
	int equal_pos = text.find("="); // Find position of the equal sign
	string LHS = text.substr(0, equal_pos);
	string RHS = text.substr(equal_pos + 1); //RHS expression
	string postFix = HelperFunction::InfixToPostfix(RHS);
	_entDataMap["pattern"].push_back(LHS);
	_entDataMap["pattern"].push_back(RHS);
	_entDataMap["pattern"].push_back(postFix);
}

DescriberEntStmt::DescriberEntStmt(Statement stmt)
{
	ExtractConstant(stmt.GetToken());
	ExtractVariable(stmt.GetToken());
	if (stmt.GetEntity() == "assign") {
		ExtractPattern(stmt.GetStmt());
	}
}

vector<string> DescriberEntStmt::GetEntData(string ent)
{
	if (_entDataMap.find(ent) != _entDataMap.end()) {
		return _entDataMap.at(ent);
	}
	return vector<string>();
}