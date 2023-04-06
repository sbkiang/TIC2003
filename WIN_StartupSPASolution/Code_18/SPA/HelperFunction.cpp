#include "HelperFunction.h"

string HelperFunction::GetEntityByStatement(string input) {
	char sqlBuf[100] = {};
	sprintf_s(sqlBuf, "select entity from statement where line_num = %s", input.c_str());
	SqlResultStore rs;
	Database::ExecuteSql(sqlBuf, rs);
	auto it = rs.sqlResultSet.begin();
	if (it != rs.sqlResultSet.end()) {
		return it->row.at("entity");
	}
	else {
		return "";
	}
}

string HelperFunction::PatternExprToPostFix(string input) {
	bool startWildCard = (input[0] == '_');
	bool endWildCard = (input[input.length() - 1] == '_');
	input = (startWildCard) ? input.substr(1, input.length()) : input;
	input = (endWildCard) ? input.substr(0, input.length() - 1) : input;
	input = InfixToPostfix(input);
	input = (startWildCard) ? ("_" + input) : input;
	input = (endWildCard) ? (input + "_") : input;
	return input;
}

int HelperFunction::Prec(char c) {
	if (c == '^')
		return 3;
	else if (c == '/' || c == '*' || c == '%')
		return 2;
	else if (c == '+' || c == '-')
		return 1;
	else
		return -1;
}


string HelperFunction::InfixToPostfix(string s) {
	stack<char> st;
	string result;
	for (int i = 0; i < s.length(); i++) {
		char c = s[i];
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
			result += c;
		else if (c == '(')
			st.push('(');
		else if (c == ')') {
			while (st.top() != '(')
			{
				result += st.top();
				st.pop();
			}
			st.pop();
		}
		else {
			while (!st.empty() && Prec(s[i]) <= Prec(st.top())) {
				result += st.top();
				st.pop();
			}
			st.push(c);
		}
	}
	while (!st.empty()) {
		result += st.top();
		st.pop();
	}
	return result;
}

string HelperFunction::ConvertPqlPatternOprtToSqlPatternOprt(string input)
{
	int pos = 0;
	do {
		pos = input.find("_");
		if (pos > -1) { input.replace(pos, 1, "%"); }
	} while (pos > -1);
	return input;
}

string HelperFunction::RemoveQuote(string input) {
	input.erase(remove(input.begin(), input.end(), '\"'), input.end());
	return input;
}

set<RowSet> HelperFunction::CartesianProduct(set<RowSet> set1, set<RowSet> set2) {
	set<RowSet> cpSet;
	for (RowSet rs1 : set1) {
		for (RowSet rs2 : set2) {
			RowSet nrs = rs1;
			nrs.row.insert(rs2.row.begin(), rs2.row.end());
			cpSet.insert(nrs);
		}
	}
	return cpSet;
}

vector<RowSet> HelperFunction::CartesianProduct(vector<RowSet> set1, vector<RowSet> set2) {
	vector<RowSet> cpSet;
	for (RowSet rs1 : set1) {
		for (RowSet rs2 : set2) {
			RowSet nrs = rs1;
			nrs.row.insert(rs2.row.begin(), rs2.row.end());
			cpSet.push_back(nrs);
		}
	}
	return cpSet;
}


set<string> HelperFunction::GetColName(set<RowSet> set1) {
	set<string> colName;
	for (RowSet rs1 : set1) {
		for (pair<string, string> row : rs1.row) {
			colName.insert(row.first);
		}
	}
	return colName;
}

set<RowSet> HelperFunction::CommonColumnIntersect(set<RowSet> set1, set<RowSet> set2)
{
	set<RowSet> newSet;
	for (RowSet rs1 : set1) {
		for (RowSet rs2 : set2) {
			if (!(rs1 < rs2) && !(rs2 < rs1)) {
					RowSet tempRs;
				for (pair<string, string> p : rs2.row) {
					tempRs.row.insert(p);
				}
				for (pair<string,string> p : rs1.row) {
					tempRs.row.insert(p);
				}
				newSet.insert(tempRs);
			}
		}
	}
	return newSet;
}

vector<RowSet> HelperFunction::CommonColumnIntersect(vector<RowSet> set1, vector<RowSet> set2)
{
	vector<RowSet> newRowSet;
	for (RowSet rs1 : set1) {
		for (RowSet rs2 : set2) {
			if (!(rs1 < rs2) && !(rs2 < rs1)) {
				RowSet tempRs;
				for (pair<string, string> p : rs2.row) {
					tempRs.row.insert(p);
				}
				for (pair<string, string> p : rs1.row) {
					tempRs.row.insert(p);
				}
				newRowSet.push_back(tempRs);
			}
		}
	}
	return newRowSet;
}

void HelperFunction::PrintRowSet(set<RowSet> setrs)
{
	bool colPrinted = false;
	string head;
	string body;
	for (RowSet rs : setrs) {
		body = "";
		if (!colPrinted) {
			for (pair<string, string> it : rs.row) {
				head += ("\t" + it.first);
			}
			cout << endl << head << endl;
			colPrinted = true;
		}
		for (pair<string, string> it : rs.row) {
			body += ("\t" + it.second);
		}
		cout << body << endl;
	}
}

bool HelperFunction::inputIsSynonym(map<string, string> SynEntMap, string input)
{
	return false;
}
