#pragma once
#include "IColumnBuilderSqlRelRefNameName.h"

class ColumnBuilderSqlCall : public IColumnBuilderSqlRelRefNameName{
private:
	static string Build_NameSynonym_NameSynonym(string syn1, string syn2);
	static string Build_NameSynonym_NameNotSynonym(string syn1);
	static string Build_NameNotSynonym_NameSynonym(string syn2);
	static string Build_NameNotSynonym_NameNotSynonym();
public:
	static string GetSqlColumnQuery(RelEnt re, map<string,string> synEntMap);
};