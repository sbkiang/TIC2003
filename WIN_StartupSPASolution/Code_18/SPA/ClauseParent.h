#pragma once
#pragma once
#include <string>
#include <format>

using namespace std;

class Parent {
public:
	static string GetParentConstruct_Synonym_Synonym(string input1, string input2); // Parent(stmt,stmt)
	static string GetParentConstruct_Synonym_NotSynonym(string input1); // Parent(stmt, 10/_)
	static string GetParentConstruct_NotSynonym_Synonym(string input2); // Parent(10/_, stmt)
	static string GetParentConstruct_NotSynonym_NotSynonym(); // Parent(10/_, 10/_)

	static string GetParent_Any_Any(string frontSql); // Parent(stmt/_ , stmt/_) 
	static string GetParent_Any_Synonym(string frontSql, string input2); // Parent(stmt/_, read/print/assign/while/if/call)
	static string GetParent_Any_Specific(string frontSql, string input2); // Parent(stmt/_, 10)
	static string GetParent_Synonym_Any(string frontSql, string input1); // Parent(while/if, stmt/_)
	static string GetParent_Synonym_Synonym(string frontSql, string input1, string input2); // Parent(while/if, read/print/assign/while/if/call)
	static string GetParent_Synonym_Specific(string frontSql, string input1, string input2); // Parent(while/if, 10)
	static string GetParent_Specific_Any(string frontSql, string input1); // Parent(10, stmt/_)
	static string GetParent_Specific_Synonym(string frontSql, string input1, string input2); // Parent(10, read/print/assign/while/if/call)
	static string GetParent_Specific_Specific(string frontSql, string input1, string input2); // // Parent(10, 10)

	static string GetParentT_Any_Any(string frontSql); // Parent(stmt/_ , stmt/_) 
	static string GetParentT_Any_Synonym(string frontSql, string input2); // Parent(stmt/_, read/print/assign/while/if/call)
	static string GetParentT_Any_Specific(string frontSql, string input2); // Parent(stmt/_, 10)
	static string GetParentT_Synonym_Any(string frontSql, string input1); // Parent(while/if, stmt/_)
	static string GetParentT_Synonym_Synonym(string frontSql, string input1, string input2); // Parent(while/if, read/print/assign/while/if/call)
	static string GetParentT_Synonym_Specific(string frontSql, string input1, string input2); // Parent(while/if, 10)
	static string GetParentT_Specific_Any(string frontSql, string input1); // Parent(10, stmt/_)
	static string GetParentT_Specific_Synonym(string frontSql, string input1, string input2); // Parent(10, read/print/assign/while/if/call)
	static string GetParentT_Specific_Specific(string frontSql, string input1, string input2); // // Parent(10, 10)

};