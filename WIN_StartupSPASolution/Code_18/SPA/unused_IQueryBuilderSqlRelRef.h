#pragma once
#include <string>
#include <map>
#include "DescriberClRelRef.h"
#include "ClRelation.h";

using namespace std;

//Interface class for all the relationship's QueryBuilders (the part where we select the columns from table)
class IQueryBuilderSqlRelRef {
public:
	virtual string GetSqlQuery(DescriberClRelRef describer) = 0;
};