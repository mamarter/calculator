#include <string>

using namespace std;

namespace Symbols
{
	const string operators = "+-*/()";
	const int precedence[] = { 0, 0, 1, 1, -1, -1, -1 };
	enum OP{
		ADD,
		SUB,
		MULT,
		DIV,
		LPAR,
		RPAR,
		NUMBER,
		UNDEFINED = -1
	};
	const int ERROR = -1;
}

class Symbol
{
public:

	Symbol() : mOp(Symbols::UNDEFINED), mStr("") {}
	Symbol(Symbols::OP op) : mOp(op), mStr(to_string(Symbols::operators[op])){}
	Symbol(const string& str)
		:mStr(str) {
		size_t pos = Symbols::operators.find(mStr);
		mOp = static_cast<Symbols::OP>(pos);
	}
	Symbol(const string& str, Symbols::OP op)
		:mStr(str), mOp(op) {}
	~Symbol(){}

	bool isNumber() { return mOp == Symbols::NUMBER; }

	const string& GetString() const { return mStr; }
	const string& SetString() const { return mStr; }
	Symbols::OP GetType() const { return mOp; }
	bool isParenthesis() const { return mOp == Symbols::RPAR || mOp == Symbols::LPAR; }

	static bool CheckNumber(const string& symbol);
	static bool CheckOperator(const string& symbol, Symbols::OP& type);

protected:
	string mStr;
	Symbols::OP mOp;
};

class Number : public Symbol
{
public:
	Number(const string& str);
	Number() : Symbol(Symbols::NUMBER), mNumber(0.f) {}
	Number(double number) :mNumber(number), Symbol(to_string(number), Symbols::NUMBER){}


	double GetValue() const { return mNumber; }
	void SetValue(double value) { mNumber = value; }

private:
	double mNumber;
};

class Operator : public Symbol
{
public:
	Operator(const string& str, Symbols::OP type) : Symbol(str, type) {}

	bool HasLessOrEqualPrecedence(const Operator* op) const { return Symbols::precedence[mOp] <= Symbols::precedence[op->GetType()]; }
	bool Calculate(const Number* op1, const Number* op2, Number* result);
	bool IsSignOperator() const { return mOp == Symbols::ADD || mOp == Symbols::SUB; }
};
