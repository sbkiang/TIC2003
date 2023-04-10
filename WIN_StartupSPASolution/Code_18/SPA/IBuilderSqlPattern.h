#pragma once
#include <string>
#include <map>
#include "DescriberClPattern.h"
#include "ClPattern.h"

using namespace std;

//Interface class for all the relationship's SelectBuilders (the part where we construct the sql SELECT query to match the col name)
class IBuilderSqlPattern {
public:
	virtual string GetSql(DescriberClPattern describer) = 0;
};
