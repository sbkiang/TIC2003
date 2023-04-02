#pragma once
#pragma once
#include <string>
#include <format>

using namespace std;

class Call {
public:
	static string GetCallConstruct_Synonym_Synonym(string input1, string input2); // Call(procedure, procedure)
	static string GetCallConstruct_Synonym_NotSynonym(string input1); // Call(procedure, "Second"/_)
	static string GetCallConstruct_NotSynonym_Synonym(string input2); // Call("First"/_, procedure)
	static string GetCallConstruct_NotSynonym_NotSynonym(); // Call("First"/_, "Second"/_)

	static string GetCall_Any_Any(string frontSql); // Call(stmt/_ , stmt/_) 
	static string GetCall_Any_Synonym(string frontSql, string input2); // Call(stmt/_, read/print/assign/while/if/call)
	static string GetCall_Any_Specific(string frontSql, string input2); // Call(stmt/_, 10)
	static string GetCall_Synonym_Any(string frontSql, string input1); // Call(while/if, stmt/_)
	static string GetCall_Synonym_Synonym(string frontSql, string input1, string input2); // Call(while/if, read/print/assign/while/if/call)
	static string GetCall_Synonym_Specific(string frontSql, string input1, string input2); // Call(while/if, 10)
	static string GetCall_Specific_Any(string frontSql, string input1); // Call(10, stmt/_)
	static string GetCall_Specific_Synonym(string frontSql, string input1, string input2); // Call(10, read/print/assign/while/if/call)
	static string GetCall_Specific_Specific(string frontSql, string input1, string input2); // // Call(10, 10)

	static string GetCallT_Any_Any(string frontSql); // Call(stmt/_ , stmt/_) 
	static string GetCallT_Any_Synonym(string frontSql, string input2); // Call(stmt/_, read/print/assign/while/if/call)
	static string GetCallT_Any_Specific(string frontSql, string input2); // Call(stmt/_, 10)
	static string GetCallT_Synonym_Any(string frontSql, string input1); // Call(while/if, stmt/_)
	static string GetCallT_Synonym_Synonym(string frontSql, string input1, string input2); // Call(while/if, read/print/assign/while/if/call)
	static string GetCallT_Synonym_Specific(string frontSql, string input1, string input2); // Call(while/if, 10)
	static string GetCallT_Specific_Any(string frontSql, string input1); // Call(10, stmt/_)
	static string GetCallT_Specific_Synonym(string frontSql, string input1, string input2); // Call(10, read/print/assign/while/if/call)
	static string GetCallT_Specific_Specific(string frontSql, string input1, string input2); // // Call(10, 10)

};
