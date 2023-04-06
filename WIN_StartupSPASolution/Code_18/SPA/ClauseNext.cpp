#include "ClauseNext.h"

string Next::insertNext(int from, int to)
{
	char sqlBuf[256];
	sprintf_s(sqlBuf, "INSERT INTO next ('from_line','to_line') VALUES (%i,%i);", from, to);
	return string(sqlBuf);
}

/*
string Next::NextConstructSelect_Synonym_Synonym(string synonym1, string synonym2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select distinct from_line as %s, to_line as %s", synonym1.c_str(), synonym2.c_str());
	return string(sqlBuf);
}

string Next::NextConstructSelect_Synonym_NotSynonym(string synonym1)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select distinct from_line as %s", synonym1.c_str());
	return string(sqlBuf);
}

string Next::NextConstructSelect_NotSynonym_Synonym(string synonym2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select distinct to_line as %s", synonym2.c_str());
	return string(sqlBuf);
}

string Next::NextConstructSelect_NotSynonym_NotSynonym()
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "select distinct from_line, to_line");
	return string(sqlBuf);
}
*/

string Next::NextConstructQuery_Any_Any(string frontSql)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from (select * from next)", frontSql.c_str());
	return string(sqlBuf);
}

string Next::NextConstructQuery_Any_Specific(string frontSql, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from (select * from next where to_line = %s)", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Next::NextConstructQuery_Any_Synonym(string frontSql, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from (select * from next where to_line in (select line_num from statement where entity = '%s')", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Next::NextConstructQuery_Synonym_Any(string frontSql, string input1)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from (select * from next where from_line in (select line_num from statement where entity = '%s')", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Next::NextConstructQuery_Synonym_Synonym(string frontSql, string input1, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from (select * from next where from_line in (select line_num from statement s where s.entity = '%s') and to_line in (select line_num from statement where entity = '%s')", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Next::NextConstructQuery_Synonym_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from (select * from next where from_line in (select line_num from statement where entity = '%s') and to_line = %s", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Next::NextConstructQuery_Specific_Any(string frontSql, string input1)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from (select * from next where from_line = %s)", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Next::NextConstructQuery_Specific_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from (select * from next where from_line = %s and to_line = %s)", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Next::NextConstructQuery_Specific_Synonym(string frontSql, string input1, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from (select * from next where from_line = %s and to_line in (select line_num from statement where entity = '%s')", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Next::NextTConstructQuery_Any_Any(string frontSql)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from ("
		"with recursive nextT as ("
		"select from_line, to_line from next"
		" union "
		"select n.from_line, n.to_line from next n join nextT nT where n.from_line = nT.to_line)"
		" select * from nextT)", frontSql.c_str());
	return string(sqlBuf);
}

string Next::NextTConstructQuery_Any_Specific(string frontSql, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from ("
		"with recursive nextT as ("
		"select from_line, to_line from next where to_line = %s"
		" union "
		"select n.from_line, n.to_line from next n join nextT nT on n.to_line = nT.from_line)"
		" select * from nextT)", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Next::NextTConstructQuery_Any_Synonym(string frontSql, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from ("
		"with recursive nextT as ("
		"select from_line, to_line from next where to_line in (select line_num from statement where entity = '%s')"
		" union "
		"select n.from_line, n.to_line from next n join nextT nt on n.to_line = nt.from_line)"
		" select * from nextT)", frontSql.c_str(), input2.c_str());
		//" select * from nextT where to_line in (select line_num from statement where entity = '%s'))", frontSql.c_str(), input2.c_str());
	return string(sqlBuf);
}
	
string Next::NextTConstructQuery_Synonym_Any(string frontSql, string input1)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from ("
		"with recursive nextT as ("
		"select from_line, to_line from next where from_line in (select line_num from statement where entity = '%s')"
		" union "
		"select n.from_line, n.to_line from next n join nextT nT on n.from_line = nT.to_line)"
		" select * from nextT)", frontSql.c_str(), input1.c_str());
		//" select * from nextT from_line in (select line_num from statement where entity = '%s'))", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Next::NextTConstructQuery_Synonym_Synonym(string frontSql, string input1, string input2)
{
	char sqlBuf[600];
	sprintf_s(sqlBuf, "%s from ("
		"select * from ("
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
		" select * from nextT))", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Next::NextTConstructQuery_Synonym_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from ("
		"select * from ("
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
		" select * from nextT))", frontSql.c_str(), input1.c_str(), input2.c_str());
	return(sqlBuf);
}

string Next::NextTConstructQuery_Specific_Any(string frontSql, string input1)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from ("
		"with recursive nextT as ( "
		"select from_line,to_line from next where from_line = %s"
		" union "
		"select n.from_line, n.to_line from next n join nextT nT on n.from_line = nT.to_line)"
		" select * from nextT)", frontSql.c_str(), input1.c_str());
	return string(sqlBuf);
}

string Next::NextTConstructQuery_Specific_Specific(string frontSql, string input1, string input2)
{
	char sqlBuf[512];
	sprintf_s(sqlBuf, "%s from ("
		"select * from ("
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
		"select * from nextT))", frontSql.c_str(), input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string Next::NextTConstructQuery_Specific_Synonym(string frontSql, string line_num, string entity)
{
	char sqlBuf[600];
	sprintf_s(sqlBuf, "%s from ("
		"select * from ("
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
		" select * from nextT))", frontSql.c_str(), line_num.c_str(), entity.c_str());
	return string(sqlBuf);
}
