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

	// Modifies(read/assign, var/_)
	static string GetModifies_AnyReadAssign_Any(string frontSql, string input1); 

	// Modifies(call, var/_)
	static string GetModifies_AnyCall_Any(string frontSql);

	// Modifies(procedure, var/_)
	static string GetModifies_AnyProcedure_Any(string frontSql); 

	// Modifies(while/if, var/_)
	static string GetModifies_AnyWhileIf_Any(string frontSql, string input1);

	// Modifies(stmt/_, var/_)
	static string GetModifies_Any_Any(string frontSql);

	// Modifies(read/assign, "x")
	static string GetModifies_AnyReadAssign_Specific(string frontSql, string input1, string input2); 

	// Modifies(call, "x")
	static string GetModifies_AnyCall_Specific(string frontSql, string input2);

	// Modifies(procedure, "x")
	static string GetModifies_AnyProcedure_Specific(string frontSql, string input2); 

	// Modifies(while/if, "x")
	static string GetModifies_AnyWhileIf_Specific(string frontSql, string input1, string input2); 

	// Modifies(stmt/_, "x)
	static string GetModifies_Any_Specific(string frontSql, string input2);

	// Modifies(10, var/_) where 10 = read/assign
	static string GetModifies_SpecificReadAssign_Any(string frontSql, string input1); 

	// Modifies(10, var/_) where 10 = call
	static string GetModifies_SpecificCall_Any(string frontSql, string input1); 

	// Modifies("main", var/_)
	static string GetModifies_SpecificProcedure_Any(string frontSql, string input1);

	// Modifies("10", var/_) where 10 = while/if
	static string GetModifies_SpecificWhileIf_Any(string frontSql, string input1); 

	// Modifies(10, "x") where 10 = read/assign
	static string GetModifies_SpecificReadAssign_Specific(string frontSql, string input1, string input2);

	// Modifies(10, var/_) where 10 = call
	static string GetModifies_SpecificCall_Specific(string frontSql, string input1, string input2); 

	// Modifies("main", var/_)
	static string GetModifies_SpecificProcedure_Specific(string frontSql, string input1, string input2); 

	// Modifies("10", var/_) where 10 = while/if
	static string GetModifies_SpecificWhileIf_Specific(string frontSql, string input1, string input2); 


};