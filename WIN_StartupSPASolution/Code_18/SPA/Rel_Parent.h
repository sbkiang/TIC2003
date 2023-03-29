#pragma once
#pragma once
#include <string>
#include "../source/Database.h"
#include <format>

using namespace std;

class Parent {
public:
	static string GetParentConstruct_Synonym_Synonym(string input1, string input2);
	static string GetParentConstruct_Synonym_NotSynonym(string input1);
	static string GetParentConstruct_NotSynonym_Synonym(string input2);
	static string GetParentConstruct_NotSynonym_NotSynonym();

	static bool GetParentAnyAny(string frontSql, SqlResultStore& rs); // Parent(stmt/_ , stmt/_) 
	static bool GetParentAnySynonym(string frontSql, string input2, SqlResultStore& rs); // Parent(stmt/_, read/print/assign/while/if/call)
	static bool GetParentAnySpecific(string frontSql, string input2, SqlResultStore& rs); // Parent(stmt/_, 10)
	static bool GetParentSynonymAny(string frontSql, string input1, SqlResultStore& rs); // Parent(while/if, stmt/_)
	static bool GetParentSynonymSynonym(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(while/if, read/print/assign/while/if/call)
	static bool GetParentSynonymSpecific(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(while/if, 10)
	static bool GetParentSpecificAny(string frontSql, string input1, SqlResultStore& rs); // Parent(10, stmt/_)
	static bool GetParentSpecificSynonym(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(10, read/print/assign/while/if/call)
	static bool GetParentSpecificSpecific(string frontSql, string input1, string input2, SqlResultStore& rs); // // Parent(10, 10)

	static bool GetParentTAnyAny(string frontSql, SqlResultStore& rs); // Parent(stmt/_ , stmt/_) 
	static bool GetParentTAnySynonym(string frontSql, string input2, SqlResultStore& rs); // Parent(stmt/_, read/print/assign/while/if/call)
	static bool GetParentTAnySpecific(string frontSql, string input2, SqlResultStore& rs); // Parent(stmt/_, 10)
	static bool GetParentTSynonymAny(string frontSql, string input1, SqlResultStore& rs); // Parent(while/if, stmt/_)
	static bool GetParentTSynonymSynonym(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(while/if, read/print/assign/while/if/call)
	static bool GetParentTSynonymSpecific(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(while/if, 10)
	static bool GetParentTSpecificAny(string frontSql, string input1, SqlResultStore& rs); // Parent(10, stmt/_)
	static bool GetParentTSpecificSynonym(string frontSql, string input1, string input2, SqlResultStore& rs); // Parent(10, read/print/assign/while/if/call)
	static bool GetParentTSpecificSpecific(string frontSql, string input1, string input2, SqlResultStore& rs); // // Parent(10, 10)

};