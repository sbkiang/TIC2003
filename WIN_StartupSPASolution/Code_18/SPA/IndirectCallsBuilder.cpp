#include "IndirectCallsBuilder.h"

void IndirectCallsBuilder::_BuildIndirectCalls(vector<Statement*> callStmt, vector<pair<string, string>>& callerCallee)
{

}

vector<pair<string, string>> IndirectCallsBuilder::BuildIndirectCalls(vector<Statement*> callStmt)
{
    vector<pair<string, string>> callerCallee;
    _BuildIndirectCalls(callStmt, callerCallee);
    return callerCallee;
}
