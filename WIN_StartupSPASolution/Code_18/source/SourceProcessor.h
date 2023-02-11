#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Database.h"
#include "Tokenizer.h"
#include <stack>
#include <regex>
#include <assert.h>
#include <iomanip>
#include <unordered_map>
#include <map>
#include "..\SPA\Procedure.h"
#include "..\SPA\Container.h"

using namespace std;

class SourceProcessor {
public:
	// method for processing the source program
	void process(string program);
};