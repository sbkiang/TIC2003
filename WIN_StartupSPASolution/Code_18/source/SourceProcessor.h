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
#include "..\SPA\Statement.h"

using namespace std;

struct CFGNode;

class SourceProcessor {
public:
	// method for processing the source program
	void process(string program);

	bool is_operator(char c);
	int prec(char c);
	string infixToPostfix(string s);

};



