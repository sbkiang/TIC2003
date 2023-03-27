#pragma once
#include <string>
#include "../SPA/Struct.h"
#include "../source/Database.h"
#include <format>


using namespace std;

class Uses {
public:
	/*
	static bool AssignSynonymSynonym(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool AssignSynonymDefined(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool AssignSynonymAny(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool AssignDefinedDefined(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool AssignDefinedSynonym(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool AssignDefinedAny(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool AssignAnyAny(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool AssignAnyDefined(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	static bool AssignAnySynonym(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific);
	*/

	static bool GetUsesForAssign(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
	static bool GetUsesForPrint(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
	static bool GetUsesForWhile(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
	static bool GetUsesForIf(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
	static bool GetUsesForCall(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
	static bool GetUsesForProcedure(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
	static bool GetUsesForUnknownInput1(string input1, string input2, bool input1IsSpecific, bool input2IsSpecific, SqlResultStore& rs);
};