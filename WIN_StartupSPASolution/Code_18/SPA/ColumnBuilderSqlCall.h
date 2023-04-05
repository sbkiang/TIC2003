#pragma once
#include "IColumnBuilderSqlRelRefNameName.h"

class ColumnBuilderSqlCall : public IColumnBuilderSqlRelRefNameName{
private:
	RelEnt _re;
	string Build_NameSynonym_NameSynonym(string syn1, string syn2);
	string Build_NameSynonym_NameNotSynonym(string syn1);
	string Build_NameNotSynonym_NameSynonym(string syn2);
	string Build_NameNotSynonym_NameNotSynonym();
public:
	ColumnBuilderSqlCall(RelEnt re);
	string GetSqlColumnQuery(map<string,string> synEntMap);
};