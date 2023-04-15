#include "BuilderSqlSelectCallsT.h"

// Call*(_,_)
string BuilderSqlSelectCallsT::Build_Any_Any() {
	char Buf[512] = {};
	sprintf_s(Buf, "with recursive callT as ("
		" select caller, callee from call"
		" union"
		" select ct.caller, c.callee"
		" from callT cT"
		" join call c on c.caller = cT.callee)"
		" select * from callT");
	return string(Buf);
}

// Call*(_, procedure)
string BuilderSqlSelectCallsT::Build_Any_Synonym() {
	char Buf[512] = {};
	sprintf_s(Buf, "with recursive callT as ("
		" select caller, callee from call"
		" union"
		" select ct.caller, c.callee"
		" from callT cT"
		" join call c on c.caller = cT.callee)"
		" select * from callT");
	return string(Buf);
}

// Call*(_, "Second")
string BuilderSqlSelectCallsT::Build_Any_Specific(string input2) {
	char Buf[512] = {};
	sprintf_s(Buf, "with recursive callT as ("
		" select caller, callee from call where callee = '%s'"
		" union"
		" select c.caller, cT.callee"
		" from callT ct"
		" join call c on cT.caller = c.callee)"
		" select * from callT", input2.c_str());
	return string(Buf);
}

// Call*(procedure, _)
string BuilderSqlSelectCallsT::Build_Synonym_Any() {
	char Buf[512] = {};
	sprintf_s(Buf, "with recursive callT as ("
		" select caller, callee from call"
		" union"
		" select ct.caller, c.callee"
		" from callT cT"
		" join call c on c.caller = cT.callee)"
		" select * from callT");
	return string(Buf);
}

// Call*("First", _)
string BuilderSqlSelectCallsT::Build_Specific_Any(string input1) {
	char Buf[512] = {};
	sprintf_s(Buf, "with recursive callT as ("
		" select caller, callee from call where caller = '%s'"
		" union"
		" select ct.caller, c.callee"
		" from callT cT"
		" join call c on c.caller = cT.callee)"
		" select * from callT", input1.c_str());
	return string(Buf);
}

// Call*(procedure, procedure)
string BuilderSqlSelectCallsT::Build_Synonym_Synonym() {
	char Buf[512] = {};
	sprintf_s(Buf, "with recursive callT as ("
		" select caller, callee from call"
		" union"
		" select ct.caller, c.callee"
		" from callT cT"
		" join call c on c.caller = cT.callee)"
		" select * from callT");
	return string(Buf);
}

// Call*(procedure, "Second")
string BuilderSqlSelectCallsT::Build_Synonym_Specific(string input2) {
	char Buf[512] = {};
	sprintf_s(Buf, "with recursive callT as ("
		" select caller, callee from call where callee = '%s'"
		" union"
		" select c.caller, cT.callee"
		" from callT ct"
		" join call c on cT.caller = c.callee)"
		" select * from callT", input2.c_str());
	return string(Buf);
}

// Call*("First", procedure)
string BuilderSqlSelectCallsT::Build_Specific_Synonym(string input1) {
	char Buf[512] = {};
	sprintf_s(Buf, "with recursive callT as ("
		" select caller, callee from call where caller = '%s'"
		" union"
		" select ct.caller, c.callee"
		" from callT cT"
		" join call c on c.caller = cT.callee)"
		" select * from callT", input1.c_str());
	return string(Buf);
}

// Call*("First", "Second")
string BuilderSqlSelectCallsT::Build_Specific_Specific(string input1, string input2) {
	char Buf[512] = {};
	sprintf_s(Buf, "select * from"
		" (select * from"
		" (with recursive callT as ("
		" selecT caller, callee from call where caller = '%s'"
		" union"
		" selecT cT.caller, c.callee"
		" from callT cT"
		" join call c on c.caller = cT.callee)"
		" selecT * from callT)"
		" intersect"
		" select * from"
		" (with recursive callT as ("
		" selecT caller, callee from call where callee = '%s'"
		" union"
		" selecT c.caller, cT.callee"
		" from callT cT"
		" join call c on cT.caller = c.callee)"
		" selecT * from callT))", input1.c_str(), input2.c_str());
	return string(Buf);
}

BuilderSqlSelectCallsT::BuilderSqlSelectCallsT() {
}

string BuilderSqlSelectCallsT::GetSql(IClConstrain& re, IDescriberClConstrain& describer)
{
	bool input1IsAny = describer.Input1IsAny();
	bool input2IsAny = describer.Input2IsAny();
	string input1 = re.GetInput1Unquoted();
	string input2 = re.GetInput2Unquoted();
	string entityInput1 = describer.EntityInput1();
	string entityInput2 = describer.EntityInput2();

	if (describer.Input1IsAny() && describer.Input2IsAny()) { // Call*(procedure/_, procedure/_)
		if (describer.Input1IsStmtOrWildcard() && describer.Input2IsStmtOrWildcard()) { // Call*(_,_)
			return Build_Any_Any();
		}
		else if (describer.Input1IsStmtOrWildcard() && !describer.Input2IsStmtOrWildcard()) { // Call*(_, procedure)
			return Build_Any_Synonym();
		}
		else if (!describer.Input1IsStmtOrWildcard() && describer.Input2IsStmtOrWildcard()) { // Call*(procedure, _)
			return Build_Synonym_Any();
		}
		else if (!describer.Input1IsStmtOrWildcard() && !describer.Input2IsStmtOrWildcard()) { // Call*(procedure, procedure)
			return Build_Synonym_Synonym();
		}
	}
	else if (describer.Input1IsAny() && !describer.Input2IsAny()) { // Call*(procedure/_, "Second")
		if (describer.Input1IsStmtOrWildcard()) { // Call*(_, "Second")
			return Build_Any_Specific(input2);
		}
		else { // Call*(procedure, "Second")
			return Build_Synonym_Specific(input2);
		}
	}
	else if (!describer.Input1IsAny() && describer.Input2IsAny()) { // Call*("First", procedure/_)
		if (describer.Input2IsStmtOrWildcard()) { // Call*("First", _)
			return Build_Specific_Any(input1);
		}
		else { // Call*("First", procedure)
			return Build_Specific_Synonym(input1);
		}
	}
	else if (!describer.Input1IsAny() && !describer.Input2IsAny()) { // Call*("First", "Second")
		return Build_Specific_Specific(input1, input2);
	}

    return string();
}
