#include "ClSelect.h"

ClSelect::ClSelect(vector<string> synonym)
{
	_synonym = synonym;
}

vector<string> ClSelect::GetSynonym()
{
	return _synonym;
}
