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
	static string GetCallConstruct_Any_Specific(); //Call(_, "Second")
	static string GetCallConstruct_Specific_Any(); //Call("First", _)

	static string GetCall_Synonym_Synonym(string frontSql); // Call(procedure, procedure)
	static string GetCall_Any_Any(string frontSql); // Call(_,_) 
	static string GetCall_Any_Synonym(string frontSql); // Call(_, procedure)
	static string GetCall_Synonym_Any(string frontSql); // Call(procedure, _)
	static string GetCall_Specific_Any(string frontSql, string input1); // Call("First", _)
	static string GetCall_Any_Specific(string frontSql, string input2); // Call(_, "Second")
	static string GetCall_Synonym_Specific(string frontSql, string input2); // Call(procedure, "Second")
	static string GetCall_Specific_Synonym(string frontSql, string input1); // Call(("First",, procedure)
	static string GetCall_Specific_Specific(string frontSql, string input1, string input2); // Call("First", "Second")

	static string GetCallT_Synonym_Synonym(string frontSql); // Call*(procedure, procedure)
	static string GetCallT_Any_Any(string frontSql); // Call*(_,_) 
	static string GetCallT_Any_Synonym(string frontSql); // Call*(_, procedure)
	static string GetCallT_Synonym_Any(string frontSql); // Call*(procedure, _)
	static string GetCallT_Specific_Any(string frontSql, string input1); // Call*("First", _)
	static string GetCallT_Any_Specific(string frontSql, string input2); // Call*(_, "Second")
	static string GetCallT_Synonym_Specific(string frontSql, string input2); // Call*(procedure, "Second")
	static string GetCallT_Specific_Synonym(string frontSql, string input1); // Call*(("First",, procedure)
	static string GetCallT_Specific_Specific(string frontSql, string input1, string input2); // Call*("First", "Second")
};
