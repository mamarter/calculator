
#include <iostream>

#include "Calculator.h"


int main()
{
	Calculator calculator;
	string str = "1+ 2*3- 4";
	vector<string> str_tokenized;
	int num = calculator.Tokenize(str, str_tokenized);
	
	while (true)
	{

	}

	return EXIT_SUCCESS;
}