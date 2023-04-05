#pragma once
#include "IColumnBuilderSqlRelRefStmtName.h"
#include "IColumnBuilderSqlRelRefNameName.h"
#include <regex>

class ColumnBuilderSqlModifies : public IColumnBuilderSqlRelRefStmtName, public IColumnBuilderSqlRelRefNameName {
private:
	RelEnt _re;

	// Stmt - Name
	string Build_StmtSynonym_NameSynonym(string syn1, string syn2);
	string Build_StmtSynonym_NameNotSynonym(string syn1);
	string Build_StmtNotSynonym_NameSynonym(string syn2);
	string Build_StmtNotSynonym_NameNotSynonym();

	// Name - Name
	string Build_NameSynonym_NameSynonym(string input1, string input2);
	string Build_NameSynonym_NameNotSynonym(string input1);
	string Build_NameNotSynonym_NameSynonym(string input2);
	string Build_NameNotSynonym_NameNotSynonym();
public:
	ColumnBuilderSqlModifies(RelEnt re);
	string GetSqlColumnQuery(map<string, string> synEntMap);
};