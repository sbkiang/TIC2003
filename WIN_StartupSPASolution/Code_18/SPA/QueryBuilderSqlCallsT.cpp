#include "QueryBuilderSqlCallsT.h"

// Call*(_,_)
string QueryBuilderSqlCallsT::Build_Any_Any() {
	char Buf[512] = {};
	sprintf_s(Buf, "select caller, callee from call");
	return string(Buf);
}

// Call*(_, procedure)
string QueryBuilderSqlCallsT::Build_Any_Synonym() {
	char Buf[512] = {};
	sprintf_s(Buf, "select caller, callee from call");
	return string(Buf);
}

// Call*(_, "Second")
string QueryBuilderSqlCallsT::Build_Any_Specific(string input2) {
	char Buf[512] = {};
	sprintf_s(Buf, "select caller, callee from call where callee = '%s'", input2.c_str());
	return string(Buf);
}

// Call*(procedure, _)
string QueryBuilderSqlCallsT::Build_Synonym_Any() {
	char Buf[512] = {};
	sprintf_s(Buf, "select caller, callee from call");
	return string(Buf);
}

// Call*("First", _)
string QueryBuilderSqlCallsT::Build_Specific_Any(string input1) {
	char Buf[512] = {};
	sprintf_s(Buf, "select caller, callee from call where caller = '%s'", input1.c_str());
	return string(Buf);
}

// Call*(procedure, procedure)
string QueryBuilderSqlCallsT::Build_Synonym_Synonym() {
	char Buf[512] = {};
	sprintf_s(Buf, "select caller, callee from call");
	return string(Buf);
}

// Call*(procedure, "Second")
string QueryBuilderSqlCallsT::Build_Synonym_Specific(string input2) {
	char Buf[512] = {};
	sprintf_s(Buf, "select caller, callee from call where callee = '%s'", input2.c_str());
	return string(Buf);
}

// Call*("First", procedure)
string QueryBuilderSqlCallsT::Build_Specific_Synonym(string input1) {
	char Buf[512] = {};
	sprintf_s(Buf, "select caller, callee from call where caller = '%s'", input1.c_str());
	return string(Buf);
}

// Call*("First", "Second")
string QueryBuilderSqlCallsT::Build_Specific_Specific(string input1, string input2) {
	char Buf[512] = {};
	sprintf_s(Buf, "select caller, callee from from call where caller = '%s' and callee = '%s'", input1.c_str(), input2.c_str());
	return string(Buf);
}

QueryBuilderSqlCallsT::QueryBuilderSqlCallsT(RelEnt re)
{
	re = _re;
}

string QueryBuilderSqlCallsT::GetSqlQuery(RelEntDescriber red)
{
	bool input1IsAny = red.Input1IsAny();
	bool input2IsAny = red.Input2IsAny();
	string input1 = _re.GetInput1Unquoted();
	string input2 = _re.GetInput2Unquoted();
	string entityInput1 = red.EntityInput1();
	string entityInput2 = red.EntityInput2();

	if (red.Input1IsAny() && red.Input2IsAny()) { // Call*(procedure/_, procedure/_)
		if (red.Input1IsStmtOrWildcard() && red.Input2IsStmtOrWildcard()) { // Call*(_,_)
			return Build_Any_Any();
		}
		else if (red.Input1IsStmtOrWildcard() && !red.Input2IsStmtOrWildcard()) { // Call*(_, procedure)
			return Build_Any_Synonym();
		}
		else if (!red.Input1IsStmtOrWildcard() && red.Input2IsStmtOrWildcard()) { // Call*(procedure, _)
			return Build_Synonym_Any();
		}
		else if (!red.Input1IsStmtOrWildcard() && !red.Input2IsStmtOrWildcard()) { // Call*(procedure, procedure)
			return Build_Synonym_Synonym();
		}
	}
	else if (red.Input1IsAny() && !red.Input2IsAny()) { // Call*(procedure/_, "Second")
		if (red.Input1IsStmtOrWildcard()) { // Call*(_, "Second")
			return Build_Any_Specific(input2);
		}
		else { // Call*(procedure, "Second")
			return Build_Synonym_Specific(input2);
		}
	}
	else if (!red.Input1IsAny() && red.Input2IsAny()) { // Call*("First", procedure/_)
		if (red.Input2IsStmtOrWildcard()) { // Call*("First", _)
			return Build_Specific_Any(input1);
		}
		else { // Call*("First", procedure)
			return Build_Specific_Synonym(input1);
		}
	}
	else if (!red.Input1IsAny() && !red.Input2IsAny()) { // Call*("First", "Second")
		return Build_Specific_Specific(input1, input2);
	}

    return string();
}
