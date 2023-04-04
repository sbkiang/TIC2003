#pragma once
#include <string>
#include <format>

using namespace std;

class Next{
public:

	static void insertNext(int from, int to);

	// next(stmtRef, stmtRef)
	static string NextConstructSelect_Synonym_Synonym(string synonym1, string synonym2);

	// next(stmtRef, 10)
	static string NextConstructSelect_Synonym_NotSynonym(string synonym1);

	// next(10, stmtRef)
	static string NextConstructSelect_NotSynonym_Synonym(string synonym2);

	// next(10, 11)
	static string NextConstructSelect_NotSynonym_NotSynonym();

	// next(stmt/_, stmt/_)
	static string NextConstructQuery_Any_Any(string frontSql);
	
	// next(stmt/_, 10)
	static string NextConstructQuery_Any_Specific(string frontSql, string input2);
	
	// next(stmt/_, stmtRef_excld_stmt)
	static string NextConstructQuery_Any_Synonym(string frontSql, string input2);

	// next(stmtRef_excld_stmt, stmt/_)
	static string NextConstructQuery_Synonym_Any(string frontSql, string input1);

	// next(stmtRef_excld_stmt, stmtRef_excld_stmt)
	static string NextConstructQuery_Synonym_Synonym(string frontSql, string input1, string input2);

	// next(stmtRef_excld_stmt, 10)
	static string NextConstructQuery_Synonym_Specific(string frontSql, string input1, string input2);

	// next(10, stmt/_)
	static string NextConstructQuery_Specific_Any(string frontSql, string input1);

	// next(10, 11)
	static string NextConstructQuery_Specific_Specific(string frontSql, string input1, string input2);

	// next(10, stmtRef_excld_stmt)
	static string NextConstructQuery_Specific_Synonym(string frontSql, string input1, string input2);

	// next*(stmt/_, stmt/_)
	static string NextTConstructQuery_Any_Any(string frontSql);

	// next*(stmt/_, 10)
	static string NextTConstructQuery_Any_Specific(string frontSql, string line_num);

	// next*(stmt/_, stmtRef_excld_stmt)
	static string NextTConstructQuery_Any_Synonym(string frontSql, string entity);

	// next*(stmtRef_excld_stmt, stmt/_)
	static string NextTConstructQuery_Synonym_Any(string frontSql, string entity);

	// next*(stmtRef_excld_stmt, stmtRef_excld_stmt)
	static string NextTConstructQuery_Synonym_Synonym(string frontSql, string entity1, string entity2);

	// next*(stmtRef_excld_stmt, 10)
	static string NextTConstructQuery_Synonym_Specific(string frontSql, string entity, string line_num);

	// next*(10, stmt/_)
	static string NextTConstructQuery_Specific_Any(string frontSql, string line_num);

	// next*(10, 11)
	static string NextTConstructQuery_Specific_Specific(string frontSql, string line_num1, string line_num2);

	// next*(10, stmtRef_excld_stmt)
	static string NextTConstructQuery_Specific_Synonym(string frontSql, string line_num, string entity);

};