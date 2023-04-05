#pragma once
#include "IColumnBuilderSqlRelRefStmtStmt.h"

class ColumnBuilderSqlNext : public IColumnBuilderSqlRelRefStmtStmt{
private:
	RelEnt _re;
	string Build_StmtSynonym_StmtSynonym(string syn1, string syn2);
	string Build_StmtSynonym_StmtNotSynonym(string syn1);
	string Build_StmtNotSynonym_StmtSynonym(string syn2);
	string Build_StmtNotSynonym_StmtNotSynonym();
public:
	ColumnBuilderSqlNext(RelEnt re);
	string GetSqlColumnQuery(map<string,string> synEntMap);
};