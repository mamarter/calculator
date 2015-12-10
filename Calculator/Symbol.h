/**
*	Author: Maria Marco
*	E-Mail: maria22mt@gmail.com
*	Description:
*	Symbol class is used by Calculator class to store each token. It represents each element found in a 
*	mathematical expression. A symbol can be either a number (that will be an operand) or an operator of 
*	the Calculator class. It stores a string as a member variable.
*	class Number is a subclass of Symbol and represents numbers. It stores a decimal value.
*	class Operator is a subclass of Symbol and represents an operator. It stores the type of operation
*	that performs.
*/

#ifndef _SYMBOL_H
#define _SYMBOL_H

#include <string>

using namespace std;

namespace Symbols
{
	//Accepted operators in a expression
	const string operators = "+-*/()";
	//Precedence of operators
	const int precedence[] = { 0, 0, 1, 1, -1, -1};
	enum OP{
		ADD,
		SUB,
		MULT,
		DIV,
		LPAR,
		RPAR
	};
	const int ERROR = -1;
}

class Symbol
{
public:

	Symbol(const string& inputStr) : mStr(inputStr) {}
	~Symbol(){}

	virtual bool IsNumber() { return false; }
	virtual bool IsParenthesis() const { return false; }
	virtual bool IsLeftParenthesis() const { return false; }
	virtual bool IsSignOperator() const { return false; }

	static bool IsNumber(const string& symbol);
	static bool IsOperator(const string& symbol, Symbols::OP& type);

protected:
	string mStr;
};

class Number : public Symbol
{
public:
	Number(double number) : Symbol(to_string(number)), mNumber(number) {}
	Number(const string& inputStr) : Symbol(inputStr), mNumber(stod(inputStr)) {}

	virtual bool IsNumber() { return true; }
	double GetValue() const { return mNumber; }
	void SetValue(double value) { mNumber = value; }

private:
	double mNumber;
};

class Operator : public Symbol
{
public:
	Operator(const string& inputStr);
	Operator(const string& inputStr, Symbols::OP type) : Symbol(inputStr), mType(type) {}
	
	Symbols::OP GetType() const { return mType; }
	virtual bool IsParenthesis() const { return mType == Symbols::RPAR || mType == Symbols::LPAR; }
	virtual bool IsSignOperator() const { return mType == Symbols::ADD || mType == Symbols::SUB; }
	virtual bool IsLeftParenthesis() const { return mType == Symbols::LPAR; }

	/** Method that compares precedence of two operators
	*  @param otherOp is the the operator that has to be compared to
	*  @return a boolean if the object's precendece is less or equal than the other operator
	*/
	bool HasLessOrEqualPrecedence(const Operator* otherOp) const; 

	/** Method that performs the corresponding mathematical operation depending of the type
	*	of operator of the object
	*  @param leftOp is the left operand
	*  @param rightOp is the right operand
	*  @param result is a pointer to a Number object where the result has to be stored
	*  @return a boolean whether the operation has been successfuly performed or not
	*/
	bool Calculate(const Number* leftOp, const Number* rightOp, Number* result) const;

private:
	Symbols::OP mType;

};

#endif