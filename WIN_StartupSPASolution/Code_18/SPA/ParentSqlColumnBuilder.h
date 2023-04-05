#pragma once
#include "IRelRefSqlColumnBuilder.h"

class ParentSqlColumnBuilder : public IRelRefSqlColumnBuilder{
private:
	string _syn1 = "";
	string _syn2 = "";
	string Build_Synonym_Synonym(string syn1, string syn2);
	string Build_Synonym_NotSynonym(string syn1);
	string Build_NotSynonym_Synonym(string syn2);
	string Build_NotSynonym_NotSynonym();
public:
	ParentSqlColumnBuilder(string syn1, string syn2) : IRelRefSqlColumnBuilder(syn1, syn2) {}
	string getSqlColumnQuery();
};