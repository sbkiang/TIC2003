#pragma once
#include "IBuilderSql.h"

class ColumnBuilderSqlPattern : public IBuilderSql {
private:
	ClPattern _cp;

	// Pattern a(v, expr)
	string Build_Synonym(string selectSynonym, string input1Synonym);

	// Pattern a(/"x", expr)
	string Build_NotSynonym(string selectSynonym);

public:
	ColumnBuilderSqlPattern(ClPattern cp);
	string GetSqlQuery(DescriberClPattern dcp);
};