#pragma once
#include "IColumnBuilderSqlRelRef.h"

class ColumnBuilderSqlCalls : public IColumnBuilderSqlRelRef{
private:
	RelEnt _re;

	string Build_NameSynonym_NameSynonym(string syn1, string syn2);
	string Build_NameSynonym_NameNotSynonym(string syn1);
	string Build_NameNotSynonym_NameSynonym(string syn2);
	string Build_NameNotSynonym_NameNotSynonym();

public:
	ColumnBuilderSqlCalls(RelEnt re);
	string GetSqlQuery(RelEntDescriber red);
};