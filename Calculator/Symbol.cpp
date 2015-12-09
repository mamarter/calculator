#include <iostream>
#include <regex>
#include "Symbol.h"

Number::Number(const string& str)
: Symbol(str, Symbols::NUMBER)
{
	mNumber = stod(str);
}

bool Symbol::CheckNumber(const string& symbol)
{
	regex decimal("((([[:digit:]]+)?)\\.)?([[:digit:]]+)");
	if (regex_match(symbol, decimal))
	{
		return true;
	}
	else
	{
		cerr << "ERROR: CheckNumber() - Number '" << symbol << "' not recognised" << endl;
		return false;
	}
}

bool Symbol::CheckOperator(const string& symbol, Symbols::OP& op)
{
	size_t pos = Symbols::operators.find(symbol);
	if (pos == string::npos)
	{
		cerr << "ERROR: CheckOperator() - Symbol '" << symbol << "' not recognised" << endl;
		return false;
	}
	else
	{
		op = static_cast<Symbols::OP>(pos);
		return true;
	}
}

bool Operator::Calculate(const Number* op1, const Number* op2, Number* result)
{
	if (!op1 || !op2)
	{
		cerr << "ERROR Calculate - Invalid operands" << endl;
		return false;
	}

	if (!result)
	{
		result = new Number();
	}

	switch (mOp)
	{
	case Symbols::ADD:
		result->SetValue(op1->GetValue() + op2->GetValue());
		break;

	case Symbols::SUB:
		result->SetValue(op1->GetValue() - op2->GetValue());
		break;

	case Symbols::MULT:
		result->SetValue(op1->GetValue() * op2->GetValue());
		break;

	case Symbols::DIV:
		if (op2->GetValue() == 0.f)
		{
			cerr << "ERROR: Operator::Calculate - Division by zero" << endl;
			return false;
		}
		result->SetValue(op1->GetValue() / op2->GetValue());
		break;

	default:
		cout << "WARNING: Calculate(Number*,Number*) - Wrong Operator or not recognised" << endl;
		return false;

	}

	return true;
}