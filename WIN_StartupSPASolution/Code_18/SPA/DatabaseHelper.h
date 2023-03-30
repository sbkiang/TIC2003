#pragma once
#include <format>
#include <string>

using namespace std;

class DatabaseHelper {
public:
	static string GetEntityByStatement(string input);
};