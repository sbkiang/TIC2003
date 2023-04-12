#pragma once
#include "IBuilderSqlConstrain.h"

using namespace std;

//Interface class for all the relationship's QueryBuilders (the part where we select the columns from table)
class BuilderSqlSelectNextT : IBuilderSqlConstrain {
private:
	ClRelRef _re;

	// next*(stmt/_, stmt/_)
	string Build_Any_Any();

	// next*(stmt/_, 10)
	string Build_Any_Specific(string line_num);

	// next*(stmt/_, stmtRef_excld_stmt)
	string Build_Any_Synonym(string entity);

	// next*(stmtRef_excld_stmt, stmt/_)
	string Build_Synonym_Any(string entity);

	// next*(stmtRef_excld_stmt, stmtRef_excld_stmt)
	string Build_Synonym_Synonym(string entity1, string entity2);

	// next*(stmtRef_excld_stmt, 10)
	string Build_Synonym_Specific(string entity, string line_num);

	// next*(10, stmt/_)
	string Build_Specific_Any(string line_num);

	// next*(10, 11)
	string Build_Specific_Specific(string line_num1, string line_num2);

	// next*(10, stmtRef_excld_stmt)
	string Build_Specific_Synonym(string line_num, string entity);

public:
	BuilderSqlSelectNextT(ClRelRef re);
	string GetSql(IDescriberClConstrain& describer);
};