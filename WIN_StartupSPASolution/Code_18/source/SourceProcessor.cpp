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

	stack<IContainer*> parentStack2;
	vector<Procedure*> procedure;
	//vector<ContainerProcedure*> procedure2;
	vector<Statement*> callStatements;
	map<string, vector<string>> procedureToUsesMap;
	map<string, vector<string>> procedureToModifiesMap;
	vector<string> usesVariable, modifiesVariable;
	map<string, ContainerProcedure*> procedureNameToPtrMap;
	int stmtNumSubtract = 0;
	int stmtNum = 0;
	int nestedLevel = 0;
	string sql;
	string procedureName;
	for (int i = 0; i < tokens.size(); i++) {	
		string word = tokens.at(i);
		if (word == "}") { // "}" indicates the end of a container
			if (!parentStack2.empty()) {
				nestedLevel--;
				if (parentStack2.top()->GetType() == "procedure") {
					procedureToUsesMap.insert(pair<string, vector<string>>(procedureName, usesVariable));
					procedureToModifiesMap.insert(pair<string, vector<string>>(procedureName, modifiesVariable));
					usesVariable.clear();
					modifiesVariable.clear();
				}
				parentStack2.top()->SetEndStmtNum(stmtNum);
				parentStack2.top()->SetAdjustedEndStmtNum(stmtNum - stmtNumSubtract);
				parentStack2.pop();
			}
		}
		else if (word == "procedure") {
			i++;
			ContainerProcedure* p2 = new ContainerProcedure(nestedLevel);
			p2->SetStartStmtNum(stmtNum + 1);
			p2->SetAdjustedStartStmtNum(p2->GetStartStmtNum() - stmtNumSubtract);
			parentStack2.push(p2);
			//procedure2.push_back(p2);
			procedureNameToPtrMap.insert(pair<string, ContainerProcedure*>(tokens.at(i), p2));
			procedureName = tokens.at(i);
			nestedLevel++;
		}
		else if (word == "while") { // while(...){...}
			stmtNum++;
			nestedLevel++;

			ContainerWhile* container2 = new ContainerWhile(nestedLevel);
			container2->SetStartStmtNum(stmtNum);
			container2->SetAdjustedStartStmtNum(stmtNum - stmtNumSubtract);
			Statement* stmt = new Statement(stmtNum, nestedLevel, container2, stmtNumSubtract);
			stmt->SetEntity(word);
			if (!parentStack2.empty()) {
				parentStack2.top()->PushBackChildContainer(container2);
			}
			parentStack2.push(container2);
			vector<string> variableStore;
			vector<Statement> useStore;
			i++; // skip the "while" keyword
			while (tokens.at(i) != "{") {
				stmt->AddStmt(tokens.at(i));
				stmt->AddToken(tokens.at(i));
				if (regex_match(tokens.at(i), regex(regexConstants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(regexVariables))) {
					variableStore.push_back(tokens.at(i));
				}
				if (regex_match(tokens.at(i), regex(regexVariables))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), stmtNumSubtract));
				}
				i++;
			}
			container2->PushBackStatement(stmt);
			Database::insertStatement(stmt->GetAdjustedStmtNum(), word, stmt->GetStmt());
			for (int i = 0; i < variableStore.size(); i++) { // insert the variable after inserting the statement due to FK
				Database::insertVariable(variableStore.at(i));
			}

			for (int i = 0; i < useStore.size(); i++) {
				// database PK constraint will trigger for duplicate variables with same line_num to prevent duplicate insertion
				Database::insertUses(useStore.at(i).GetAdjustedStmtNum(), useStore.at(i).GetStmt());
				usesVariable.push_back(useStore.at(i).GetStmt());
			}
		}
		else if (word == "if") { // if(...) then {...} else {...}
			stmtNum++;
			nestedLevel++;

			ContainerIf* container2 = new ContainerIf(nestedLevel);
			container2->SetStartStmtNum(stmtNum);
			container2->SetAdjustedStartStmtNum(stmtNum - stmtNumSubtract);
			Statement* stmt = new Statement(stmtNum, nestedLevel, container2, stmtNumSubtract);
			stmt->SetEntity(word);
			if (!parentStack2.empty()) { // if there's parent container, add current container to parent's child
				parentStack2.top()->PushBackChildContainer(container2);
			}
			parentStack2.push(container2);
			vector<string> variableStore; // we need to insert statement first before inserting variable due to FK
			vector<Statement> useStore;
			i++; // skip the "if" keyword for the while loop below
			while (tokens.at(i) != "then") { // from current index till "then", it's the condition. "if(...) then{"
				stmt->AddStmt(tokens.at(i));
				stmt->AddToken(tokens.at(i));
				if (regex_match(tokens.at(i), regex(regexConstants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(regexVariables))) {
					variableStore.push_back(tokens.at(i));
				}

				if (regex_match(tokens.at(i), regex(regexVariables))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), stmtNumSubtract));
				}
				i++;
			}
			container2->PushBackStatement(stmt);
			Database::insertStatement(stmt->GetAdjustedStmtNum(), word, stmt->GetStmt());
			for (int i = 0; i < variableStore.size(); i++) {
				Database::insertVariable(variableStore.at(i));
			}
			for (int i = 0; i < useStore.size(); i++) {
				// database PK constraint will trigger for duplicate variables with same line_num to prevent duplicate insertion
				Database::insertUses(useStore.at(i).GetAdjustedStmtNum(), useStore.at(i).GetStmt());
				usesVariable.push_back(useStore.at(i).GetStmt());
			}
		}
		else if (word == "else") { // for else container
			stmtNum++;
			nestedLevel++;
			stmtNumSubtract++;

			ContainerElse* container2 = new ContainerElse(nestedLevel);
			container2->SetStartStmtNum(stmtNum);
			container2->SetAdjustedStartStmtNum(stmtNum - stmtNumSubtract);

			Statement* stmt = new Statement(stmtNum, nestedLevel, container2, stmtNumSubtract - 1);
			stmt->SetEntity(word);
			stmt->AddStmt(word);
			stmt->AddToken(word);

			container2->PushBackStatement(stmt);
			if (!parentStack2.empty()) { // if there's parent container, add current container to parent's child
				parentStack2.top()->PushBackChildContainer(container2);
			}
			parentStack2.push(container2);
		}
		else if (word == "=") { // for assign
			stmtNum++;
			Statement* stmt = new Statement(stmtNum, nestedLevel, parentStack2.top(), stmtNumSubtract);
			stmt->AddStmt(tokens.at(i - 1));
			stmt->AddToken(tokens.at(i - 1));
			stmt->SetEntity("assign");
			vector<string> variableStore; // we need to insert statement first before inserting variable due to FK. So, we store the variables here
			vector<Statement> useStore;
			vector<Statement> modifiesStore;
			variableStore.push_back(tokens.at(i - 1));
			string LHS = tokens.at(i - 1);
			while (tokens.at(i) != ";") {
				stmt->AddStmt(tokens.at(i));
				stmt->AddToken(tokens.at(i));
				if (regex_match(tokens.at(i), regex(regexConstants))) {
					Database::insertConstant(tokens.at(i));
				}
				else if (regex_match(tokens.at(i), regex(regexVariables))) {
					variableStore.push_back(tokens.at(i));
				}

				if (regex_match(tokens.at(i), regex(regexVariables))) {
					useStore.push_back(Statement(stmtNum, tokens.at(i), stmtNumSubtract));
				}
				i++;
			}

			parentStack2.top()->PushBackStatement(stmt);
			modifiesStore.push_back(Statement(stmtNum, LHS, stmtNumSubtract)); //Store LHS variable
			Database::insertStatement(stmt->GetAdjustedStmtNum(), stmt->GetEntity(), stmt->GetStmt());
			
			for (int i = 0; i < variableStore.size(); i++) {
				Database::insertVariable(variableStore.at(i));
			}

			for (int i = 0; i < useStore.size(); i++) {
				// database PK constraint will trigger for duplicate variables with same line_num to prevent duplicate insertion
				Database::insertUses(useStore.at(i).GetAdjustedStmtNum(), useStore.at(i).GetStmt());
				usesVariable.push_back(useStore.at(i).GetStmt());
				//parentStack.top()->_uses.push_back((useStore.at(i)._stmt));
			}

			for (int i = 0; i < modifiesStore.size(); i++) {
				Database::insertModifies(modifiesStore.at(i).GetAdjustedStmtNum(), modifiesStore.at(i).GetStmt());
				modifiesVariable.push_back(modifiesStore.at(i).GetStmt());
			}

			size_t equal_pos = (stmt->GetStmt()).find("="); // Find position of the equal sign
			string RHS = (stmt->GetStmt()).substr(equal_pos + 1); //RHS expression
			Database::insertPattern(stmt->GetAdjustedStmtNum(), LHS, RHS, HelperFunction::InfixToPostfix(RHS));
		}
		else if (word == "read" || word == "print" || word == "call") {
			stmtNum++;
			Statement* stmt = new Statement(stmtNum, nestedLevel, parentStack2.top(), stmtNumSubtract);
			i++; // skip the keywords
			stmt->AddStmt(tokens.at(i));
			stmt->AddToken(tokens.at(i));
			parentStack2.top()->PushBackStatement(stmt);
			Database::insertStatement(stmt->GetAdjustedStmtNum(), word, stmt->GetStmt());
			stmt->SetEntity(word);
			if (word == "read") {
				Database::insertVariable(stmt->GetStmt());
				Database::insertModifies(stmt->GetAdjustedStmtNum(), stmt->GetStmt());
				modifiesVariable.push_back(stmt->GetStmt());
			} 
			else if(word == "print") {
				Database::insertVariable(stmt->GetStmt());
				Database::insertUses(stmt->GetAdjustedStmtNum(), stmt->GetStmt());
				usesVariable.push_back(stmt->GetStmt());
			}
			else if (word == "call") {
				callStatements.push_back(stmt);
				Database::insertCall(procedureName, tokens.at(i));
			}
		}
	}

	//for each call stmt, find get all the child container?

	for (Statement* stmt : callStatements) {
		string procedureName = stmt->GetStmt();
		for (string var : procedureToUsesMap.at(procedureName)) {
			Database::insertUses(stmt->GetAdjustedStmtNum(), var);
		}
		for (string var : procedureToModifiesMap.at(procedureName)) {
			Database::insertModifies(stmt->GetAdjustedStmtNum(), var);
		}

		SqlResultStore callChain;
		ClRelRef relRef = ClRelRef("Calls*", stmt->GetStmt(), "_");
		DescriberClRelRef describer = DescriberClRelRef(relRef, map<string, string>());
		BuilderSqlSelectCallsT builder = BuilderSqlSelectCallsT();
		string sql = builder.GetSql(relRef, describer);
		Database::ExecuteSql(sql, callChain);
		for (RowSet rs : callChain.sqlResultSet) {
			for (pair<string,string> row : rs.row) {
				for (string var : procedureToUsesMap.at(row.second)) {
					Database::insertUses(stmt->GetAdjustedStmtNum(), var);
				}
				for (string var : procedureToModifiesMap.at(row.second)) {
					Database::insertModifies(stmt->GetAdjustedStmtNum(), var);
				}
			}
		}
	}

	vector<CFG*> CFGs;
	for (auto pair : procedureNameToPtrMap) {
	//for(ContainerProcedure* procedure : procedure2){
	//for (int i = 0; i < procedure.size(); i++) {
		//CFG cfg = CFGBuilder::BuildCFG(procedure);
		CFG cfg = CFGBuilder::BuildCFG(pair.second);
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

	for (pair<string, ContainerProcedure*> pair : procedureNameToPtrMap) {
		Database::insertProcedure(pair.first, pair.second->GetAdjustedStartStmtNum(), pair.second->GetAdjustedEndStmtNum());
	}

	for (pair<string, ContainerProcedure*> pair : procedureNameToPtrMap) {
		HelperFunction::LinkIfElseEndStmtNum(pair.second);
		vector<IContainer*> containers = pair.second->GetAllChildContainer(); // get all the if and while containers
		for (int j = 0; j < containers.size(); j++) {
			if (containers.at(j)->GetType() == "else") {
				continue;
			}
			Database::insertParent(containers.at(j)->GetAdjustedStartStmtNum(), containers.at(j)->GetAdjustedStartStmtNum() + 1, containers.at(j)->GetAdjustedEndStmtNum());
		}
	}
	
}