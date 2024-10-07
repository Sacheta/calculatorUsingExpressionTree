#include <iostream>
#include <stack>
#include <string>
#include <stdexcept>
using namespace std;

class Node {
protected:
    string val;
public:
    Node* left;

    Node* right;
    Node(string vall) : val(vall), left(nullptr), right(nullptr) {}

    virtual void printVal() {
        cout << val << endl;
    }
    virtual int evaluate() = 0;
};

// OperatorNode handles operators like +, -, *, /
class OperatorNode : public Node {
public:
    OperatorNode(string vall) : Node(vall) {}

    int evaluate() override {
        int leftValue = left->evaluate();   // Recursively evaluate the left subtree
        int rightValue = right->evaluate(); // Recursively evaluate the right subtree
        if (val == "+") {
            return leftValue + rightValue;
        }
        else if (val == "-") {
            return leftValue - rightValue;
        }
        else if (val == "*") {
            return leftValue * rightValue;
        }
        else if (val == "/") {
            if (rightValue == 0) throw runtime_error("Division by zero error.");
            return leftValue / rightValue;
        }
        else {
            throw runtime_error("Unknown operator");
        }
    }
};

// OperandNode handles operands (numbers)
class OperandNode : public Node {
public:
    OperandNode(string vall) : Node(vall) {}

    int evaluate() override {
        return stoi(val);
    }
};

// Utility function to get operator precedence
int getPrecedence(char op) {
    switch (op) {
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
Node* createExpTree(const string& exp) {
    stack<char> operatorSt;     // To hold operators
    stack<Node*> operandSt;     // To hold operands (leaf nodes)

    unsigned int size = (unsigned int)exp.size();
    for (unsigned int i = 0; i < size; ++i) {
        if (isspace(exp[i])) {
            continue;
        }
        

        // Parse operands (numbers)
        if (isdigit(exp[i])) {
            string operand = "";
            while (i < exp.size() && isdigit(exp[i])) {
                operand += exp[i];
                i++;
            }
            --i;
            operandSt.push(new OperandNode(operand));
            continue;
        }

        // Handle opening parenthesis
        if (exp[i] == '(') {
            operatorSt.push(exp[i]);
            continue;
        }

        // Handle closing parenthesis
        if (exp[i] == ')') {
            while (!operatorSt.empty() && operatorSt.top() != '(') {
                Node* right = operandSt.top(); operandSt.pop();
                Node* left = operandSt.top(); operandSt.pop();
                char op = operatorSt.top(); operatorSt.pop();

                Node* newNode = new OperatorNode(string(1, op));
                newNode->left = left;
                newNode->right = right;
                operandSt.push(newNode);
            }
            operatorSt.pop();  // Pop the '('
            continue;
        }

        // Handle operators (+, -, *, /)
        if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') {
            while (!operatorSt.empty() && getPrecedence(exp[i]) <= getPrecedence(operatorSt.top())) {
                // Pop two operands and one operator to create a new subtree
                Node* right = operandSt.top(); operandSt.pop();
                Node* left = operandSt.top(); operandSt.pop();
                char op = operatorSt.top(); operatorSt.pop();

                Node* newNode = new OperatorNode(string(1, op));
                newNode->left = left;
                newNode->right = right;
                operandSt.push(newNode);
            }
            operatorSt.push(exp[i]);
        }
    }

    while (!operatorSt.empty()) {
        Node* right = operandSt.top(); operandSt.pop();
        Node* left = operandSt.top(); operandSt.pop();
        char op = operatorSt.top(); operatorSt.pop();

        Node* newNode = new OperatorNode(string(1, op));
        newNode->left = left;
        newNode->right = right;
        operandSt.push(newNode);
    }

    return operandSt.top();
}

int main() {
    string exp;
    cout << "ENTER INPUT: ";
    getline(cin, exp);

    try {
        Node* root = createExpTree(exp);
        cout << "The result of the expression is: " << root->evaluate() << endl;
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
