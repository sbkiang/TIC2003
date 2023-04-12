#pragma once
#include <string>
#include <map>
#include "DescriberClPattern.h"
#include "ClPattern.h"
#include "ClRelRef.h"
#include "IDescriberClConstrain.h"

using namespace std;

//Interface class for all the relationship's SelectBuilders (the part where we construct the sql SELECT query to match the col name)
class IBuilderSqlConstrain {
public:
	virtual string GetSql(IDescriberClConstrain& describer) = 0;
};
