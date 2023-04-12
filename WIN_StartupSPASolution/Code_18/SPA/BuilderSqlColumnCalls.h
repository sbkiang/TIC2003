#pragma once
#include "IBuilderSqlConstrain.h"

class BuilderSqlColumnCalls : public IBuilderSqlConstrain {
private:
	ClRelRef _re;

	string Build_NameSynonym_NameSynonym(string syn1, string syn2);
	string Build_NameSynonym_NameNotSynonym(string syn1);
	string Build_NameNotSynonym_NameSynonym(string syn2);
	string Build_NameNotSynonym_NameNotSynonym();

public:
	BuilderSqlColumnCalls(ClRelRef re);
	string GetSql(IDescriberClConstrain& describer);
};