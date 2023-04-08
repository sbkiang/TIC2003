#pragma once
#include "IBuilderSqlRelRef.h"

class ColumnBuilderSqlCalls : public IBuilderSqlRelRef {
private:
	ClRelRef _re;

	string Build_NameSynonym_NameSynonym(string syn1, string syn2);
	string Build_NameSynonym_NameNotSynonym(string syn1);
	string Build_NameNotSynonym_NameSynonym(string syn2);
	string Build_NameNotSynonym_NameNotSynonym();

public:
	ColumnBuilderSqlCalls(ClRelRef re);
	string GetSqlQuery(DescriberClRelRef describer);
};