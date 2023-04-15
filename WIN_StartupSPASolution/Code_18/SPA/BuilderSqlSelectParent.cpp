#include "BuilderSqlSelectParent.h"

string BuilderSqlSelectParent::Build_Any_Any() {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)");
	return string(sqlBuf);
}

// Parent(stmt/_, read/print/assign/while/if/call)
string BuilderSqlSelectParent::Build_Any_Synonym(string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where s.entity = '%s' and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", input2.c_str());
	return string(sqlBuf);
}

// Parent(stmt/_, 10)
string BuilderSqlSelectParent::Build_Any_Specific(string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where s.line_num = %s order by p.line_num desc limit 1", input2.c_str());
	return string(sqlBuf);
}

// Parent(while/if, stmt/_)
string BuilderSqlSelectParent::Build_Synonym_Any(string input1) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num in (select line_num from statement where entity = '%s') and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", input1.c_str());
	return string(sqlBuf);
}

// Parent(while/if, read/print/assign/while/if/call)
string BuilderSqlSelectParent::Build_Synonym_Synonym(string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where s.entity = '%s' and p.line_num in (select line_num from statement where entity = '%s') and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", input2.c_str(), input1.c_str());
	return string(sqlBuf);
}

// Parent(while/if, 10)
string BuilderSqlSelectParent::Build_Synonym_Specific(string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where s.line_num = %s and p.line_num in (select line_num from statement where entity = '%s') and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", input2.c_str(), input1.c_str());
	return string(sqlBuf);
}

// Parent(10, stmt/_)
string BuilderSqlSelectParent::Build_Specific_Any(string input1) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = %s and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", input1.c_str());
	return string(sqlBuf);
}

// Parent(10, read/print/assign/while/if/call)
string BuilderSqlSelectParent::Build_Specific_Synonym(string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = %s and s.entity = '%s' and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

// Parent(10, 10)
string BuilderSqlSelectParent::Build_Specific_Specific(string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num as parent, s.line_num as child from parent p join statement s on s.line_num between p.child_start and p.child_end where p.line_num = %s and s.line_num = %s and p.line_num = (select p.line_num from parent p where s.line_num between p.child_start and p.child_end order by p.line_num desc limit 1)", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

BuilderSqlSelectParent::BuilderSqlSelectParent()
{
}

string BuilderSqlSelectParent::GetSql(IClConstrain& re, IDescriberClConstrain& describer)
{
	bool input1IsAny = describer.Input1IsAny();
	bool input2IsAny = describer.Input2IsAny();
	string input1 = re.GetInput1Unquoted();
	string input2 = re.GetInput2Unquoted();
	string entityInput1 = describer.EntityInput1();
	string entityInput2 = describer.EntityInput2();

	if (describer.Input1IsAny() && describer.Input2IsAny()) { // Parent(entity/_, entity/_)
		if (describer.Input1IsStmtOrWildcard() && describer.Input2IsStmtOrWildcard()) { // Parent(stmt/_, stmt/_)
			return Build_Any_Any();
		}
		else if (describer.Input1IsStmtOrWildcard() && !describer.Input2IsStmtOrWildcard()) { // Parent(stmt/_, entity_excld_stmt)
			return Build_Any_Synonym(entityInput2);
		}
		else if (!describer.Input1IsStmtOrWildcard() && describer.Input2IsStmtOrWildcard()) { // Parent(entity_excld_stmt, stmt/_)
			return Build_Synonym_Any(entityInput1);
		}
		else if (!describer.Input1IsStmtOrWildcard() && !describer.Input2IsStmtOrWildcard()) { // Parent(entity_excld_stmt, entity_excld_stmt)
			return Build_Synonym_Synonym(entityInput1, entityInput2);
		}
	}
	else if (describer.Input1IsAny() && !describer.Input2IsAny()) { // Parent(entity/_, 10)
		if (describer.Input1IsStmtOrWildcard()) { // Parent(stmt/_, 10)
			return Build_Any_Specific(input2);
		}
		else if (!describer.Input1IsStmtOrWildcard()) { // Parent(entity_excld_stmt, 10)
			return Build_Synonym_Specific(entityInput1, input2);
		}
	}
	else if (!describer.Input1IsAny() && describer.Input2IsAny()) { // Parent(10, entity/_)
		if (describer.Input2IsStmtOrWildcard()) { // Parent(10, stmt/_)
			return Build_Specific_Any(input1);
		}
		else if (!describer.Input2IsStmtOrWildcard()) { // Parent(10, entity_excld_stmt)
			return Build_Specific_Synonym(input1, entityInput2);
		}
	}
	else if (!describer.Input1IsAny() && !describer.Input2IsAny()) {// Parent(10,11)
		return Build_Specific_Specific(input1, input2);
	}

    return string();
}
