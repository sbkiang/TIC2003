#pragma once
#include <string>
#include "../source/Database.h"
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
	static bool GetModifies_AnyReadAssign_Any(string frontSql, string input1, SqlResultStore& rs);

	// Uses(call, var/_)
	static bool GetModifies_AnyCall_Any(string frontSql, SqlResultStore& rs);

	// Uses(procedure, var/_)
	static bool GetModifies_AnyProcedure_Any(string frontSql, SqlResultStore& rs);

	// Uses(while/if, var/_)
	static bool GetModifies_AnyWhileIf_Any(string frontSql, string input1, SqlResultStore& rs);

	// Uses(read/assign, "x")
	static bool GetModifies_AnyReadAssign_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	// Uses(call, "x")
	static bool GetModifies_AnyCall_Specific(string frontSql, string input2, SqlResultStore& rs);

	// Uses(procedure, "x")
	static bool GetModifies_AnyProcedure_Specific(string frontSql, string input2, SqlResultStore& rs);

	// Uses(while/if, "x")
	static bool GetModifies_AnyWhileIf_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	// Uses(10, var/_) where 10 = read/assign
	static bool GetModifies_SpecificReadAssign_Any(string frontSql, string input1, SqlResultStore& rs);

	// Uses(10, var/_) where 10 = call
	static bool GetModifies_SpecificCall_Any(string frontSql, string input1, SqlResultStore& rs);

	// Uses("main", var/_)
	static bool GetModifies_SpecificProcedure_Any(string frontSql, string input1, SqlResultStore& rs);

	// Uses("10", var/_) where 10 = while/if
	static bool GetModifies_SpecificWhileIf_Any(string frontSql, string input1, SqlResultStore& rs);

	// Uses(10, "x") where 10 = read/assign
	static bool GetModifies_SpecificReadAssign_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	// Uses(10, var/_) where 10 = call
	static bool GetModifies_SpecificCall_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	// Uses("main", var/_)
	static bool GetModifies_SpecificProcedure_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);

	// Uses("10", var/_) where 10 = while/if
	static bool GetModifies_SpecificWhileIf_Specific(string frontSql, string input1, string input2, SqlResultStore& rs);
};