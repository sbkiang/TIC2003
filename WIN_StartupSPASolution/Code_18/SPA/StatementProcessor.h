#pragma once
#include <regex>
#include <string>
#include <vector>
#include "Statement.h"
#include "DescriberEntStmt.h"
#include "define.h"

using namespace std;

class StatementProcessor {
private:
	vector<DescriberEntStmt> ProcessWhile(Statement stmt);
	vector<DescriberEntStmt> ProcessIf(Statement stmt);
	vector<DescriberEntStmt> ProcessElse(Statement stmt);
	vector<DescriberEntStmt> ProcessAssign(Statement stmt);
	vector<DescriberEntStmt> ProcessCall(Statement stmt);
	vector<DescriberEntStmt> ProcessPrint(Statement stmt);
	vector<DescriberEntStmt> ProcessRead(Statement stmt);
public:

};