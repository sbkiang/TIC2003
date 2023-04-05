#pragma once
#include <string>
#include "RelEnt.h"
#include <map>

//Interface class for all the relationship's SelectBuilders (the part where we construct the sql SELECT query to match the col name)

using namespace std;

class IColumnBuilderSqlRelRefStmtName {
private:
	virtual string Build_StmtSynonym_NameSynonym(string syn1, string syn2) = 0;
	virtual string Build_StmtSynonym_NameNotSynonym(string syn1) = 0;
	virtual string Build_StmtNotSynonym_NameSynonym(string syn2) = 0;
	virtual string Build_StmtNotSynonym_NameNotSynonym() = 0;
public:
	virtual string GetSqlColumnQuery() = 0;
};
