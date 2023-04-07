#pragma once
#include <string>
#include <map>
#include "RelEnt.h"
#include "RelEntDescriber.h"

using namespace std;

//Interface class for all the relationship's SelectBuilders (the part where we construct the sql SELECT query to match the col name)
class IColumnBuilderSqlRelRef {
public:
	virtual string GetSqlQuery(RelEntDescriber red) = 0;
};
