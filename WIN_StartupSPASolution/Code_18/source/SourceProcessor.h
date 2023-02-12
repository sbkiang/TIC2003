#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <regex>
#include <assert.h>
#include <iomanip>
#include <map>
#include "Database.h"
#include "Tokenizer.h"
#include "..\SPA\Procedure.h"
#include "..\SPA\Container.h"
#include "..\SPA\CFGBuilder.h"

using namespace std;

class SourceProcessor {
public:
	// method for processing the source program
	void process(string program);
};