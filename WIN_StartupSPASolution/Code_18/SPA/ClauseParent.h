#pragma once
#pragma once
#include <string>
#include <format>

using namespace std;

class Parent {
public:
	static string GetParentConstruct_Synonym_Synonym(string input1, string input2);
	static string GetParentConstruct_Synonym_NotSynonym(string input1);
	static string GetParentConstruct_NotSynonym_Synonym(string input2);
	static string GetParentConstruct_NotSynonym_NotSynonym();

	static string GetParentAnyAny(string frontSql, SqlResultStore& rs); // Parent(stmt/_ , stmt/_) 
	static string GetParentAnySynonym(string frontSql, string input2, SqlResultStore& rs); // Parent(stmt/_, read/print/assign/while/if/call)
	static string GetParentAnySpecific(string frontSql, string input2, SqlResultStore& rs); // Parent(stmt/_, 10)
	static string GetParentSynonymAny(string frontSql, string input1, SqlResultStore& rs); // Parent(while/if, stmt/_)
	static string GetParentSynonymSynonym(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(while/if, read/print/assign/while/if/call)
	static string GetParentSynonymSpecific(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(while/if, 10)
	static string GetParentSpecificAny(string frontSql, string input1, SqlResultStore& rs); // Parent(10, stmt/_)
	static string GetParentSpecificSynonym(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(10, read/print/assign/while/if/call)
	static string GetParentSpecificSpecific(string frontSql, string input1, string input2, SqlResultStore& rs); // // Parent(10, 10)

	static string GetParentTAnyAny(string frontSql, SqlResultStore& rs); // Parent(stmt/_ , stmt/_) 
	static string GetParentTAnySynonym(string frontSql, string input2, SqlResultStore& rs); // Parent(stmt/_, read/print/assign/while/if/call)
	static string GetParentTAnySpecific(string frontSql, string input2, SqlResultStore& rs); // Parent(stmt/_, 10)
	static string GetParentTSynonymAny(string frontSql, string input1, SqlResultStore& rs); // Parent(while/if, stmt/_)
	static string GetParentTSynonymSynonym(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(while/if, read/print/assign/while/if/call)
	static string GetParentTSynonymSpecific(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(while/if, 10)
	static string GetParentTSpecificAny(string frontSql, string input1, SqlResultStore& rs); // Parent(10, stmt/_)
	static string GetParentTSpecificSynonym(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(10, read/print/assign/while/if/call)
	static string GetParentTSpecificSpecific(string frontSql, string input1, string input2, SqlResultStore& rs); // // Parent(10, 10)

};