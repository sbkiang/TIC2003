#include "StatementProcessor.h"

vector<DescriberEntStmt> StatementProcessor::ProcessWhile(Statement stmt)
{
//		vector<DescriberEntStmt> describerVar;
//		vector<DescriberEntStmt> describerUse;
//		string text = stmt.GetStmt();
//		for(string token : stmt.GetToken()){
//			if (regex_match(token, regex(regexConstants))) {
//				DescriberEntStmt de = DescriberEntStmt()
//				Database::insertConstant(tokens.at(i));
//			}
//			else if (regex_match(tokens.at(i), regex(regexVariables))) {
//				variableStore.push_back(Statement(stmtNum, tokens.at(i), container, stmtNumSubtract));
//			}
//			if (regex_match(tokens.at(i), regex(regexVariables))) {
//				useStore.push_back(Statement(stmtNum, tokens.at(i), stmtNumSubtract));
//			}
//			i++;
//		}
//		container->_statements.push_back(stmt);
//		Database::insertStatement(stmt->GetAdjustedStmtNum(), word, stmt->GetStmt());
//		for (int i = 0; i < variableStore.size(); i++) { // insert the variable after inserting the statement due to FK
//			Database::insertVariable(variableStore.at(i).GetStmt(), variableStore.at(i).GetAdjustedStmtNum());
//		}
//
//		for (int i = 0; i < useStore.size(); i++) {
//			// database PK constraint will trigger for duplicate variables with same line_num to prevent duplicate insertion
//			Database::insertUses(useStore.at(i).GetAdjustedStmtNum(), useStore.at(i).GetStmt());
//			procedure.back()->_uses.push_back(useStore.at(i).GetStmt());
//		}
	vector<DescriberEntStmt> de;
	return de;
}
