#pragma once
#include <string>
#include <map>
#include "DescriberClRelRef.h"
#include "ClRelRef.h";

using namespace std;

//Interface class for all the relationship's SelectBuilders (the part where we construct the sql SELECT query to match the col name)
class IBuilderSqlRelRef {
public:
	virtual string GetSql(DescriberClRelRef describer) = 0;
};
