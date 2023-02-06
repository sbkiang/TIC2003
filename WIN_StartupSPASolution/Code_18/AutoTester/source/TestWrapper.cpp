#include "TestWrapper.h"
#include "../../source/SourceProcessor.h"
#include "../../source/Database.h"
#include "../../source/QueryProcessor.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// read the source program from the file
	ifstream fs(filename);
	stringstream buffer;
	buffer << fs.rdbuf();
	string program = buffer.str();

	// call the source processor to process it
	SourceProcessor sp;
	sp.process(program);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
	vector<string> output;

	// call the query processor to process a single query
	QueryProcessor qp;
	qp.evaluate(query, output);

	// convert a vector of results to a list of results
	for (string result : output) {
		results.push_back(result);
	}
}


int main() {
	if (1 == 1) {
		if (2 == 2) {
			int a = 1;
			a = 1 + 1;
		}
		else {
			int b = 1;
			b = 1 + 1;
		}
	}
	else {
		int c = 1;
		c++;
	}
	int d = 1;

	TestWrapper* tp = new TestWrapper();
	cout << "=========== iteration2_CFG_Nested_Condition_L1_While_If_Parent_Stmt_Start_End.txt ===========" << endl;
	tp->parse("../../Tests_18/iteration2_CFG_Nested_Condition_L1_While_If_Parent_Stmt_Start_End.txt");
	cout << "=========== iteration2_CFG_Nested_Condition_L1_While_If_Parent_Stmt_None.txt ===========" << endl;
	tp->parse("../../Tests_18/iteration2_CFG_Nested_Condition_L1_While_If_Parent_Stmt_None.txt"); 
	cout << "=========== iteration2_CFG_Nested_Condition_L1_While_If_Parent_Stmt_End_correct.txt ===========" << endl;
	tp->parse("../../Tests_18/iteration2_CFG_Nested_Condition_L1_While_If_Parent_Stmt_End_correct.txt");
}