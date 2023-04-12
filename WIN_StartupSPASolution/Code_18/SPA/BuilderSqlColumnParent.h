#pragma once
#include "IBuilderSqlConstrain.h"

class BuilderSqlColumnParent : public IBuilderSqlConstrain {
private:
	ClRelRef _re;

	string Build_StmtSynonym_StmtSynonym(string syn1, string syn2);
	string Build_StmtSynonym_StmtNotSynonym(string syn1);
	string Build_StmtNotSynonym_StmtSynonym(string syn2);
	string Build_StmtNotSynonym_StmtNotSynonym();

public:
	BuilderSqlColumnParent(ClRelRef re);
	string GetSql(IDescriberClConstrain& describer);
};