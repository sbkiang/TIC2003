#pragma once
#include "IBuilderSqlConstrain.h"

class BuilderSqlColumnPattern : IBuilderSqlConstrain {
private:
	// Pattern a(v, expr)
	string Build_Synonym(string selectSynonym, string input1Synonym);

	// Pattern a(/"x", expr)
	string Build_NotSynonym(string selectSynonym);

public:
	BuilderSqlColumnPattern();
	string GetSql(IClConstrain& re, IDescriberClConstrain& dcp);
};