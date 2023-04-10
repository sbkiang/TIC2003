#pragma once
#include "IBuilderSqlRelRef.h"

class BuilderSqlColumnNext : public IBuilderSqlRelRef {
private:
	ClRelRef _re;
	
	string Build_StmtSynonym_StmtSynonym(string syn1, string syn2);
	string Build_StmtSynonym_StmtNotSynonym(string syn1);
	string Build_StmtNotSynonym_StmtSynonym(string syn2);
	string Build_StmtNotSynonym_StmtNotSynonym();

public:
	BuilderSqlColumnNext(ClRelRef re);
	string GetSql(DescriberClRelRef describer);
};