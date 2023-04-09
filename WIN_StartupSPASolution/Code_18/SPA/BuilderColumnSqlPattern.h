#pragma once
#include "IBuilderSqlPattern.h"

class BuilderColumnSqlPattern : IBuilderSqlPattern {
private:
	ClPattern _cp;

	// Pattern a(v, expr)
	string Build_Synonym(string selectSynonym, string input1Synonym);

	// Pattern a(/"x", expr)
	string Build_NotSynonym(string selectSynonym);

public:
	BuilderColumnSqlPattern(ClPattern cp);
	string GetSql(DescriberClPattern dcp);
};