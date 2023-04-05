#pragma once
#include <string>
#include "RelEnt.h"
#include <map>

//Interface class for all the relationship's SelectBuilders (the part where we construct the sql SELECT query to match the col name)

using namespace std;

class IColumnBuilderSqlRelRefNameStmt {
private:
	virtual string Build_NameSynonym_StmtSynonym(string syn1, string syn2) = 0;
	virtual string Build_NameSynonym_StmtNotSynonym(string syn1) = 0;
	virtual string Build_NameNotSynonym_StmtSynonym(string syn2) = 0;
	virtual string Build_NameNotSynonym_StmtNotSynonym() = 0;
public:
	virtual string GetSqlColumnQuery() = 0;
};
