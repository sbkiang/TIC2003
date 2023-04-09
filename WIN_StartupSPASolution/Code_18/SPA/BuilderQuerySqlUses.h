#pragma once
#include "IBuilderSqlRelRef.h"

using namespace std;

//Interface class for all the relationship's QueryBuilders (the part where we select the columns from table)
class BuilderQuerySqlUses : IBuilderSqlRelRef {
private:
	ClRelRef _re;

	// Uses(print/assign, var/_)
	string Build_AnyPrintAssign_Any(string input1);

	// Uses(call, var/_)
	string Build_AnyCall_Any();

	// Uses(procedure, var/_)
	string Build_AnyProcedure_Any();

	// Uses(while/if, var/_)
	string Build_AnyWhileIf_Any(string input1);

	// Uses(stmt/_, var/_)
	string Build_Any_Any();

	// Uses(print/assign, "x")
	string Build_AnyPrintAssign_Specific(string input1, string input2);

	// Uses(call, "x")
	string Build_AnyCall_Specific(string input2);

	// Uses(procedure, "x")
	string Build_AnyProcedure_Specific(string input2);

	// Uses(while/if, "x")
	string Build_AnyWhileIf_Specific(string input1, string input2);

	// Uses(stmt/_, "x")
	string Build_Any_Specific(string input2);

	// Uses(10, var/_) where 10 = print/assign
	string Build_SpecificPrintAssign_Any(string input1);

	// Uses(10, var/_) where 10 = call
	string Build_SpecificCall_Any(string input1);

	// Uses("main", var/_)
	string Build_SpecificProcedure_Any(string input1);

	// Uses("10", var/_) where 10 = while/if
	string Build_SpecificWhileIf_Any(string input1);

	// Uses(10, "x") where 10 = print/assign
	string Build_SpecificPrintAssign_Specific(string input1, string input2);

	// Uses(10, "x") where 10 = call
	string Build_SpecificCall_Specific(string input1, string input2);

	// Uses("main", "x")
	string Build_SpecificProcedure_Specific(string input1, string input2);

	// Uses("10", "x") where 10 = while/if
	string Build_SpecificWhileIf_Specific(string input1, string input2);

public:
	BuilderQuerySqlUses(ClRelRef re);
	string GetSql(DescriberClRelRef describer);
};