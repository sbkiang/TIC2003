#pragma once
#include <string>
#include <map>
#include "DescriberClRelRef.h"

using namespace std;

//Interface class for all the relationship's SelectBuilders (the part where we construct the sql SELECT query to match the col name)
class IColumnBuilderSqlRelRef {
public:
	virtual string GetSqlQuery(DescriberClRelRef describer) = 0;
};
