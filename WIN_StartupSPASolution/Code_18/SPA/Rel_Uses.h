#pragma once
#include <string>
#include "../source/Database.h"
#include <format>

using namespace std;

class Uses {
public:
	static string GetUsesConstruct_StatementSynonym_Synonym(string input1, string input2);
	static string GetUsesConstruct_StatementSynonym_NotSynonym(string input1);
	static string GetUsesConstruct_NameSynonym_Synonym(string input1, string input2);
	static string GetUsesConstruct_NameSynonym_NotSynonym(string input1);

	static string GetUsesConstruct_StatementNotSynonym_Synonym(string input2);
	static string GetUsesConstruct_StatementNotSynonym_NotSynonym();
	static string GetUsesConstruct_NameNotSynonym_Synonym(string input2);
	static string GetUsesConstruct_NameNotSynonym_NotSynonym();

	// Uses(print/assign, var/_)
	static bool GetUses_AnyPrintAssign_Any(string frontSql, string input1, SqlResultStore& rs);

	// Uses(call, var/_)
	static bool GetUses_AnyCall_Any(string frontSql, SqlResultStore& rs);

	// Uses(procedure, var/_)
	static bool GetUses_AnyProcedure_Any(string frontSql, SqlResultStore& rs);

	// Uses(while/if, var/_)
	static bool GetUses_AnyWhileIf_Any(string frontSql, string input1, SqlResultStore& rs);

	// Uses(print/assign, "x")
	static bool GetUses_AnyPrintAssign_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	// Uses(call, "x")
	static bool GetUses_AnyCall_Specific(string frontSql, string input2, SqlResultStore& rs);

	// Uses(procedure, "x")
	static bool GetUses_AnyProcedure_Specific(string frontSql, string input2, SqlResultStore& rs);

	// Uses(while/if, "x")
	static bool GetUses_AnyWhileIf_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	// Uses(10, var/_) where 10 = print/assign
	static bool GetUses_SpecificPrintAssign_Any(string frontSql, string input1, SqlResultStore& rs);

	// Uses(10, var/_) where 10 = call
	static bool GetUses_SpecificCall_Any(string frontSql, string input1, SqlResultStore& rs);

	// Uses("main", var/_)
	static bool GetUses_SpecificProcedure_Any(string frontSql, string input1, SqlResultStore& rs);

	// Uses("10", var/_) where 10 = while/if
	static bool GetUses_SpecificWhileIf_Any(string frontSql, string input1, SqlResultStore& rs);

	// Uses(10, "x") where 10 = print/assign
	static bool GetUses_SpecificPrintAssign_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	// Uses(10, var/_) where 10 = call
	static bool GetUses_SpecificCall_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	// Uses("main", var/_)
	static bool GetUses_SpecificProcedure_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	// Uses("10", var/_) where 10 = while/if
	static bool GetUses_SpecificWhileIf_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);
};