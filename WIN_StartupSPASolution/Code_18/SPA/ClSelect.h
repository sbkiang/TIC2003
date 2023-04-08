#pragma once
#include <vector>
#include <string>

using namespace std;

class ClSelect {
private:
	vector<string> _synonym;

public:
	ClSelect(vector<string> synonym);
	vector<string> GetSynonym();
};