#include <string>
#include <vector>
#include <queue>

using namespace std;

class Symbol;

class Calculator
{
public:

	Calculator(){}
	~Calculator();

	// Function that completes the evaluation of a mathematical expression
	void EvaluateExpression(const string& inputToEvaluate);

private:

	// Clears symbols from memory
	void ClearSymbolsVector();
	
	int ParseExpression(const string& input);

	// convert from infix notation relying on order of operations to 
	// Reverse Polish notation (RPN)
	queue<Symbol*> ConstructRPN();

	bool ResolveExpression(queue<Symbol*>& symbolsQueue, double& result);

	vector<Symbol*> mSymbolsVector;
};
