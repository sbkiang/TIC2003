#pragma once
#include "IBuilderSqlRelRef.h"

using namespace std;

//Interface class for all the relationship's QueryBuilders (the part where we select the columns from table)
class BuilderQuerySqlModifies : IBuilderSqlRelRef {
private:
	ClRelRef _re;

	// Modifies(read/assign, var/_)
	string Build_AnyReadAssign_Any(string input1);

	// Modifies(call, var/_)
	string Build_AnyCall_Any();

	// Modifies(procedure, var/_)
	string Build_AnyProcedure_Any();

	// Modifies(while/if, var/_)
	string Build_AnyWhileIf_Any(string input1);

	// Modifies(stmt/_, var/_)
	string Build_Any_Any();

	// Modifies(read/assign, "x")
	string Build_AnyReadAssign_Specific(string input1, string input2);

	// Modifies(call, "x")
	string Build_AnyCall_Specific(string input2);

	// Modifies(procedure, "x")
	string Build_AnyProcedure_Specific(string input2);

	// Modifies(while/if, "x")
	string Build_AnyWhileIf_Specific(string input1, string input2);

	// Modifies(stmt/_, "x)
	string Build_Any_Specific(string input2);

	// Modifies(10, var/_) where 10 = read/assign
	string Build_SpecificReadAssign_Any(string input1);

	// Modifies(10, var/_) where 10 = call
	string Build_SpecificCall_Any(string input1);

	// Modifies("main", var/_)
	string Build_SpecificProcedure_Any(string input1);

	// Modifies("10", var/_) where 10 = while/if
	string Build_SpecificWhileIf_Any(string input1);

	// Modifies(10, "x") where 10 = read/assign
	string Build_SpecificReadAssign_Specific(string input1, string input2);

	// Modifies(10, var/_) where 10 = call
	string Build_SpecificCall_Specific(string input1, string input2);

	// Modifies("main", var/_)
	string Build_SpecificProcedure_Specific(string input1, string input2);

	// Modifies("10", var/_) where 10 = while/if
	string Build_SpecificWhileIf_Specific(string input1, string input2);

public:
	BuilderQuerySqlModifies(ClRelRef re);
	string GetSqlQuery(DescriberClRelRef describer);
};