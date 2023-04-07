#include "QueryBuilderSqlParent.h"

string QueryBuilderSqlParent::Build_Any_Any() {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, c.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)");
	return string(sqlBuf);
}

// Parent(stmt/_, read/print/assign/while/if/call)
string QueryBuilderSqlParent::Build_Any_Synonym(string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, c.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end and s.entity = '%s' and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", input2.c_str());
	return string(sqlBuf);
}

// Parent(stmt/_, 10)
string QueryBuilderSqlParent::Build_Any_Specific(string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, c.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end and s.line_num = %s order by p.line_num desc limit 1", input2.c_str());
	return string(sqlBuf);
}

// Parent(while/if, stmt/_)
string QueryBuilderSqlParent::Build_Synonym_Any(string input1) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, c.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num in (select line_num from statement where entity = '%s') and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", input1.c_str());
	return string(sqlBuf);
}

// Parent(while/if, read/print/assign/while/if/call)
string QueryBuilderSqlParent::Build_Synonym_Synonym(string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, c.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where s.entity = '%s' and p.line_num in (select line_num from statement where entity = '%s') and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", input2.c_str(), input1.c_str());
	return string(sqlBuf);
}

// Parent(while/if, 10)
string QueryBuilderSqlParent::Build_Synonym_Specific(string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, c.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where s.line_num = %s and p.line_num in (select line_num from statement where entity = '%s') and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", input2.c_str(), input1.c_str());
	return string(sqlBuf);
}

// Parent(10, stmt/_)
string QueryBuilderSqlParent::Build_Specific_Any(string input1) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, c.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = %s and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", input1.c_str());
	return string(sqlBuf);
}

// Parent(10, read/print/assign/while/if/call)
string QueryBuilderSqlParent::Build_Specific_Synonym(string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, c.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = %s and s.entity = '%s' and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

// Parent(10, 10)
string QueryBuilderSqlParent::Build_Specific_Specific(string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, c.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end and p.line_num = %s and s.line_num = %s", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

QueryBuilderSqlParent::QueryBuilderSqlParent(RelEnt re)
{
    _re = re;
}

string QueryBuilderSqlParent::GetSqlQuery(RelEntDescriber red)
{
	bool input1IsAny = red.Input1IsAny();
	bool input2IsAny = red.Input2IsAny();
	string input1 = _re.GetInput1Unquoted();
	string input2 = _re.GetInput2Unquoted();
	string entityInput1 = red.EntityInput1();
	string entityInput2 = red.EntityInput2();

	if (red.Input1IsAny() && red.Input2IsSyn()) { // Parent(entity/_, entity/_)
		if (red.Input1IsStmtOrWildcard() && red.Input2IsStmtOrWildcard()) { // Parent(stmt/_, stmt/_)
			return Build_Any_Any();
		}
		else if (red.Input1IsStmtOrWildcard() && !red.Input2IsStmtOrWildcard()) { // Parent(stmt/_, entity_excld_stmt)
			return Build_Any_Synonym(entityInput2);
		}
		else if (!red.Input1IsStmtOrWildcard() && red.Input2IsStmtOrWildcard()) { // Parent(entity_excld_stmt, stmt/_)
			return Build_Synonym_Any(entityInput1);
		}
		else if (!red.Input1IsStmtOrWildcard() && !red.Input2IsStmtOrWildcard()) { // Parent(entity_excld_stmt, entity_excld_stmt)
			return Build_Synonym_Synonym(entityInput1, entityInput2);
		}
	}
	else if (red.Input1IsAny() && !red.Input2IsSyn()) { // Parent(entity/_, 10)
		if (red.Input1IsStmtOrWildcard()) { // Parent(stmt/_, 10)
			return Build_Any_Specific(input2);
		}
		else if (!red.Input1IsStmtOrWildcard()) { // Parent(entity_excld_stmt, 10)
			return Build_Synonym_Specific(entityInput1, input2);
		}
	}
	else if (!red.Input1IsAny() && red.Input2IsSyn()) { // Parent(10, entity/_)
		if (red.Input2IsStmtOrWildcard()) { // Parent(10, stmt/_)
			return Build_Specific_Any(input1);
		}
		else if (!red.Input2IsStmtOrWildcard()) { // Parent(10, entity_excld_stmt)
			return Build_Specific_Synonym(input1, entityInput2);
		}
	}
	else if (!red.Input1IsAny() && !red.Input2IsSyn()) {// Parent(10,11)
		return Build_Specific_Specific(input1, input2);
	}

    return string();
}
