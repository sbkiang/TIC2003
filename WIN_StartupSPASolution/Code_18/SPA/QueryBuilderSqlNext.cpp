#include "QueryBuilderSqlNext.h"

string QueryBuilderSqlNext::Build_Any_Any()
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select * from next");
	return string(sqlBuf);
}

string QueryBuilderSqlNext::Build_Any_Specific(string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select * from next where to_line = %s", input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlNext::Build_Any_Synonym(string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select * from next where to_line in (select line_num from statement where entity = '%s')", input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlNext::Build_Synonym_Any(string input1)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select * from next where from_line in (select line_num from statement where entity = '%s')", input1.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlNext::Build_Synonym_Synonym(string input1, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select * from next where from_line in (select line_num from statement s where s.entity = '%s') and to_line in (select line_num from statement where entity = '%s')", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlNext::Build_Synonym_Specific(string input1, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select * from next where from_line in (select line_num from statement where entity = '%s') and to_line = %s", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlNext::Build_Specific_Any(string input1)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select * from next where from_line = %s)", input1.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlNext::Build_Specific_Specific(string input1, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select * from next where from_line = %s and to_line = %s)", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlNext::Build_Specific_Synonym(string input1, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select * from next where from_line = %s and to_line in (select line_num from statement where entity = '%s')", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

QueryBuilderSqlNext::QueryBuilderSqlNext(RelEnt re)
{
    _re = re;
}

string QueryBuilderSqlNext::GetSqlQuery(RelEntDescriber red)
{
	bool input1IsAny = red.Input1IsAny();
	bool input2IsAny = red.Input2IsAny();
	string input1 = _re.GetInput1Unquoted();
	string input2 = _re.GetInput2Unquoted();
	string entityInput1 = red.EntityInput1();
	string entityInput2 = red.EntityInput2();

	if (red.Input1IsAny() && red.Input2IsAny()) { // Next(entity/_, entity/_)
		if (red.Input1IsStmtOrWildcard() && red.Input2IsStmtOrWildcard()) { // Next(stmt/_, stmt/_)
			return Build_Any_Any();
		}
		else if (red.Input1IsStmtOrWildcard() && !red.Input2IsStmtOrWildcard()) { // Next(stmt/_, stmtRef_excld_stmt)
			return Build_Any_Synonym(entityInput2);
		}
		else if (!red.Input1IsStmtOrWildcard() && red.Input2IsStmtOrWildcard()) { // Next(stmtRef_excld_stmt, stmt/_)
			return Build_Synonym_Any(entityInput1);
		}
		else if (!red.Input1IsStmtOrWildcard() && !red.Input2IsStmtOrWildcard()) { // Next(stmtRef_excld_stmt, stmtRef_excld_stmt)
			return Build_Synonym_Synonym(entityInput1, entityInput2);
		}
	}
	else if (red.Input1IsAny() && !red.Input2IsAny()) { // Next(entity/_, 10)
		if (red.Input1IsStmtOrWildcard()) { // Next(stmt/_, 10)
			return Build_Any_Specific(input2);
		}
		else { // Next(stmtRef_excld_stmt, 10)
			return Build_Synonym_Specific(entityInput1, input2);
		}
	}
	else if (!red.Input1IsAny() && red.Input2IsAny()) { // Next(10, entity/_)
		if (red.Input2IsStmtOrWildcard()) { // Next(10, stmt/_)
			return Build_Specific_Any(input1);
		}
		else { // Next(10, stmtRef_excld_stmt)
			return Build_Specific_Synonym(input1, entityInput2);
		}
	}
	else if (!red.Input1IsAny() && !red.Input2IsAny()) { // Next(10, 11)
		return Build_Specific_Specific(input1, input2);
	}

    return string();
}
