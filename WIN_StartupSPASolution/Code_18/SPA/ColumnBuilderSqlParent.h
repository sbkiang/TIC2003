#pragma once
#include "IColumnBuilderSqlRelRefStmtStmt.h"

class ColumnBuilderSqlParent : public IColumnBuilderSqlRelRefStmtStmt{
private:
	static string Build_StmtSynonym_StmtSynonym(string syn1, string syn2);
	static string Build_StmtSynonym_StmtNotSynonym(string syn1);
	static string Build_StmtNotSynonym_StmtSynonym(string syn2);
	static string Build_StmtNotSynonym_StmtNotSynonym();
public:
	static string GetSqlColumnQuery(RelEnt re, map<string,string> synEntMap);
};