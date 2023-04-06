#pragma once
#include "IColumnBuilderSqlRelRefStmtName.h"
#include "IColumnBuilderSqlRelRefNameName.h"
#include <regex>

class ColumnBuilderSqlUses : public IColumnBuilderSqlRelRefStmtName, public IColumnBuilderSqlRelRefNameName {
private:
	// Stmt - Name
	static string Build_StmtSynonym_NameSynonym(string syn1, string syn2);
	static string Build_StmtSynonym_NameNotSynonym(string syn1);
	static string Build_StmtNotSynonym_NameSynonym(string syn2);
	static string Build_StmtNotSynonym_NameNotSynonym();

	// Name - Name
	static string Build_NameSynonym_NameSynonym(string input1, string input2);
	static string Build_NameSynonym_NameNotSynonym(string input1);
	static string Build_NameNotSynonym_NameSynonym(string input2);
	static string Build_NameNotSynonym_NameNotSynonym();
public:
	static string GetSqlColumnQuery(RelEnt re, map<string, string> synEntMap);
};