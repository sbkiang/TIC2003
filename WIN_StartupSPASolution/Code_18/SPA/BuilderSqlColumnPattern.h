#pragma once
#include "IBuilderSqlConstrain.h"

class BuilderSqlColumnPattern : IBuilderSqlConstrain {
private:
	ClPattern _cp;

	// Pattern a(v, expr)
	string Build_Synonym(string selectSynonym, string input1Synonym);

	// Pattern a(/"x", expr)
	string Build_NotSynonym(string selectSynonym);

public:
	BuilderSqlColumnPattern(ClPattern cp);
	string GetSql(IDescriberClConstrain& dcp);
};