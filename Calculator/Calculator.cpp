
#include <iostream>
#include <sstream>
#include <cstring>
#include <regex>
#include <iterator>
#include <stack>
#include "Calculator.h"
#include "Symbol.h"

Calculator::~Calculator()
{
	ClearSymbolsVector();
}

void Calculator::ClearSymbolsVector()
{
	vector<Symbol*>::iterator symbolsIt;
	for (symbolsIt = mSymbolsVector.begin(); symbolsIt != mSymbolsVector.end(); symbolsIt++)
	{
		cout << "Delete " << (*symbolsIt)->GetString() << endl;
		delete *symbolsIt;
	}

	cout << "Clear mSymbolsVector" << endl;
	mSymbolsVector.clear();
}

int Calculator::ParseExpression(string& input)
{
	//Removes whitespaces
	string::iterator end_pos = remove(input.begin(), input.end(), ' ');
	input.erase(end_pos, input.end());

	//Parse string
	size_t prev = 0, current;
	Symbol* newSymbol = NULL;
	string symbolRead;
	while ((current = input.find_first_of(Symbols::operators, prev)) != string::npos)
	{
		if (current > prev) //found number
		{
			symbolRead = input.substr(prev, current - prev);
			if (Symbol::CheckNumber(symbolRead))
			{
				newSymbol = new Number(symbolRead);
				mSymbolsVector.push_back(newSymbol);
			}
			else{
				return Symbols::ERROR;
			}
		}

		symbolRead = input[current];
		Symbols::OP operatorType;
		if (Symbol::CheckOperator(symbolRead, operatorType))
		{
			newSymbol = new Operator(symbolRead, operatorType);

			if (mSymbolsVector.empty() &&
				!newSymbol->isParenthesis()) // first symbol
			{
				Symbol* zeroNumber = new Number(0.f);
				mSymbolsVector.push_back(zeroNumber);
			}
			mSymbolsVector.push_back(newSymbol);
		}
		else{
			return Symbols::ERROR;
		}

		prev = current + 1;
	}

	if (prev < input.length()) // number after last delimiter
	{
		string remainingStr = input.substr(prev, string::npos);
		if (Symbol::CheckNumber(remainingStr))
		{
			Symbol* newSymbol = new Number(remainingStr);
			mSymbolsVector.push_back(newSymbol);
		}
		else{
			return Symbols::ERROR;
		}
	}

	return mSymbolsVector.size();
}

void Calculator::EvaluateExpression(const string& inputToEvaluate)
{
	string input = inputToEvaluate;
	double result = 0;

	if (ParseExpression(input) > 0)
	{
		queue<Symbol*> reversedExpression = ConstructRPN();
		if (!reversedExpression.empty())
		{
			if (ResolveExpression(reversedExpression, result))
			{
				cout << "Result = " << result << endl;
			}
		}
	}

	ClearSymbolsVector();
}

queue<Symbol*> Calculator::ConstructRPN()
{
	stack<Operator*> operatorsStack; // temporal stack to store read operators
	queue<Symbol*> numberQueue; // queue to store expression in reverse polish notation

	Symbol* currentSym = NULL;
	Operator* currentOp = NULL;
	vector<Symbol*>::iterator symbolsIt;
	for (symbolsIt = mSymbolsVector.begin(); symbolsIt != mSymbolsVector.end(); symbolsIt++)
	{
		currentSym = *symbolsIt;
		switch (currentSym->GetType())
		{
		case Symbols::ADD:
		case Symbols::SUB:
		case Symbols::MULT:
		case Symbols::DIV:
			currentOp = static_cast<Operator*>(currentSym);
			if (currentOp)
			{
				Operator* topOperator = operatorsStack.empty() ? NULL : operatorsStack.top();
				while (topOperator &&
					currentOp->HasLessOrEqualPrecedence(topOperator))
				{
					operatorsStack.pop();
					numberQueue.push(topOperator);
					topOperator = operatorsStack.empty() ? NULL : operatorsStack.top();
				}
				operatorsStack.push(currentOp);
			}
			break;
		case Symbols::LPAR:
			currentOp = static_cast<Operator*>(currentSym);
			if (currentOp)
			{
				operatorsStack.push(currentOp);
			}
			break;
		case Symbols::RPAR:
			currentOp = static_cast<Operator*>(currentSym);
			if (currentOp)
			{
				Operator* topOperator = operatorsStack.empty() ? NULL : operatorsStack.top();
				while (topOperator &&
					topOperator->GetType() != Symbols::LPAR)
				{
					operatorsStack.pop();
					numberQueue.push(topOperator);
					topOperator = operatorsStack.empty() ? NULL : operatorsStack.top();
				}

				if (topOperator) // left parenthesis found
				{
					operatorsStack.pop();
				}
				else
				{
					cerr << "ERROR: Calculator::ConstructRPN() - Unbalanced parenthesis" << endl;
				}
			}
			break;
		case Symbols::NUMBER:
			numberQueue.push(currentSym);
			break;
		default:
			cout << "WARNING: Calculator::ConstructRPN() - Type of symbol not set" << endl;
		}
	}

	while (!operatorsStack.empty())
	{
		numberQueue.push(operatorsStack.top());
		operatorsStack.pop();
	}

	return numberQueue;
}

bool Calculator::ResolveExpression(queue<Symbol*>& symbolsQueue, double& result)
{
	stack<Number*> numbersStack;
	Number* currNumber = NULL;
	Number* leftOp = NULL;
	Number* rightOp = NULL;
	Operator* currOperator = NULL;
	while (!symbolsQueue.empty())
	{
		Symbol* current = symbolsQueue.front();
		symbolsQueue.pop();
		if (current->isNumber()) // is number
		{
			currNumber = static_cast<Number*>(current);
			if (currNumber)
			{
				numbersStack.push(currNumber);
			}
		}
		else // is operator
		{
			currOperator = static_cast<Operator*>(current);
			if (currOperator)
			{
				if (numbersStack.size() > 1) // can pop two elements
				{
					rightOp = numbersStack.top();
					// remove one of the elements (2nd operand)
					numbersStack.pop();
					leftOp = numbersStack.top();
					// keep the other one in the stack to store the result
					if (!currOperator->Calculate(leftOp, rightOp, leftOp))
					{
						return false;
					}
				}
				else // more operators than needed
				{
					cerr << "ERROR ResolveExpression() - Too many operators" << endl;
					return false;
				}
			}
		}
	}

	if (numbersStack.size() == 1)
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
		cerr << "ERROR ResolveExpression() - Not enough operators to solve the expression" << endl;
	}

	return false;
}
