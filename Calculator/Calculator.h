/**
*	Author: Maria Marco
*	E-Mail: maria22mt@gmail.com
*	Description:
*	The Calculator class processes mathematical expressions formed by operators and operands called symbols in this project.
*	The currently accepted symbols are addition, substraction, multiplication, division, parenthesis and decimal numbers.
*	To evaluate a string of operations it first converts the expression from infix notation to reverse polish
*	notation (RPN) relying on order of operations (based on the Shunting-yard algorithm). Then resolves the equation
*	storing intermediate results in a stack.
*/

#ifndef _CALCULATOR_H
#define _CALCULATOR_H

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

	/** Method that evaluates a mathematical expression and prints the result
	*  to the standard output
	*  @param inputToEvaluate is a line read from the standard input
	*/
	void EvaluateExpression(const string& inputToEvaluate);

private:

	/** Deletes symbols from memory and clears the symbols vector
	*/
	void clearSymbolsVector();
	
	/** Method that parses a mathematical expression and stores it into a vector of symbols. 
	*  It accepts all operators stored in Symbols::operators. It checks the validity of the 
	*  input and creates an object for each symbol (that can be either a Number or an Operator)
	*  @param inputToEvaluate is the expression string that has to be parsed
	*  @return the number of symbols that have been correctly created
	*/
	int parseExpression(const string& inputToEvaluate);

	/** Method that converts a mathematical expression from infix notation 
	* to reverse polish notation (RPN) relying on order of operations. It is based on the 
	* Shunting-yard algorithm
	*  @param symbolQueue is the queue  that the function has to fill with the processed 
	*  expression in reverse polish notation 
	*  @return the number of symbols that have been correctly added to the queue
	*/
	int constructRPN(queue<Symbol*>& symbolQueue);

	/** Method that processes a mathematical expression in reverse polish notation (RPN) 
	* using a temporary stack to store operands
	*  @param symbolQueue is a queue of processed symbols in reverse polish notation that has to be resolved
	*  @param result is the variable where the expression's result has to be stored
	*  @return a boolean whether the expression has been successfuly processed or not
	*/
	bool resolveExpression(queue<Symbol*>& symbolsQueue, double& result);

	vector<Symbol*> mSymbolsVector;
};

#endif