#pragma once
#include "IBuilderSqlConstrain.h"
#include <regex>

class BuilderSqlColumnModifies : public IBuilderSqlConstrain {
private:
	// Stmt - Name
	string Build_StmtSynonym_NameSynonym(string syn1, string syn2);
	string Build_StmtSynonym_NameNotSynonym(string syn1);
	string Build_StmtNotSynonym_NameSynonym(string syn2);
	string Build_StmtNotSynonym_NameNotSynonym();

	// Name - Name
	string Build_NameSynonym_NameSynonym(string input1, string input2);
	string Build_NameSynonym_NameNotSynonym(string input1);
	string Build_NameNotSynonym_NameSynonym(string input2);
	string Build_NameNotSynonym_NameNotSynonym();
public:
	BuilderSqlColumnModifies();
	string GetSql(IClConstrain& re, IDescriberClConstrain& describer);
};