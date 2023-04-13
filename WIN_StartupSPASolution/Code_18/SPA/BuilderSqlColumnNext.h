#pragma once
#include "IBuilderSqlConstrain.h"

class BuilderSqlColumnNext : public IBuilderSqlConstrain {
private:
	string Build_StmtSynonym_StmtSynonym(string syn1, string syn2);
	string Build_StmtSynonym_StmtNotSynonym(string syn1);
	string Build_StmtNotSynonym_StmtSynonym(string syn2);
	string Build_StmtNotSynonym_StmtNotSynonym();

public:
	BuilderSqlColumnNext();
	string GetSql(IClConstrain& re, IDescriberClConstrain& describer);
};