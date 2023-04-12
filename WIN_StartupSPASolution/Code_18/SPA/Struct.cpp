#include "Struct.h"

bool RowSet::operator< (const RowSet& otherSqlRow) const
{
	vector<string> commonColName;
	set<string> thisColName;
	set<string> thatColName;
	//string thisCol, thatCol;
	//cout << endl << "NEW COMPARE" << endl;
	for (auto it = row.begin(); it != row.end(); it++) {
		thisColName.insert(it->first);
		//thisCol += (it->first + ",");
	}
	for (auto it2 = otherSqlRow.row.begin(); it2 != otherSqlRow.row.end(); it2++) {
		thatColName.insert(it2->first);
		//thatCol += (it2->first + ",");
	}
	set_intersection(thisColName.begin(), thisColName.end(), thatColName.begin(), thatColName.end(), inserter(commonColName, commonColName.begin()));
	size_t thisHashResult = 0;
	size_t otherHashResult = 0;
	char buf[512] = {};
	/*
	cout << "thisCol | thatCol " << thisCol << " | " << thatCol << endl;
	string thisVal, thatVal;
	string commonCol;
	*/
	for (int i = 0; i < commonColName.size(); i++) {
		thisHashResult = otherHashResult = 0;
		string col = commonColName.at(i);
		/*
		commonCol += (col + ",");
		thisVal += (this->row.at(col) + ",");
		thatVal += (otherSqlRow.row.at(col) + ",");
		sprintf_s(buf, "<\tthis: %s - %s\tthat: %s - %s", commonColName.at(i).c_str(), this->row.at(commonColName.at(i)).c_str(), commonColName.at(i).c_str(), otherSqlRow.row.at(commonColName.at(i)).c_str());
		thisHashResult = thisHashResult ^ hash<string>{}(this->row.at(col));
		otherHashResult = otherHashResult ^ hash<string>{}(otherSqlRow.row.at(col));
		*/
		thisHashResult = hash<string>{}(this->row.at(col));
		otherHashResult = hash<string>{}(otherSqlRow.row.at(col));

		// if the same column values are different, we don't need to compare other columns
		if (thisHashResult != otherHashResult) {
			break;
		}
	}
	/*
	bool compare = thisHashResult == otherHashResult;
	string coutprint = commonCol + " - " + thisVal + "\t" + commonCol + " - " + thatVal;
	cout << coutprint << (compare ? "\ttrue" : "\tfalse") << endl;
	*/
	return thisHashResult < otherHashResult;
}