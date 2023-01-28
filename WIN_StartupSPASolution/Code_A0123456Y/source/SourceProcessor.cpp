#include "SourceProcessor.h"
#include "..\SPA\If_Then_Else.h"
#include "..\SPA\Procedure.h"
#include "..\SPA\While.h"
#include "..\SPA\Container.h"

// method for processing the source program
// This method currently only inserts the procedure name into the database
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all the required syntax.

void SourceProcessor::process(string program) {
	// initialize the database
	Database::initialize();

	// tokenize the program
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(program, tokens);

	// This logic is highly simplified based on iteration 1 requirements and 
	// the assumption that the programs are valid.

	//string procedureName = tokens.at(1);

	// insert the procedure into the database
	//Database::insertProcedure(procedureName);
	if (true) { {} {string test = "true"; } }

	vector<Procedure*> procedures;
	int curlyBrackets = 0;
	stack<Container*> parentStack;
	vector<string> containerWords{ "procedure","if","while" };
	vector<string> noncontainers{ "" };
	vector<string> assign, read, print, variable, constant, procedure;
	bool elseFlag = false;
	int stmtNum = 0;
	for (int i = 0; i < tokens.size(); i++) {
		string word = tokens.at(i);
		if (word == "{") { curlyBrackets++; }
		else if (word == "}") {
			curlyBrackets--;
			if (!parentStack.empty()) {
				if (parentStack.top()->_type != "if") {
					parentStack.pop();
				}
				else if (parentStack.top()->_type == "if" && elseFlag) { // for "if" container, pop after encountered "else"
					parentStack.pop();
					elseFlag = false;
				}
			}
		}
		else if (word == "procedure") {
			Procedure* procedure = new Procedure();
			i++;
			procedure->_name = tokens.at(i);
			procedures.push_back(procedure);
			parentStack.push(procedure);

			Database::insertProcedure(procedure->_name);
		}
		else if (word == "while") { // while(...){...}
			stmtNum++;
			Container* container = new Container();
			container->_type = "while";
			Statement* stmt = new Statement(stmtNum, elseFlag);
			if (!parentStack.empty()) {
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container);
			int brackets = 0;
			do { // Anything enclosed between the brackets is the if condition
				if (tokens.at(i) == "(") {
					brackets++;
					continue;
				}
				if (tokens.at(i) == ")") {
					brackets--;
					continue;
				}
				container->_condition += tokens.at(i);
				stmt->_stmt += tokens.at(i);
				i++;
			} while (brackets != 0);
			container->_statements.push_back(stmt);
			Database::insertStatement(stmtNum, procedures.back()->_name, word);
		}
		else if (word == "if") { // if(...) then {...} else {...}
			stmtNum++;
			Container* container = new Container();
			container->_type = "if";
			Statement* stmt = new Statement(stmtNum, elseFlag);
			if (!parentStack.empty()) { // if there's parent container, add current container to parent's child
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container); //set itself as the latest parent container
			while (tokens.at(i) != "then") { // from current index till "then", it's the condition. "if(...) then{"
				container->_condition += tokens.at(i);
				stmt->_stmt += tokens.at(i);
				i++;
			}
			container->_statements.push_back(stmt);
			Database::insertStatement(stmtNum, procedures.back()->_name, word);
		}
		else if (word == "else") { // set the flag for encountering "else"
			elseFlag = true;
		}
		else if (word == "=") { // for assign
			stmtNum++;
			Statement* stmt = new Statement(stmtNum, elseFlag);
			while (tokens.at(i) != ";") {
				stmt->_stmt += tokens.at(i);
				i++;
			}
			parentStack.top()->_statements.push_back(stmt);
			Database::insertStatement(stmtNum, procedures.back()->_name, "assign");
			Database::insertVariable(tokens.at(i - 3), stmtNum);
		}
		else if (word == "read" || word == "print" || word == "call") {
			stmtNum++;
			Statement* stmt = new Statement(stmtNum, elseFlag);
			while (tokens.at(i) != ";") {
				stmt->_stmt += tokens.at(i);
				i++;
			}
			parentStack.top()->_statements.push_back(stmt);
			Database::insertStatement(stmtNum, procedures.back()->_name, word);

			if (word == "read") {
				Database::insertVariable(tokens.at(i - 1), stmtNum);
			}
		}
	}
}

void formStatement(int index, vector<string> tokens, int& retIndex) {
	string ret = "";
	vector<string> r_p_c{ "read","print","call" };
	vector<string> i_w{ "if","while" };
	// join tokens together, delimited by ";" = end of statement

	// if current token is a (read, print, call), find the terminating ";" to form a statement
	if (find(r_p_c.begin(), r_p_c.end(), tokens.at(index)) == r_p_c.end()) {
		while (tokens.at(index) != ";") {
			ret += tokens.at(index);
			index++;
		}
	}
}