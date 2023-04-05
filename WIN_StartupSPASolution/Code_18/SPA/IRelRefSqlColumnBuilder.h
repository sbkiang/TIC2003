#pragma once
#include <string>

//Interface class for all the relationship's SelectBuilders (the part where we construct the sql SELECT query to match the col name)

using namespace std;

class IRelRefSqlColumnBuilder {
private:
	string _syn1;
	string _syn2;
	virtual string Build_Synonym_Synonym(string syn1, string syn2) = 0;
	virtual string Build_Synonym_NotSynonym(string syn1) = 0;
	virtual string Build_NotSynonym_Synonym(string syn2) = 0;
	virtual string Build_NotSynonym_NotSynonym() = 0;
public:
	IRelRefSqlColumnBuilder(string syn1, string syn2);
	virtual string getSqlColumnQuery() = 0;
};
