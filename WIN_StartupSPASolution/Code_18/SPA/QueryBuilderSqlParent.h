#pragma once
#include <string>
#include <map>
#include "../SPA/RelEnt.h"
#include "IQueryBuilderSqlRelRef.h"

using namespace std;

//Interface class for all the relationship's QueryBuilders (the part where we select the columns from table)
class QueryBuilderSqlParent : IQueryBuilderSqlRelRef {
private:
	RelEnt _re;

	// Parent(stmt/_ , stmt/_) 
	string Build_Any_Any();

	// Parent(stmt/_, read/print/assign/while/if/call)
	string Build_Any_Synonym(string input2);

	// Parent(stmt/_, 10)
	string Build_Any_Specific(string input2);

	// Parent(while/if, stmt/_)
	string Build_Synonym_Any(string input1);

	// Parent(while/if, read/print/assign/while/if/call)
	string Build_Synonym_Synonym(string input1, string input2);

	// Parent(while/if, 10)
	string Build_Synonym_Specific(string input1, string input2);

	// Parent(10, stmt/_)
	string Build_Specific_Any(string input1);

	// Parent(10, read/print/assign/while/if/call)
	string Build_Specific_Synonym(string input1, string input2);

	// Parent(10, 10)
	string Build_Specific_Specific(string input1, string input2);

public:
	QueryBuilderSqlParent(RelEnt re);
	string GetSqlQuery(RelEntDescriber red);
};