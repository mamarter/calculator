
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
	
	void EvaluateExpression(const string& inputToEvaluate);

private:

	void ClearSymbolsVector();
	int ParseExpression(string& input);

	// convert from infix notation relying on order of operations to 
	// Reverse Polish notation (RPN)
	queue<Symbol*> ConstructRPN();

	bool ResolveExpression(queue<Symbol*>& symbolsQueue, double& result);

	vector<Symbol*> mSymbolsVector;
};



