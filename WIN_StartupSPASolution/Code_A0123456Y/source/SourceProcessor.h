#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Database.h"
#include "Tokenizer.h"
#include <stack>
#include <regex>

using namespace std;

class SourceProcessor {
public:
	// method for processing the source program
	void process(string program);
};