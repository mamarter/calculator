
#include <iostream>
#include <iterator>
#include <stack>
#include "Calculator.h"
#include "Symbol.h"

Calculator::~Calculator()
{
	clearSymbolsVector();
}

void Calculator::EvaluateExpression(const string& inputToEvaluate)
{
	// First the expression is parsed and stored in a vector of symbols
	if (parseExpression(inputToEvaluate) > 0)
	{
		// The vector of symbols from the input is stored in infix notation
		// Reorder the expression to reverse polish notation (RPN) in order 
		// to be able to evaluate it
		queue<Symbol*> RPNExpression;
		// Expression in RPN will be stored in the queue RPNExpression
		if (constructRPN(RPNExpression) > 0) 
		{
			double result = 0;
			if (resolveExpression(RPNExpression, result)) // Evaluates RPNExpression and gets result
			{
				cout << "Result = " << result << endl;
			}
			else
			{
				cerr << "ERROR: resolveExpression() - Error processing expression '";
				cerr << inputToEvaluate << "'" << endl;
			}
		}
		else
		{
			cerr << "ERROR: constructRPN() - Error constructing expression in RPN '";
			cerr << inputToEvaluate << "'" << endl;
		}
	}
	else
	{
		cerr << "ERROR: parseExpression() - Error parsing expression '";
			cerr << inputToEvaluate << "'" << endl;
	}

	// Clear symbols no matter it has been successful or not
	clearSymbolsVector();
}

int Calculator::parseExpression(const string& inputToEvaluate)
{
	string input = inputToEvaluate;
	//Remove whitespaces
	string::iterator end_pos = remove(input.begin(), input.end(), ' ');
	input.erase(end_pos, input.end());

	//Parse string
	size_t prev = 0, current;
	string inputSymbol;
	while ((current = input.find_first_of(Symbols::operators, prev)) != string::npos)
	{
		if (current > prev) //found number
		{
			inputSymbol = input.substr(prev, current - prev);
			if (Symbol::IsNumber(inputSymbol))
			{
				mSymbolsVector.push_back(new Number(inputSymbol)); // Number is subclass of Symbol
			}
			else{
				cerr << "ERROR: IsNumber() - Number '" << inputSymbol << "' not recognised" << endl;
				return Symbols::ERROR;
			}
		}
		
		// read symbol
		inputSymbol = input[current];
		Symbols::OP operatorType;
		if (Symbol::IsOperator(inputSymbol, operatorType))
		{
			Symbol* symbolObject = new Operator(inputSymbol, operatorType); // subclass of Symbol
			
			// Add 0 to the vector if the first symbol found is an operator
			const bool firstSymbolIsOperator = mSymbolsVector.empty() && !symbolObject->IsParenthesis();  
			// Add 0 to the vector when a expression inside parenthesis starts with an operator (sign operator)
			const bool symbolAfterParenthesis = mSymbolsVector.size() > 1 && 
												mSymbolsVector[mSymbolsVector.size() - 1]->IsLeftParenthesis() && 
												symbolObject->IsSignOperator();
			if (firstSymbolIsOperator || symbolAfterParenthesis)
			{
				mSymbolsVector.push_back(new Number(0.f)); 
			}
			mSymbolsVector.push_back(symbolObject);
		}
		else{
			cerr << "ERROR: IsOperator() - Symbol '" << inputSymbol << "' not recognised" << endl;
			return Symbols::ERROR;
		}

		prev = current + 1;
	}

	// No more delimiters found but there are still unread chars from the input string
	if (prev < input.length()) 
	{
		string remainingStr = input.substr(prev, string::npos);
		if (Symbol::IsNumber(remainingStr))
		{
			mSymbolsVector.push_back(new Number(remainingStr));
		}
		else{
			cerr << "ERROR: IsNumber() - Number '" << remainingStr << "' not recognised" << endl;
			return Symbols::ERROR;
		}
	}

	return mSymbolsVector.size();
}

int Calculator::constructRPN(queue<Symbol*>& symbolQueue)
{
	stack<Operator*> operatorsStack; // temporary stack to prioritise operators by precedence

	// Iterate through the symbols vector and process each one
	vector<Symbol*>::iterator symbolsIt;
	for (symbolsIt = mSymbolsVector.begin(); symbolsIt != mSymbolsVector.end(); symbolsIt++)
	{
		Symbol* currentSym = *symbolsIt;
		if (currentSym->IsNumber())
		{
			symbolQueue.push(currentSym);
		}
		else
		{
			Operator* currentOp = dynamic_cast<Operator*>(currentSym);
			if (!currentOp)
			{
				continue;
			}

			switch (currentOp->GetType())
			{
				case Symbols::ADD:
				case Symbols::SUB:
				case Symbols::MULT:
				case Symbols::DIV:
				{
					// If currentOp has less or equal precedence than the operators already stored in the operatorsStack
					// pop these operators from the stack and push them into the symbolQueue. This means operators with 
					// higher or equal precedence need to be processed first
					Operator* topOperator = operatorsStack.empty() ? NULL : operatorsStack.top();
					while (topOperator &&
						currentOp->HasLessOrEqualPrecedence(topOperator))
					{
						operatorsStack.pop();
						symbolQueue.push(topOperator);
						topOperator = operatorsStack.empty() ? NULL : operatorsStack.top();
					}

					operatorsStack.push(currentOp);
					break;
				}
				case Symbols::LPAR:
				{
					operatorsStack.push(currentOp);
					break;
				}
				case Symbols::RPAR:
				{
					// Pop operators from operatorsStack and push them to the symbolQueue until left 
					// parenthesis is found
					Operator* topOperator = operatorsStack.empty() ? NULL : operatorsStack.top();
					while (topOperator &&
						!topOperator->IsLeftParenthesis())
					{
						operatorsStack.pop();
						symbolQueue.push(topOperator);
						topOperator = operatorsStack.empty() ? NULL : operatorsStack.top();
					}

					if (topOperator) // left parenthesis found
					{
						operatorsStack.pop();
					}
					else
					{
						cerr << "ERROR: Calculator::constructRPN() - Unbalanced parenthesis" << endl;
						return Symbols::ERROR;
					}
					break;
				}
				default:
					cout << "WARNING: Calculator::constructRPN() - Type of symbol not set" << endl;
			}
		}
	}

	// Push remaining operators from operatorsStack to the symbolQueue
	while (!operatorsStack.empty())
	{
		Symbol* symbol = operatorsStack.top();
		if (symbol->IsParenthesis())
		{
			cerr << "ERROR: Calculator::constructRPN() - Unbalanced parenthesis" << endl;
			return Symbols::ERROR;
		}
		symbolQueue.push(symbol);
		operatorsStack.pop();
	}

	return symbolQueue.size();
}

bool Calculator::resolveExpression(queue<Symbol*>& symbolsQueue, double& result)
{
	// Process elements from symbolQueue 
	stack<Number*> numbersStack; // stack to store numbers from the symbolsQueue and intermediate results from operations
	while (!symbolsQueue.empty())
	{
		Symbol* current = symbolsQueue.front();
		symbolsQueue.pop();
		// Push numbers directly to the stack
		if (current->IsNumber()) // is number
		{
			Number* currNumber = static_cast<Number*>(current);
			if (currNumber)
			{
				numbersStack.push(currNumber);
			}
		}
		else // is operator - perform operation with first two numbers of the stack
		{
			const Operator* currOperator = static_cast<Operator*>(current);
			if (currOperator)
			{
				if (numbersStack.size() > 1) // can pop two elements
				{
					Number* rightOp = numbersStack.top();
					// pop one of the elements (right operand)
					numbersStack.pop();
					Number* leftOp = numbersStack.top();
					// keep the other one in the stack to store the result
					if (!currOperator->Calculate(leftOp, rightOp, leftOp))
					{
						return false;
					}
				}
				else // more operators than needed, not enough numbers to process
				{
					cerr << "ERROR resolveExpression() - Too many operators" << endl;
					return false;
				}
			}
		}
	}

	if (numbersStack.size() == 1) // the result remains in the top of the stack
	{
		Number* resultNumber = numbersStack.top();
		if (resultNumber)
		{
			result = numbersStack.top()->GetValue();
			return true;
		}
	}
	else
	{
		cerr << "ERROR resolveExpression() - Not enough operators to solve the expression" << endl;
	}

	return false;
}

void Calculator::clearSymbolsVector()
{
	vector<Symbol*>::iterator symbolsIt;
	for (symbolsIt = mSymbolsVector.begin(); symbolsIt != mSymbolsVector.end(); symbolsIt++)
	{
		delete *symbolsIt;
	}

	mSymbolsVector.clear();
}