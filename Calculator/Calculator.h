
#include <string>
#include <vector>

using namespace std;
enum
{
	ADD_LEVEL = 0,
	SUB_LEVEL = 0,
	MULT_LEVEL = 1,
	DIV_LEVEL = 1
};
enum{
	ADD = '+',
	SUB = '-',
	MULT = '*',
	DIV = '/'
};

class Node
{
public:
	Node();
	Node(int value);
	~Node();
	void SetValue(int value) { mValue = value; }
	int GetValue() const { return mValue; }

	void SetRightNode(Node* right) { mRight = right; }
	Node* GetRightNode() const { return mRight; }

	void SetLeftNode(Node* left){ mLeft = left; }
	Node* GetLeftNode() const { return mLeft; }

	void AddOperation(){}
	void EvaluateChildren(){}

private:
	int mValue;
	Node* mRight;
	Node* mLeft;

};

class Calculator
{
public:

	Calculator();
	~Calculator();

	void ProcessExpression(const string& input){}
	int Tokenize(const string& input, vector<string>& tokens);
	void ConstructExpressionTree(){}
	void EvaluateCurrentExpression(){}
	void ShowResult(){}

private:

	Node* mOperationsRoot;

};



