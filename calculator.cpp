#include <iostream>
#include <stack>
#include <string>
#include <stdexcept>
using namespace std;

class Node
{
protected:
    string val;

public:
    Node *left;
    Node *right;

    // constructor to initialise the expression tree node
    Node(string val) : val(val), left(nullptr), right(nullptr) {}
    // pure virtual function to be defined by the child classes : operator nodes and operand nodes
    virtual int evaluate() = 0;
};

class OperatorNode : public Node
{
public:
    // constructor to initialise the expression tree Operator node
    OperatorNode(string val) : Node(val) {}

    // overridden function to recursively evaluate the expression tree
    int evaluate() override
    {
        int leftValue = left->evaluate();   // Recursively evaluate the left subtree
        int rightValue = right->evaluate(); // Recursively evaluate the right subtree
        try
        {
            if (val == "+")
            {
                return leftValue + rightValue;
            }
            else if (val == "-")
            {
                return leftValue - rightValue;
            }
            else if (val == "*")
            {
                return leftValue * rightValue;
            }
            else if (val == "/")
            {
                return leftValue / rightValue;
            }
        }
        catch (exception e)
        {
            cout << e.what() << endl;
        }
    }
};

class OperandNode : public Node
{
public:
    // constructor to initialise the expression tree Operand nodes
    OperandNode(string val) : Node(val) {}

    // overridden function to evaluate the operand nodes: just returning the integer equivalants
    int evaluate() override
    {
        return stoi(val);
    }
};

// Utility function to get operator precedence
int getPrecedence(char op)
{
    switch (op)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    default:
        return 0;
    }
}

// Function to create the expression tree from an infix expression
Node *createExpTree(const string &exp)
{
    stack<char> operatorSt;  // To hold operators
    stack<Node *> operandSt; // To hold expression tree nodes

    unsigned int size = (unsigned int)exp.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        if (isspace(exp[i]))
        { // continue if current char is a space
            continue;
        }

        if (isdigit(exp[i]))
        { // case when there is an operand: get the whole operand, create OperandNode and push it on the operand stack
            string operand = "";
            while (i < exp.size() && isdigit(exp[i]))
            {
                operand += exp[i];
                i++;
            }
            --i;
            operandSt.push(new OperandNode(operand));
            continue;
        }

        // case when there is an opening paranthesis: push it on the operator stack
        if (exp[i] == '(')
        {
            operatorSt.push(exp[i]);
            continue;
        }

        // case when there is a closing paranthesis: get all the operators until opening parenthesis found, make the expression subtrees, push onto the operantStack
        if (exp[i] == ')')
        {
            while (!operatorSt.empty() && operatorSt.top() != '(')
            {
                Node *right = operandSt.top();
                operandSt.pop();
                Node *left = operandSt.top();
                operandSt.pop();
                char op = operatorSt.top();
                operatorSt.pop();

                Node *newNode = new OperatorNode(string(1, op));
                newNode->left = left;
                newNode->right = right;
                operandSt.push(newNode);
            }
            operatorSt.pop(); // Pop the '('
            continue;
        }

        // Handle operators (+, -, *, /)
        if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/')
        {
            while (!operatorSt.empty() && getPrecedence(exp[i]) <= getPrecedence(operatorSt.top()))
            { // case when higher/equal precedence op is on top of stack
                Node *right = operandSt.top();
                operandSt.pop();
                Node *left = operandSt.top();
                operandSt.pop();
                char op = operatorSt.top();
                operatorSt.pop();

                Node *newNode = new OperatorNode(string(1, op));
                newNode->left = left;
                newNode->right = right;
                operandSt.push(newNode);
            }
            operatorSt.push(exp[i]);
        }
    }

    while (!operatorSt.empty())
    { // to handle the remaining operators in the op stack
        Node *right = operandSt.top();
        operandSt.pop();
        Node *left = operandSt.top();
        operandSt.pop();
        char op = operatorSt.top();
        operatorSt.pop();

        Node *newNode = new OperatorNode(string(1, op));
        newNode->left = left;
        newNode->right = right;
        operandSt.push(newNode);
    }

    return operandSt.top();
}

int main()
{
    string exp;
    cout << "ENTER INPUT: ";
    getline(cin, exp);
    Node *root = createExpTree(exp);
    cout << "The result of the expression is: " << root->evaluate() << endl;
    return 0;
}
