#pragma once
#include <string>
#include <format>

using namespace std;

class Uses {
public:

	static void insertUses(int stmtNum, string variablename);

	static string GetUsesConstruct_StatementSynonym_Synonym(string input1, string input2);
	static string GetUsesConstruct_StatementSynonym_NotSynonym(string input1);
	static string GetUsesConstruct_NameSynonym_Synonym(string input1, string input2);
	static string GetUsesConstruct_NameSynonym_NotSynonym(string input1);

	static string GetUsesConstruct_StatementNotSynonym_Synonym(string input2);
	static string GetUsesConstruct_StatementNotSynonym_NotSynonym();
	static string GetUsesConstruct_NameNotSynonym_Synonym(string input2);
	static string GetUsesConstruct_NameNotSynonym_NotSynonym();

	// Uses(print/assign, var/_)
	static string GetUses_AnyPrintAssign_Any(string frontSql, string input1);

	// Uses(call, var/_)
	static string GetUses_AnyCall_Any(string frontSql);

	// Uses(procedure, var/_)
	static string GetUses_AnyProcedure_Any(string frontSql);

	// Uses(while/if, var/_)
	static string GetUses_AnyWhileIf_Any(string frontSql, string input1);

	// Uses(stmt/_, var/_)
	static string GetUses_Any_Any(string frontSql);

	// Uses(print/assign, "x")
	static string GetUses_AnyPrintAssign_Specific(string frontSql, string input1, string input2);

	// Uses(call, "x")
	static string GetUses_AnyCall_Specific(string frontSql, string input2);

	// Uses(procedure, "x")
	static string GetUses_AnyProcedure_Specific(string frontSql, string input2);

	// Uses(while/if, "x")
	static string GetUses_AnyWhileIf_Specific(string frontSql, string input1, string input2);

	// Uses(stmt/_, "x")
	static string GetUses_Any_Specific(string frontSql, string input2);

	// Uses(10, var/_) where 10 = print/assign
	static string GetUses_SpecificPrintAssign_Any(string frontSql, string input1);

	// Uses(10, var/_) where 10 = call
	static string GetUses_SpecificCall_Any(string frontSql, string input1);

	// Uses("main", var/_)
	static string GetUses_SpecificProcedure_Any(string frontSql, string input1);

	// Uses("10", var/_) where 10 = while/if
	static string GetUses_SpecificWhileIf_Any(string frontSql, string input1);

	// Uses(10, "x") where 10 = print/assign
	static string GetUses_SpecificPrintAssign_Specific(string frontSql, string input1, string input2);

	// Uses(10, "x") where 10 = call
	static string GetUses_SpecificCall_Specific(string frontSql, string input1, string input2);

	// Uses("main", "x")
	static string GetUses_SpecificProcedure_Specific(string frontSql, string input1, string input2);

	// Uses("10", "x") where 10 = while/if
	static string GetUses_SpecificWhileIf_Specific(string frontSql, string input1, string input2);
};