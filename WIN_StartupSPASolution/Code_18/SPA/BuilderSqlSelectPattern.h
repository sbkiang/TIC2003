#pragma once
#include "IBuilderSqlConstrain.h"

class BuilderSqlSelectPattern : public IBuilderSqlConstrain {
private:
	ClPattern _clause;
	string Build_NotAny_Expr(string input1, string input2);
	string Build_Any_Expr(string input2);

public:
	BuilderSqlSelectPattern(ClPattern clause);
	string GetSql(IDescriberClConstrain& describer);
};