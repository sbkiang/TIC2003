#include "QueryBuilderSqlCalls.h"

// Call(_,_)
string QueryBuilderSqlCalls::Build_Any_Any() {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select caller, callee from call where direct_call = 1");
	return string(sqlBuf);
}

// Call(_, procedure)  ** procedure is also a wildcard. so, can be treated as Call(_,_)
string QueryBuilderSqlCalls::Build_Any_Synonym() {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select caller, callee from call where direct_call = 1");
	return string(sqlBuf);
}

// Call(_, "Second")
string QueryBuilderSqlCalls::Build_Any_Specific(string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select caller, callee from call where callee = '%s' and direct_call = 1", input2.c_str());
	return string(sqlBuf);
}

// Call(procedure, _)  ** procedure is also a wildcard. so, can be treated as Call(_,_)
string QueryBuilderSqlCalls::Build_Synonym_Any() {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select caller, callee from call where direct_call = 1");
	return string(sqlBuf);
}

// Call("First", _)
string QueryBuilderSqlCalls::Build_Specific_Any(string input1) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select caller, callee from call where caller = '%s' and direct_call = 1", input1.c_str());
	return string(sqlBuf);
}

// Call(procedure, procedure)  ** procedure = any. So, same as Call(_,_)
string QueryBuilderSqlCalls::Build_Synonym_Synonym() {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select caller, callee from call where direct_call = 1");
	return string(sqlBuf);
}

// Call(procedure, "Second")
string QueryBuilderSqlCalls::Build_Synonym_Specific(string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select caller, callee from call where callee = '%s' and direct_call = 1", input2.c_str());
	return string(sqlBuf);
}

// Call("First", procedure)
string QueryBuilderSqlCalls::Build_Specific_Synonym(string input1) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select caller, callee from call where caller = '%s' and direct_call = 1", input1.c_str());
	return string(sqlBuf);
}

// Call("First", "Second")
string QueryBuilderSqlCalls::Build_Specific_Specific(string input1, string input2) {
	char sqlBuf[512] = {};
	sprintf_s(sqlBuf, "select caller, callee from call where caller = '%s' and callee = '%s' and direct_call = 1", input1.c_str(), input2.c_str());
	return string(sqlBuf);
}

QueryBuilderSqlCalls::QueryBuilderSqlCalls(RelEnt re) {
	_re = re;
}

string QueryBuilderSqlCalls::GetSqlQuery(RelEntDescriber red)
{
	bool input1IsAny = red.Input1IsAny();
	bool input2IsAny = red.Input2IsAny();
	string input1 = _re.GetInput1Unquoted();
	string input2 = _re.GetInput2Unquoted();
	string entityInput1 = red.EntityInput1();
	string entityInput2 = red.EntityInput2();

	if (red.Input1IsAny() && red.Input2IsAny()) { // Call(procedure/_, procedure/_)
		if (red.Input1IsStmtOrWildcard() && red.Input2IsStmtOrWildcard()) { // Call(_,_)
			return Build_Any_Any();
		}
		else if (red.Input1IsStmtOrWildcard() && !red.Input2IsStmtOrWildcard()) { // Call(_, procedure)
			return Build_Any_Synonym();
		}
		else if (!red.Input1IsStmtOrWildcard() && red.Input2IsStmtOrWildcard()) { // Call(procedure, _)
			return Build_Synonym_Any();
		}
		else if (!red.Input1IsStmtOrWildcard() && !red.Input2IsStmtOrWildcard()) { // Call(procedure, procedure)
			return Build_Synonym_Synonym();
		}
	}
	else if (red.Input1IsAny() && !red.Input2IsAny()) { // Call(procedure/_, "Second")
		if (red.Input1IsStmtOrWildcard()) { // Call(_, "Second")
			return Build_Any_Specific(input2);
		}
		else { // Call(procedure, "Second")
			return Build_Synonym_Specific(input2);
		}
	}
	else if (!red.Input1IsAny() && red.Input2IsAny()) { // Call("First", procedure/_)
		if (red.Input2IsStmtOrWildcard()) { // Call("First", _)
			return Build_Specific_Any(input1);
		}
		else { // Call("First", procedure)
			return Build_Specific_Synonym(input1);
		}
	}
	else if (!red.Input1IsAny() && !red.Input2IsAny()) { // Call("First", "Second")
		return Build_Specific_Specific(input1, input2);
	}
    return string();
}
