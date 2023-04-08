#include "QueryBuilderSqlNextT.h"

string QueryBuilderSqlNextT::Build_Any_Any()
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "with recursive nextT as ("
		"select from_line, to_line from next"
		" union "
		"select n.from_line, n.to_line from next n join nextT nT where n.from_line = nT.to_line)"
		" select * from nextT");
	return string(sqlBuf);
}

string QueryBuilderSqlNextT::Build_Any_Specific(string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "with recursive nextT as ("
		"select from_line, to_line from next where to_line = %s"
		" union "
		"select n.from_line, n.to_line from next n join nextT nT on n.to_line = nT.from_line)"
		" select * from nextT", input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlNextT::Build_Any_Synonym(string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "with recursive nextT as ("
		"select from_line, to_line from next where to_line in (select line_num from statement where entity = '%s')"
		" union "
		"select n.from_line, n.to_line from next n join nextT nt on n.to_line = nt.from_line)"
		" select * from nextT", input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlNextT::Build_Synonym_Any(string input1)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "with recursive nextT as ("
		"select from_line, to_line from next where from_line in (select line_num from statement where entity = '%s')"
		" union "
		"select n.from_line, n.to_line from next n join nextT nT on n.from_line = nT.to_line)"
		" select * from nextT", input1.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlNextT::Build_Synonym_Synonym(string input1, string input2)
{
	char sqlBuf[600];
	sprintf_s(sqlBuf, "select * from ("
		"with recursive nextT as ("
		"select from_line, to_line from next where from_line in (select line_num from statement where entity = '%s')"
		" union "
		"select n.from_line, n.to_line from next n join nextT nT on n.from_line = nT.to_line)"
		" select * from nextT)"
		" intersect "
		"select * from ("
		"with recursive nextT as ("
		"select from_line, to_line from next where to_line in (select line_num from statement where entity = '%s')"
		" union "
		"select n.from_line, n.to_line from next n join nextT nt on n.to_line = nt.from_line)"
		" select * from nextT)", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlNextT::Build_Synonym_Specific(string input1, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select * from ("
		"with recursive nextT as ( "
		"select from_line,to_line from next where from_line in (select line_num from statement where entity = '%s')"
		" union "
		"select n.from_line, n.to_line from next n join nextT nT on n.from_line = nT.to_line)"
		" select * from nextT)"
		" intersect "
		"select * from ("
		"with recursive nextT as ("
		"select from_line, to_line from next where to_line = %s"
		" union "
		"select n.from_line, n.to_line from next n join nextT nT on n.to_line = nT.from_line)"
		" select * from nextT)", input1.c_str(), input2.c_str());
	return(sqlBuf);
}

string QueryBuilderSqlNextT::Build_Specific_Any(string input1)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "with recursive nextT as ( "
		"select from_line,to_line from next where from_line = %s"
		" union "
		"select n.from_line, n.to_line from next n join nextT nT on n.from_line = nT.to_line)"
		" select * from nextT", input1.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlNextT::Build_Specific_Specific(string input1, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select * from ("
		"with recursive nextT as ("
		"select from_line, to_line from next where from_line = %s"
		" union "
		"select n.from_line, n.to_line from next n join nextT nT on n.from_line = nT.to_line)"
		"select * from nextT)"
		" intersect "
		"select * from ("
		"with recursive nextT as ("
		"select from_line, to_line from next where to_line = %s"
		" union "
		"select n.from_line, n.to_line from next n join nextT nT on n.to_line = nT.from_line)"
		"select * from nextT)", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlNextT::Build_Specific_Synonym(string line_num, string entity)
{
	char sqlBuf[600];
	sprintf_s(sqlBuf, "select * from ("
		"with recursive nextT as ("
		"select from_line, to_line from next where from_line = %s"
		" union "
		"select n.from_line, n.to_line from next n join nextT nt on n.from_line = nt.to_line where n.to_line)"
		" select * from nextT)"
		" intersect "
		"select * from ("
		"with recursive nextT as ("
		"select from_line, to_line from next where to_line in (select line_num from statement where entity = '%s')"
		" union "
		"select n.from_line, n.to_line from next n join nextT nt on n.to_line = nt.from_line)"
		" select * from nextT)", line_num.c_str(), entity.c_str());
	return string(sqlBuf);
}

QueryBuilderSqlNextT::QueryBuilderSqlNextT(ClRelation re)
{
    _re = re;
}

string QueryBuilderSqlNextT::GetSqlQuery(DescriberClRelation describer)
{
	bool input1IsAny = describer.Input1IsAny();
	bool input2IsAny = describer.Input2IsAny();
	string input1 = _re.GetInput1Unquoted();
	string input2 = _re.GetInput2Unquoted();
	string entityInput1 = describer.EntityInput1();
	string entityInput2 = describer.EntityInput2();

	if (describer.Input1IsAny() && describer.Input2IsAny()) { // Next*(entity/_, entity/_)
		if (describer.Input1IsStmtOrWildcard() && describer.Input2IsStmtOrWildcard()) { // Next*(stmt/_, stmt/_)
			return Build_Any_Any();
		}
		else if (describer.Input1IsStmtOrWildcard() && !describer.Input2IsStmtOrWildcard()) { // Next*(stmt/_, stmtRef_excld_stmt)
			return Build_Any_Synonym(entityInput2);
		}
		else if (!describer.Input1IsStmtOrWildcard() && describer.Input2IsStmtOrWildcard()) { // Next*(stmtRef_excld_stmt, stmt/_)
			return Build_Synonym_Any(entityInput1);
		}
		else if (!describer.Input1IsStmtOrWildcard() && !describer.Input2IsStmtOrWildcard()) { // Next*(stmtRef_excld_stmt, stmtRef_excld_stmt)
			return Build_Synonym_Synonym(entityInput1, entityInput2);
		}
	}
	else if (describer.Input1IsAny() && !describer.Input2IsAny()) { // Next*(entity/_, 10)
		if (describer.Input1IsStmtOrWildcard()) { // Next*(stmt/_, 10)
			return Build_Any_Specific(input2);
		}
		else { // Next*(stmtRef_excld_stmt, 10)
			return Build_Synonym_Specific(entityInput1, input2);
		}
	}
	else if (!describer.Input1IsAny() && describer.Input2IsAny()) { // Next*(10, entity/_)
		if (describer.Input2IsStmtOrWildcard()) { // Next*(10, stmt/_)
			return Build_Specific_Any(input1);
		}
		else { // Next*(10, stmtRef_excld_stmt)
			return Build_Specific_Synonym(input1, entityInput2);
		}
	}
	else if (!describer.Input1IsAny() && !describer.Input2IsAny()) { // Next*(10, 11)
		return Build_Specific_Specific(input1, input2);
	}

    return string();
}
