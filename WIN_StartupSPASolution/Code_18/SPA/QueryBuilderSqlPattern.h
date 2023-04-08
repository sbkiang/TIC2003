#pragma once
#include "IBuilderSqlPattern.h"

class QueryBuilderSqlPattern : public IBuilderSqlPattern{
private:
	ClPattern _clause;
	string Build_NotAny_Expr(string input1, string input2);
	string Build_Any_Expr(string input2);

public:
	QueryBuilderSqlPattern(ClPattern clause);
	string GetSqlQuery(DescriberClPattern describer);
};