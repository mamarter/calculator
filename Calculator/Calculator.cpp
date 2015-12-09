#include "Calculator.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <regex>
#include <iterator>

Calculator::Calculator()
	: mOperationsRoot(NULL)
{

}

Calculator::~Calculator()
{

}

int Calculator::Tokenize(const string& input, vector<string>& tokens)
{
	string str = "1 +2 * 3 - 4";
	//Removes whitespaces
	string::iterator end_pos = remove(str.begin(), str.end(), ' ');
	str.erase(end_pos, str.end());
	
	
	const char* sep = " +-*/";

	// Simple regular expression matching
	//regex base_regex("[[:digit:]]+"); //([\ + \ - \*\ / ])*
	regex base_regex("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?(\\+\\-)?");
	cout << str << ": " << std::regex_match(str, base_regex) << endl;

	// Extraction of a sub-match
	//(\d+)([\+\-\*\/])*
	smatch base_match;

	if (std::regex_match(str, base_match, base_regex)) {
			// The first sub_match is the whole string; the next
			// sub_match is the first parenthesized expression.
			if (base_match.size() == 2) {
				std::ssub_match base_sub_match = base_match[1];
				std::string base = base_sub_match.str();
				std::cout << str << " has a base of " << base << '\n';
			}
		}

	return tokens.size();
}
