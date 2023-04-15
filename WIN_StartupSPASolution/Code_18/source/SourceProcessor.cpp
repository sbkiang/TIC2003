#include "SourceProcessor.h"
#include "QueryProcessor.h"
#include <unordered_map>

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

	vector<Procedure*> procedure;
	stack<Container*> parentStack;
	vector<string> caller, callee;
	vector<Statement*> callStatements;
	int stmtNumSubtract = 0;
	int stmtNum = 0;
	int nestedLevel = 0;
	string sql;
	for (int i = 0; i < tokens.size(); i++) {	
		string word = tokens.at(i);
		if (word == "}") { // "}" indicates the end of a container
			if (!parentStack.empty()) {
				nestedLevel--;
				parentStack.top()->_endStmtNum = stmtNum;
				parentStack.top()->_adjustedEndStmtNum = stmtNum - stmtNumSubtract;
				parentStack.pop();
			}
		}
		else if (word == "procedure") {
			i++;
			Procedure* p = new Procedure(tokens.at(i));
			p->_type = "procedure";
			p->_startStmtNum = stmtNum + 1;
			p->_adjustedStartStmtNum = p->_startStmtNum - stmtNumSubtract;
			p->_level = nestedLevel;
			procedure.push_back(p);
			parentStack.push(p);
			nestedLevel++;
		}
		else if (word == "while") { // while(...){...}
			stmtNum++;
			nestedLevel++;
			Container* container = new Container();
			container->_type = word;
			container->_startStmtNum = stmtNum;
			container->_adjustedStartStmtNum = stmtNum - stmtNumSubtract;
			container->_level = nestedLevel;
			Statement* stmt = new Statement(stmtNum, nestedLevel, container, stmtNumSubtract);
			stmt->SetEntity(word);
			if (!parentStack.empty()) {
				container->_parent = parentStack.top();
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container);
			vector<Statement> variableStore;
			vector<Statement> useStore;
			i++; // skip the "while" keyword
			while (tokens.at(i) != "{") {
				container->_condition += tokens.at(i);
				stmt->AddStmt(tokens.at(i));
				stmt->AddToken(tokens.at(i));
				if (regex_match(tokens.at(i), regex(regexConstants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(regexVariables))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i), container, stmtNumSubtract));
				}
				if (regex_match(tokens.at(i), regex(regexVariables))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), stmtNumSubtract));
				}
				i++;
			}
			container->_statements.push_back(stmt);
			Database::insertStatement(stmt->GetAdjustedStmtNum(), word, stmt->GetStmt());
			for (int i = 0; i < variableStore.size(); i++) { // insert the variable after inserting the statement due to FK
				Database::insertVariable(variableStore.at(i).GetStmt(), variableStore.at(i).GetAdjustedStmtNum());
			}

			for (int i = 0; i < useStore.size(); i++) {
				// database PK constraint will trigger for duplicate variables with same line_num to prevent duplicate insertion
				Database::insertUses(useStore.at(i).GetAdjustedStmtNum(), useStore.at(i).GetStmt());
				procedure.back()->_uses.push_back(useStore.at(i).GetStmt());
			}
		}
		else if (word == "if") { // if(...) then {...} else {...}
			stmtNum++;
			nestedLevel++;
			Container* container = new Container();
			container->_type = word;
			container->_startStmtNum = stmtNum;
			container->_adjustedStartStmtNum = stmtNum - stmtNumSubtract;
			container->_level = nestedLevel;
			container->_parent = parentStack.top();
			Statement* stmt = new Statement(stmtNum, nestedLevel, container, stmtNumSubtract);
			stmt->SetEntity(word);
			if (!parentStack.empty()) { // if there's parent container, add current container to parent's child
				container->_parent = parentStack.top();
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container); //set itself as the latest parent container
			vector<Statement> variableStore; // we need to insert statement first before inserting variable due to FK
			vector<Statement> useStore;
			i++; // skip the "if" keyword for the while loop below
			while (tokens.at(i) != "then") { // from current index till "then", it's the condition. "if(...) then{"
				container->_condition += tokens.at(i);
				stmt->AddStmt(tokens.at(i));
				stmt->AddToken(tokens.at(i));
				if (regex_match(tokens.at(i), regex(regexConstants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(regexVariables))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i), container, stmtNumSubtract));
				}

				if (regex_match(tokens.at(i), regex(regexVariables))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), stmtNumSubtract));
				}

				i++;
			}
			container->_statements.push_back(stmt);
			Database::insertStatement(stmt->GetAdjustedStmtNum(), word, stmt->GetStmt());
			for (int i = 0; i < variableStore.size(); i++) {
				Database::insertVariable(variableStore.at(i).GetStmt(), variableStore.at(i).GetAdjustedStmtNum());
			}

			for (int i = 0; i < useStore.size(); i++) {
				// database PK constraint will trigger for duplicate variables with same line_num to prevent duplicate insertion
				Database::insertUses(useStore.at(i).GetAdjustedStmtNum(), useStore.at(i).GetStmt());
				procedure.back()->_uses.push_back((useStore.at(i).GetStmt()));
			}
		}
		else if (word == "else") { // for else container
			stmtNum++;
			nestedLevel++;
			stmtNumSubtract++;
			Container* container = new Container();
			container->_type = word;
			Statement* stmt = new Statement(stmtNum, nestedLevel, container, stmtNumSubtract - 1);
			stmt->SetEntity(word);
			stmt->AddStmt(word);
			stmt->AddToken(word);
			container->_statements.push_back(stmt);
			container->_startStmtNum = stmtNum;
			container->_adjustedStartStmtNum = stmtNum - stmtNumSubtract;
			container->_level = nestedLevel;
			if (!parentStack.empty()) { // if there's parent container, add current container to parent's child
				container->_parent = parentStack.top();
				parentStack.top()->_childContainers.push_back(container);
			}
			parentStack.push(container); // we push the current "else" container to the parentStack for future statements
		}
		else if (word == "=") { // for assign
			stmtNum++;
			Statement* stmt = new Statement(stmtNum, nestedLevel, parentStack.top(), stmtNumSubtract);
			stmt->AddStmt(tokens.at(i - 1));
			stmt->AddToken(tokens.at(i - 1));
			stmt->SetEntity("assign");
			vector<Statement> variableStore; // we need to insert statement first before inserting variable due to FK. So, we store the variables here
			vector<Statement> useStore;
			vector<Statement> modifiesStore;
			variableStore.push_back(Statement(stmtNum, tokens.at(i - 1), stmtNumSubtract));
			string LHS = tokens.at(i - 1);
			while (tokens.at(i) != ";") {
				stmt->AddStmt(tokens.at(i));
				stmt->AddToken(tokens.at(i));
				if (regex_match(tokens.at(i), regex(regexConstants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(regexVariables))) {
					variableStore.push_back(Statement(stmtNum, tokens.at(i), nestedLevel, parentStack.top(), stmtNumSubtract));
				}

				if (regex_match(tokens.at(i), regex(regexVariables))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), stmtNumSubtract));
				}
				i++;
			}
			parentStack.top()->_statements.push_back(stmt);
			modifiesStore.push_back(Statement(stmtNum, LHS, stmtNumSubtract)); //Store LHS variable
			Database::insertStatement(stmt->GetAdjustedStmtNum(), stmt->GetEntity(), stmt->GetStmt());
			
			for (int i = 0; i < variableStore.size(); i++) {
				Database::insertVariable(variableStore.at(i).GetStmt(), variableStore.at(i).GetAdjustedStmtNum());
			}

			for (int i = 0; i < useStore.size(); i++) {
				// database PK constraint will trigger for duplicate variables with same line_num to prevent duplicate insertion
				Database::insertUses(useStore.at(i).GetAdjustedStmtNum(), useStore.at(i).GetStmt());
				procedure.back()->_uses.push_back(useStore.at(i).GetStmt());
				//parentStack.top()->_uses.push_back((useStore.at(i)._stmt));
			}

			for (int i = 0; i < modifiesStore.size(); i++) {
				Database::insertModifies(modifiesStore.at(i).GetAdjustedStmtNum(), modifiesStore.at(i).GetStmt());
				procedure.back()->_modifies.push_back(modifiesStore.at(i).GetStmt());
			}

			size_t equal_pos = (stmt->GetStmt()).find("="); // Find position of the equal sign
			string RHS = (stmt->GetStmt()).substr(equal_pos + 1); //RHS expression
			Database::insertPattern(stmt->GetAdjustedStmtNum(), LHS, RHS, HelperFunction::InfixToPostfix(RHS));
		}
		else if (word == "read" || word == "print" || word == "call") {
			stmtNum++;
			Statement* stmt = new Statement(stmtNum, nestedLevel, parentStack.top(), stmtNumSubtract);
			//stmt->_stmt += tokens.at(i); // skip the (read, print, call) keywords
			i++; // skip the keywords
			stmt->AddStmt(tokens.at(i));
			stmt->AddToken(tokens.at(i));
			parentStack.top()->_statements.push_back(stmt);
			Database::insertStatement(stmt->GetAdjustedStmtNum(), word, stmt->GetStmt());
			stmt->SetEntity(word);
			if (word == "read") {
				Database::insertVariable(stmt->GetStmt(), stmt->GetAdjustedStmtNum());
				Database::insertModifies(stmt->GetAdjustedStmtNum(), stmt->GetStmt());
				procedure.back()->_modifies.push_back(stmt->GetStmt());
			} 
			else if(word == "print") {
				Database::insertVariable(stmt->GetStmt(), stmt->GetAdjustedStmtNum());
				Database::insertUses(stmt->GetAdjustedStmtNum(), stmt->GetStmt());
				procedure.back()->_uses.push_back(stmt->GetStmt());
			}
			else if (word == "call") {
				caller.push_back(procedure.back()->_name);
				callee.push_back(tokens.at(i));
				callStatements.push_back(stmt);
				Database::insertCall(procedure.back()->_name, tokens.at(i));
			}
		}
	}

	unordered_map<Statement*, Procedure*> stmtProcMap;
	
	// add each called procedure to the parent procedure
	for (int i = 0; i < callStatements.size(); i++) {
		Statement* cp = callStatements.at(i);
		Procedure* parentProc = nullptr;
		Procedure* childProc = nullptr;
		for (int j = 0; j < procedure.size(); j++) {
			if (cp->GetAdjustedStmtNum() >= procedure.at(j)->_adjustedStartStmtNum && cp->GetAdjustedStmtNum() <= procedure.at(j)->_adjustedEndStmtNum) {
				parentProc = procedure.at(j);
			}
			if (cp->GetStmt() == procedure.at(j)->_name) {
				childProc = procedure.at(j);
			}
			if (childProc && parentProc) { break; }
		}
		parentProc->_calls.push_back(childProc);
		stmtProcMap.insert(pair<Statement*, Procedure*>(cp, childProc)); // save each call stmt to the called procedure
	}

	// for each called procedure, find all the direct and indirect variables that satisfy uses(s,v) and modifies(s,v), and add them to the database 
	for (auto it = stmtProcMap.begin(); it != stmtProcMap.end(); it++) {
		Statement* stmt = (*it).first;
		Procedure* proc = (*it).second;
		if (!proc) { cout << "**ERROR** Procedure not found"; }
		vector<string> allUses = proc->GetAllUses();
		vector<string> allModifies = proc->GetAllModifies();
		for (int i = 0; i < allUses.size(); i++) { Database::insertUses(stmt->GetAdjustedStmtNum(), allUses.at(i));	}
		for (int i = 0; i < allModifies.size(); i++) { Database::insertModifies(stmt->GetAdjustedStmtNum(), allModifies.at(i)); }
	}

	for (int i = 0; i < procedure.size(); i++) {
		Database::insertProcedure(procedure.at(i)->_name, procedure.at(i)->_adjustedStartStmtNum, procedure.at(i)->_adjustedEndStmtNum);
	}

	for (int i = 0; i < procedure.size(); i++) {
		vector<Container> containers = procedure.at(i)->getAllContainers(); // get all the if and while containers
		for (int j = 0; j < containers.size(); j++) {
			Database::insertParent(containers.at(j)._adjustedStartStmtNum, containers.at(j)._adjustedStartStmtNum + 1, containers.at(j)._adjustedEndStmtNum);
		}
	}

	vector<CFG*> CFGs;
	for (int i = 0; i < procedure.size(); i++) {
		CFG cfg = CFGBuilder::BuildCFG(procedure.at(i));
		vector<CFGNode*> nodes = cfg.GetAllCFGNodes();
		for (int j = 0; j < nodes.size(); j++) {
			CFGNode* node = nodes.at(j);
			int nodeStmtNum = node->_stmtPtr->GetAdjustedStmtNum();
			if (node->_stmtPtr->GetStmt() == "else") {
				continue;
			}
			if (node->_sJump) {
				int nextStmtNum = node->_sJump->_stmtPtr->GetAdjustedStmtNum();
				Database::insertNext(nodeStmtNum, nextStmtNum);
			}
			if (node->_fJump) {
				int nextStmtNum = node->_fJump->_stmtPtr->GetAdjustedStmtNum();
				Database::insertNext(nodeStmtNum, nextStmtNum);
			}
		}
		//procedure.at(i)->printContainerTree(0);
	}
	
}