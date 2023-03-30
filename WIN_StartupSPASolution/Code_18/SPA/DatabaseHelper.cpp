#include "DatabaseHelper.h"

string DatabaseHelper::GetEntityByStatement(string input) {
	char sqlBuf[100] = {};
	sprintf_s(sqlBuf, "select entity from statement where line_num = %s", input.c_str());
	return string(sqlBuf);
}