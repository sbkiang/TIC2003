#pragma once
#include <string>
#include <map>
#include "IDescriberCl.h"

using namespace std;

//Interface class for all the relationship's SelectBuilders (the part where we construct the sql SELECT query to match the col name)
class IBuilderSql {
public:
	virtual string GetSqlQuery(IDescriberCl describer) = 0;
};
