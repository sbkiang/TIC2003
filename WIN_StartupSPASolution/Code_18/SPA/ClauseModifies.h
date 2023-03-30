#pragma once
#include <string>
#include <format>

using namespace std;

class Modifies {
public:
	static string GetModifiesConstruct_StatementSynonym_Synonym(string input1, string input2);
	static string GetModifiesConstruct_StatementSynonym_NotSynonym(string input1);
	static string GetModifiesConstruct_NameSynonym_Synonym(string input1, string input2);
	static string GetModifiesConstruct_NameSynonym_NotSynonym(string input1);

	static string GetModifiesConstruct_StatementNotSynonym_Synonym(string input2);
	static string GetModifiesConstruct_StatementNotSynonym_NotSynonym();
	static string GetModifiesConstruct_NameNotSynonym_Synonym(string input2);
	static string GetModifiesConstruct_NameNotSynonym_NotSynonym();

	// Uses(read/assign, var/_)
	static string GetModifies_AnyReadAssign_Any(string frontSql, string input1);

	// Uses(call, var/_)
	static string GetModifies_AnyCall_Any(string frontSql);

	// Uses(procedure, var/_)
	static string GetModifies_AnyProcedure_Any(string frontSql);

	// Uses(while/if, var/_)
	static string GetModifies_AnyWhileIf_Any(string frontSql, string input1);

	// Uses(read/assign, "x")
	static string GetModifies_AnyReadAssign_Specific(string frontSql, string input1, string input2);

	// Uses(call, "x")
	static string GetModifies_AnyCall_Specific(string frontSql, string input2);

	// Uses(procedure, "x")
	static string GetModifies_AnyProcedure_Specific(string frontSql, string input2);

	// Uses(while/if, "x")
	static string GetModifies_AnyWhileIf_Specific(string frontSql, string input1, string input2);

	// Uses(10, var/_) where 10 = read/assign
	static string GetModifies_SpecificReadAssign_Any(string frontSql, string input1);

	// Uses(10, var/_) where 10 = call
	static string GetModifies_SpecificCall_Any(string frontSql, string input1);

	// Uses("main", var/_)
	static string GetModifies_SpecificProcedure_Any(string frontSql, string input1);

	// Uses("10", var/_) where 10 = while/if
	static string GetModifies_SpecificWhileIf_Any(string frontSql, string input1);

	// Uses(10, "x") where 10 = read/assign
	static string GetModifies_SpecificReadAssign_Specific(string frontSql, string input1, string input2);

	// Uses(10, var/_) where 10 = call
	static string GetModifies_SpecificCall_Specific(string frontSql, string input1, string input2);

	// Uses("main", var/_)
	static string GetModifies_SpecificProcedure_Specific(string frontSql, string input1, string input2);

	// Uses("10", var/_) where 10 = while/if
	static string GetModifies_SpecificWhileIf_Specific(string frontSql, string input1, string input2);
};