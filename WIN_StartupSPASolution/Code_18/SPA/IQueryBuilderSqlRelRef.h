#pragma once
#include <string>
#include <map>
#include "DescriberClRelation.h"
#include "ClRelation.h";

using namespace std;

//Interface class for all the relationship's QueryBuilders (the part where we select the columns from table)
class IQueryBuilderSqlRelRef {
public:
	virtual string GetSqlQuery(DescriberClRelation describer) = 0;
};