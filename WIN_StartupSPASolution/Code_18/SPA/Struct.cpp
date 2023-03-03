#include "Struct.h"

using namespace std;

string SqlResult::getColumnValue(string col) {
	return row.at(col);
}

vector<string> SqlResultStore::getColumnValues(string col) {
	vector<string> result;
	if (sqlResult.empty()) {
		return result;
	}
	for (auto it = sqlResult.begin(); it != sqlResult.end(); it++) {
		if ((*it).getColumnValue(col) == "") { break; }
		result.push_back((*it).getColumnValue(col));
	}
	return result;
}