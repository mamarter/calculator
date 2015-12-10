
#include <iostream>
#include <regex>
#include "Symbol.h"

bool Symbol::IsNumber(const string& symbol)
{
	// Check for decimal numbers
	regex decimal("((([[:digit:]]+)?)\\.)?([[:digit:]]+)");
	return regex_match(symbol, decimal);
}

bool Symbol::IsOperator(const string& symbol, Symbols::OP& op)
{
	// Check if the operator is in the string of accepted operators
	size_t pos = Symbols::operators.find(symbol);
	if (pos == string::npos)
	{
		return false;
	}
	else
	{
		op = static_cast<Symbols::OP>(pos);
		return true;
	}
}

Operator::Operator(const string& inputStr)
	: Symbol(inputStr)
{
	size_t pos = Symbols::operators.find(mStr);
	mType = static_cast<Symbols::OP>(pos);
}

bool Operator::HasLessOrEqualPrecedence(const Operator* otherOp) const
{
	return Symbols::precedence[mType] <= Symbols::precedence[otherOp->GetType()];
}

bool Operator::Calculate(const Number* op1, const Number* op2, Number* result) const
{
	if (!op1 || !op2 || !result)
	{
		cerr << "ERROR Operator::Calculate() - NULL operand" << endl;
		return false;
	}

	switch (mType)
	{
		case Symbols::ADD:
		{
			result->SetValue(op1->GetValue() + op2->GetValue());
			break;
		}
		case Symbols::SUB:
		{
			result->SetValue(op1->GetValue() - op2->GetValue());
			break;
		}
		case Symbols::MULT:
		{
			result->SetValue(op1->GetValue() * op2->GetValue());
			break;
		}
		case Symbols::DIV:
		{
			if (op2->GetValue() == 0.f)
			{
				cerr << "ERROR: Operator::Calculate() - Division by zero" << endl;
				return false;
			}
			result->SetValue(op1->GetValue() / op2->GetValue());
			break;
		}
		default:
		{
			cout << "ERROR: Operator::Calculate() - Wrong Operator or not recognised" << endl;
			return false;
		}
	}

	return true;
}