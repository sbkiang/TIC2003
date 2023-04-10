#pragma once
#include "IBuilderSqlPattern.h"

class BuilderSqlColumnPattern : IBuilderSqlPattern {
private:
	ClPattern _cp;

	// Pattern a(v, expr)
	string Build_Synonym(string selectSynonym, string input1Synonym);

	// Pattern a(/"x", expr)
	string Build_NotSynonym(string selectSynonym);

public:
	BuilderSqlColumnPattern(ClPattern cp);
	string GetSql(DescriberClPattern dcp);
};