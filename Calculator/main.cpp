
#include <iostream>

#include "Calculator.h"
#include "Symbol.h"

int main()
{
	Calculator calculator;
	string input;

	while (getline(cin, input))
	{
		calculator.EvaluateExpression(input);
	}

	return EXIT_SUCCESS;
}