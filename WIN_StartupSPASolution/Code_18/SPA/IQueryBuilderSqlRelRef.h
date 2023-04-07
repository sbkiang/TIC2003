#pragma once
#include <string>
#include <map>
#include "RelEnt.h"
#include "RelEntDescriber.h"

using namespace std;

//Interface class for all the relationship's QueryBuilders (the part where we select the columns from table)
class IQueryBuilderSqlRelRef {
public:
	virtual string GetSqlQuery(RelEntDescriber red) = 0;
};