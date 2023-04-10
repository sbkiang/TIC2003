#pragma once
#include "IBuilderSqlRelRef.h"

using namespace std;

//Interface class for all the relationship's QueryBuilders (the part where we select the columns from table)
class BuilderSqlSelectNext : IBuilderSqlRelRef {
private:
	ClRelRef _re;

	// next(stmt/_, stmt/_)
	string Build_Any_Any();

	// next(stmt/_, 10)
	string Build_Any_Specific(string input2);

	// next(stmt/_, stmtRef_excld_stmt)
	string Build_Any_Synonym(string input2);

	// next(stmtRef_excld_stmt, stmt/_)
	string Build_Synonym_Any(string input1);

	// next(stmtRef_excld_stmt, stmtRef_excld_stmt)
	string Build_Synonym_Synonym(string input1, string input2);

	// next(stmtRef_excld_stmt, 10)
	string Build_Synonym_Specific(string input1, string input2);

	// next(10, stmt/_)
	string Build_Specific_Any(string input1);

	// next(10, 11)
	string Build_Specific_Specific(string input1, string input2);

	// next(10, stmtRef_excld_stmt)
	string Build_Specific_Synonym(string input1, string input2);

public:
	BuilderSqlSelectNext(ClRelRef re);
	string GetSql(DescriberClRelRef describer);
};