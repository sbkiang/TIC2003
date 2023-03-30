#include "Rel_Modifies.h"

string Modifies::GetModifiesConstruct_StatementSynonym_Synonym(string input1, string input2)
{
    return string();
}

string Modifies::GetModifiesConstruct_StatementSynonym_NotSynonym(string input1)
{
    return string();
}

string Modifies::GetModifiesConstruct_NameSynonym_Synonym(string input1, string input2)
{
    return string();
}

string Modifies::GetModifiesConstruct_NameSynonym_NotSynonym(string input1)
{
    return string();
}

string Modifies::GetModifiesConstruct_StatementNotSynonym_Synonym(string input2)
{
    return string();
}

string Modifies::GetModifiesConstruct_StatementNotSynonym_NotSynonym()
{
    return string();
}

string Modifies::GetModifiesConstruct_NameNotSynonym_Synonym(string input2)

{
    return string();
}

string Modifies::GetModifiesConstruct_NameNotSynonym_NotSynonym()
{
    return string();
}

bool Modifies::GetModifies_AnyReadAssign_Any(string frontSql, string input1, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_AnyCall_Any(string frontSql, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_AnyProcedure_Any(string frontSql, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_AnyWhileIf_Any(string frontSql, string input1, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_AnyReadAssign_Specific(string frontSql, string input1, string input2, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_AnyCall_Specific(string frontSql, string input2, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_AnyProcedure_Specific(string frontSql, string input2, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_AnyWhileIf_Specific(string frontSql, string input1, string input2, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_SpecificReadAssign_Any(string frontSql, string input1, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_SpecificCall_Any(string frontSql, string input1, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_SpecificProcedure_Any(string frontSql, string input1, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_SpecificWhileIf_Any(string frontSql, string input1, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_SpecificReadAssign_Specific(string frontSql, string input1, string input2, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_SpecificCall_Specific(string frontSql, string input1, string input2, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_SpecificProcedure_Specific(string frontSql, string input1, string input2, SqlResultStore& rs)
{
    return false;
}

bool Modifies::GetModifies_SpecificWhileIf_Specific(string frontSql, string input1, string input2, SqlResultStore& rs)
{
    return false;
}
