#include "BuilderSqlSelectModifies.h"

string BuilderSqlSelectModifies::Build_AnyReadAssign_Any(string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select m.line_num, m.variable_name from modify m join statement s on s.line_num = m.line_num where s.entity = '%s'", input1.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_AnyCall_Any()
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select s.line_num, m.variable_name from statement s join modify m on m.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.entity = 'call'");
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_AnyProcedure_Any()
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.name, m.variable_name from procedure p join modify m on m.line_num between p.start and p.end");
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_AnyWhileIf_Any(string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num, m.variable_name from parent p join statement s on s.line_num = p.line_num join modify m on m.line_num between p.line_num and p.child_end where s.entity = '%s'", input1.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_Any_Any()
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num, m.variable_name from parent p join modify m on m.line_num between p.line_num and p.child_end union select line_num, variable_name from modify");
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_AnyReadAssign_Specific(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select m.line_num, m.variable_name from modify m join statement s on s.line_num = m.line_num where s.entity = '%s' and m.variable_name = '%s'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_AnyCall_Specific(string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select s.line_num, m.variable_name from statement s join modify m on m.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.entity = 'call' and m.variable_name = '%s'", input2.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_AnyProcedure_Specific(string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.name, m.variable_name from procedure p join modify m on m.line_num between p.start and p.end where m.variable_name = '%s'", input2.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_AnyWhileIf_Specific(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num, m.variable_name from parent p join statement s on s.line_num = p.line_num join modify m on m.line_num between p.line_num and p.child_end where s.entity = '%s' and m.variable_name = '%s'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_Any_Specific(string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num, m.variable_name from parent p join modify m on m.line_num between p.line_num and p.child_end where m.variable_name = '%s' union select line_num, variable_name from modify where variable_name = '%s'", input2.c_str(), input2.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_SpecificReadAssign_Any(string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select m.line_num, m.variable_name from modify m join statement s on s.line_num = m.line_num where s.line_num = %s", input1.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_SpecificCall_Any(string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select s.line_num, m.variable from statement s join modify m on m.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.line_num = %s", input1.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_SpecificProcedure_Any(string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.name, m.variable_name from procedure p join modify m on m.line_num between p.start and p.end where p.name = '%s'", input1.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_SpecificWhileIf_Any(string input1)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num, m.variable_name from parent p join modify m on m.line_num between p.line_num and p.child_end where p.line_num = %s", input1.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_SpecificReadAssign_Specific(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select m.line_num, m.variable_name from modify m join statement s on s.line_num = m.line_num where s.line_num = %s and m.variable_name = '%s'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_SpecificCall_Specific(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select s.line_num, m.variable_name from statement s join modify m on m.line_num between (select start from procedure where name = s.text) and (select end from procedure where name = s.text) where s.line_num = %s and m.variable_name = '%s'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

string BuilderSqlSelectModifies::Build_SpecificProcedure_Specific(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num, m.variable_name from procedure p join modify m on m.line_num between p.start and p.end where p.name = '%s' and m.variable_name = '%s'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}


string BuilderSqlSelectModifies::Build_SpecificWhileIf_Specific(string input1, string input2)
{
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select p.line_num, m.variable_name from parent p join modify m on m.line_num between p.line_num and p.child_end where p.line_num = %s and m.variable_name = '%s'", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

BuilderSqlSelectModifies::BuilderSqlSelectModifies()
{
}

string BuilderSqlSelectModifies::GetSql(IClConstrain& re, IDescriberClConstrain& describer)
{
	bool input1IsAny = describer.Input1IsAny();
	bool input2IsAny = describer.Input2IsAny();
	string input1 = re.GetInput1Unquoted();
	string input2 = re.GetInput2Unquoted();
	string entityInput1 = describer.EntityInput1();

	if (entityInput1.empty()) {
		if (isdigit(input1[0])) { // for stmt num, we need to get the input1's entity first
			entityInput1 = HelperFunction::GetEntityByStatement(input1);
		}
		else if (isalpha(input1[0])) {
			entityInput1 = "procedure";
		}
	}

	if (input1IsAny && input2IsAny) { // Modifies(entity, variable/_)
		if (entityInput1 == "procedure") { // Modifies(procedure, variable/_)
			return Build_AnyProcedure_Any();
		}
		else if (regex_match(entityInput1, regex(regexAssignRead))) { // Modifies(print/assign, variable/_)
			return Build_AnyReadAssign_Any(entityInput1);
		}
		else if (regex_match(entityInput1, regex(regexWhileIf))) {
			return Build_AnyWhileIf_Any(entityInput1);
		}
		else if (entityInput1 == "call") {
			return Build_AnyCall_Any();
		}
		else if (describer.Input1IsStmtOrWildcard()) {
			return Build_Any_Any();
		}
	}
	else if (!input1IsAny && input2IsAny) { // entityInput1 here is already determine at the top
		if (entityInput1 == "procedure") { // Modifies(procedure, variable/_)
			return Build_SpecificProcedure_Any(input1);
		}
		else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Modifies(print/assign, variable/_)
			return Build_SpecificReadAssign_Any(input1);
		}
		else if (regex_match(entityInput1, regex(regexWhileIf))) {
			return Build_SpecificWhileIf_Any(input1);
		}
		else if (entityInput1 == "call") {
			return Build_SpecificCall_Any(input1);
		}
	}
	else if (input1IsAny && !input2IsAny) {
		if (entityInput1 == "procedure") { // Modifies(procedure, variable/_)
			return Build_AnyProcedure_Specific(input2);
		}
		else if (regex_match(entityInput1, regex(regexAssignRead))) { // Modifies(print/assign, variable/_)
			return Build_AnyReadAssign_Specific(entityInput1, input2);
		}
		else if (regex_match(entityInput1, regex(regexWhileIf))) {
			return Build_AnyWhileIf_Specific(entityInput1, input2);
		}
		else if (entityInput1 == "call") {
			return Build_AnyCall_Specific(input2);
		}
		else if (describer.Input1IsStmtOrWildcard()) {
			return Build_Any_Specific(input2);
		}
	}
	else if (!input1IsAny && !input2IsAny) { // entityInput1 here is already determine at the top
		if (isalpha(input1[0])) { // Modifies("main", "x")
			return Build_SpecificProcedure_Specific(input1, input2);
		}
		else if (regex_match(entityInput1, regex(regexAssignPrint))) { // Modifies(print/assign stmt, "x")
			return Build_SpecificReadAssign_Specific(input1, input2);
		}
		else if (regex_match(entityInput1, regex(regexWhileIf))) { // Modifies(while/if stmt, "x")
			return Build_SpecificWhileIf_Specific(input1, input2);
		}
		else if (entityInput1 == "call") { // Modifies(call stmt, "x")
			return Build_SpecificCall_Specific(input1, input2);
		}
	}

	return string();
}
