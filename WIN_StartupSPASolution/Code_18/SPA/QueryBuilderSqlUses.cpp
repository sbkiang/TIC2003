#include "QueryBuilderSqlUses.h"

string QueryBuilderSqlUses::Build_AnyPrintAssign_Any(string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select u.line_num, u.variable_name from use u join statement s on s.line_num = u.line_num where s.entity = '%s'", input1.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_AnyCall_Any()
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select s.line_num, u.variable_name from statement s join use u on u.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.entity = 'call'");
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_AnyProcedure_Any()
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.name, u.variable_name from procedure p join use u on u.line_num between p.start and p.end");
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_AnyWhileIf_Any(string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num, u.variable_name from parent p join statement s on s.line_num = p.line_num join use u on u.line_num between p.line_num and p.child_end where s.entity = '%s'", input1.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_Any_Any()
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num, u.variable_name from parent p join use u on u.line_num between p.line_num and p.child_end union select line_num, variable_name from use");
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_AnyPrintAssign_Specific(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select u.line_num, u.variable_name from use u join statement s on s.line_num = u.line_num where s.entity = '%s' and u.variable_name = '%s'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_AnyCall_Specific(string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select s.line_num, u.variable_name from statement s join use u on u.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.entity = 'call' and u.variable_name = '%s'", input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_AnyProcedure_Specific(string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.name, u.variable_name from procedure p join use u on u.line_num between p.start and p.end where u.variable_name = '%s'", input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_AnyWhileIf_Specific(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num, u.variable_name from parent p join statement s on s.line_num = p.line_num join use u on u.line_num between p.line_num and p.child_end where s.entity = '%s' and u.variable_name = '%s'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_Any_Specific(string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num, u.variable_name from parent p join use u on u.line_num between p.line_num and p.child_end and u.variable_name = '%s' union select line_num, variable_name from use where variable_name = '%s'", input2.c_str(), input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_SpecificPrintAssign_Any(string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select u.line_num, u.variable_name from use u join statement s on s.line_num = u.line_num where s.line_num = %s", input1.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_SpecificCall_Any(string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select s.line_num, u.variable_name from statement s join use u on u.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.line_num = %s", input1.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_SpecificProcedure_Any(string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.name, u.variable_name from procedure p join use u on u.line_num between p.start and p.end where p.name = '%s'", input1.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_SpecificWhileIf_Any(string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num, u.variable_name from parent p join use u on u.line_num between p.line_num and p.child_end where p.line_num = %s", input1.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_SpecificPrintAssign_Specific(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select u.line_num, u.variable_name from use u join statement s on s.line_num = u.line_num where s.line_num = %s and u.variable_name = '%s'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_SpecificCall_Specific(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select s.line_num, u.variable_name from statement s join use u on u.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.line_num = %s where u.variable_name = '%s'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string QueryBuilderSqlUses::Build_SpecificProcedure_Specific(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.name, u.variable_name from procedure p join use u on u.line_num between p.start and p.end where p.name = '%s' and u.variable_name = '%s'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}


string QueryBuilderSqlUses::Build_SpecificWhileIf_Specific(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num, u.variable_name from parent p join use u on u.line_num between p.line_num and p.child_end where p.line_num = %s and u.variable_name = '%s'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

QueryBuilderSqlUses::QueryBuilderSqlUses(RelEnt re)
{
	_re = re;
}

string QueryBuilderSqlUses::GetSqlQuery(RelEntDescriber red) {
	bool input1IsAny = red.Input1IsAny();
	bool input2IsAny = red.Input2IsAny();
	string input1 = _re.GetInput1Unquoted();
	string input2 = _re.GetInput2Unquoted();
	string entityInput1 = red.EntityInput1();
	string entityInput2 = red.EntityInput2();

	if (entityInput1.empty()) {
		if (isdigit(input1[0])) { // for stmt num, we need to get the input1's entity first
			entityInput1 = HelperFunction::GetEntityByStatement(input1);
		}
		else if (isalpha(input1[0])) {
			entityInput1 = "procedure";
		}
	}

	if (input1IsAny && input2IsAny) { // Uses(entity, variable/_)
		if (entityInput1 == "procedure") { // Uses(procedure, variable/_)
			return Build_AnyProcedure_Any();
		}
		else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Uses(print/assign, variable/_)
			return Build_AnyPrintAssign_Any(entityInput1);
		}
		else if (regex_match(entityInput1, regex(regexWhileIf))) { // Uses(while/if, variable/_)
			return Build_AnyWhileIf_Any(entityInput1);
		}
		else if (entityInput1 == "call") { // Uses(call, variable/_)
			return Build_AnyCall_Any();
		}
		else if (red.Input1IsStmtOrWildcard()) { // Uses(stmt/_, variable/_)
			return Build_Any_Any();
		}
	}
	else if (!input1IsAny && input2IsAny) { // entityInput1 here is already determine at the top
		if (entityInput1 == "procedure") { // Uses(procedure, variable/_)
			return Build_SpecificProcedure_Any(input1);
		}
		else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Uses(print/assign, variable/_)
			return Build_SpecificPrintAssign_Any(input1);
		}
		else if (regex_match(entityInput1, regex(regexWhileIf))) {
			return Build_SpecificWhileIf_Any(input1);
		}
		else if (entityInput1 == "call") {
			return Build_SpecificCall_Any(input1);
		}
	}
	else if (input1IsAny && !input2IsAny) {
		if (entityInput1 == "procedure") { // Uses(procedure, variable/_)
			return Build_AnyProcedure_Specific(input2);
		}
		else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Uses(print/assign, variable/_)
			return Build_AnyPrintAssign_Specific(entityInput1, input2);
		}
		else if (regex_match(entityInput1, regex(regexWhileIf))) {
			return Build_AnyWhileIf_Specific(entityInput1, input2);
		}
		else if (entityInput1 == "call") {
			return Build_AnyCall_Specific(input2);
		}
		else if (red.Input1IsStmtOrWildcard()) {
			return Build_Any_Specific(input2);
		}
	}
	else if (!input1IsAny && !input2IsAny) { // entityInput1 here is already determine at the top
		if (entityInput1 == "procedure") { // Uses("main", "x")
			return Build_SpecificProcedure_Specific(input1, input2);
		}
		else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Uses(print/assign stmt, "x")
			return Build_SpecificPrintAssign_Specific(input1, input2);
		}
		else if (regex_match(entityInput1, regex(regexWhileIf))) { // Uses(while/if stmt, "x")
			return Build_SpecificWhileIf_Specific(input1, input2);
		}
		else if (entityInput1 == "call") { // Uses(call stmt, "x")
			return Build_SpecificCall_Specific(input1, input2);
		}
	}
	return string();
}