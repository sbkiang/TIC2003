#pragma once
#include <string>
#include <map>
#include "IQueryBuilderSqlRelRef.h"
#include "../SPA/RelEnt.h"

using namespace std;

//Interface class for all the relationship's QueryBuilders (the part where we select the columns from table)
class QueryBuilderSqlCalls : IQueryBuilderSqlRelRef {
private:
	RelEnt _re;

	// Call(procedure, procedure)
	string Build_Synonym_Synonym();

	// Call(_,_) 
	string Build_Any_Any();

	// Call(_, procedure)
	string Build_Any_Synonym();

	// Call(procedure, _)
	string Build_Synonym_Any();

	// Call("First", _)
	string Build_Specific_Any(string input1);

	// Call(_, "Second")
	string Build_Any_Specific(string input2);

	// Call(procedure, "Second")
	string Build_Synonym_Specific(string input2);

	// Call(("First",, procedure)
	string Build_Specific_Synonym(string input1);

	// Call("First", "Second")
	string Build_Specific_Specific(string input1, string input2);

public:
	QueryBuilderSqlCalls(RelEnt re);
	string GetSqlQuery(RelEntDescriber red);
};