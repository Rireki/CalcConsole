/*
  Name: CALCULATOR v1.0
  Author: Aldo Ziflaj
  Date: 01-01-2013 12:01
  Description: A simple calculator that can add, subtract multiply, divide,
               raise to n-th integer power and calculate the factorial

 Name: Calculator v1.1
 Author: Riri Furikawa
 Date: 25-05-2024 18:19
 Description: Added exception if anything except number and arithmetic operations were entered

 Name: Calculator v1.2
 Author: Riri Furikawa
 Date: 14-06-2024 13:48
 Description: Complete refactor. Added comments for ease of understanding. Added multiple expression support.
*/

#include <iostream> // Include input-output stream library
#include <string> // Include string library
#include <sstream> // Include string stream library
#include <stack> // Include stack library
#include <vector> // Include vector library
#include <cctype> // Include character handling library
#include <cmath> // Include math functions library
#include <stdexcept> // Include standard exceptions library

using namespace std; // Use the standard namespace

// Function to calculate the factorial of a number
unsigned int factorial(int a) {
    int r = 1; // Start with 1
    for (int i = a; i > 0; i--) r *= i; // Multiply r by each number from a down to 1
    return r; // Return the result
}

// Function to determine the precedence of an operator
int precedence(char op) {
    if (op == '+' || op == '-') return 1; // + and - have precedence 1
    if (op == '*' || op == '/') return 2; // * and / have precedence 2
    if (op == '^') return 3; // ^ has precedence 3
    return 0; // Default precedence
}

// Function to apply an operator to two numbers
double applyOp(double a, double b, char op) {
    switch (op) { // Apply the operator based on the type
    case '+': return a + b; // Addition
    case '-': return a - b; // Subtraction
    case '*': return a * b; // Multiplication
    case '/': return a / b; // Division
    case '^': return pow(a, b); // Power
    }
    return 0; // Default return
}

// Function to split an expression into tokens (numbers and operators)
vector<string> tokenize(string expr) {
    vector<string> tokens; // Vector to store tokens
    string number; // String to store current number
    for (char& c : expr) { // Loop through each character in the expression
        if (isdigit(c) || c == '.') { // If the character is a digit or a dot
            number += c; // Add it to the current number
        }
        else { // If the character is not a digit
            if (!number.empty()) { // If there is a current number
                tokens.push_back(number); // Add the number to tokens
                number.clear(); // Clear the current number
            }
            if (!isspace(c)) { // If the character is not a space
                tokens.push_back(string(1, c)); // Add the character as a token
            }
        }
    }
    if (!number.empty()) { // If there is a remaining number
        tokens.push_back(number); // Add the number to tokens
    }
    return tokens; // Return the tokens
}

// Function to convert tokens from infix to postfix notation
vector<string> toPostfix(vector<string> tokens) {
    vector<string> output; // Vector to store output tokens
    stack<string> operators; // Stack to store operators
    for (string& token : tokens) { // Loop through each token
        if (isdigit(token[0]) || (token[0] == '.' && token.size() > 1)) { // If the token is a number
            output.push_back(token); // Add it to the output
        }
        else if (token == "(") { // If the token is an open parenthesis
            operators.push(token); // Push it onto the stack
        }
        else if (token == ")") { // If the token is a close parenthesis
            while (!operators.empty() && operators.top() != "(") { // While the top of the stack is not an open parenthesis
                output.push_back(operators.top()); // Add the operator to the output
                operators.pop(); // Pop the operator from the stack
            }
            if (!operators.empty()) { // If there is an open parenthesis on the stack
                operators.pop(); // Pop the open parenthesis
            }
        }
        else { // If the token is an operator
            while (!operators.empty() && precedence(operators.top()[0]) >= precedence(token[0])) { // While the stack is not empty and the precedence of the top operator is higher or equal
                output.push_back(operators.top()); // Add the operator to the output
                operators.pop(); // Pop the operator from the stack
            }
            operators.push(token); // Push the current operator onto the stack
        }
    }
    while (!operators.empty()) { // While the stack is not empty
        output.push_back(operators.top()); // Add the remaining operators to the output
        operators.pop(); // Pop the operators from the stack
    }
    return output; // Return the output tokens
}

// Function to evaluate a postfix expression
double evaluatePostfix(vector<string> tokens) {
    stack<double> values; // Stack to store values
    for (string& token : tokens) { // Loop through each token
        if (isdigit(token[0]) || (token[0] == '.' && token.size() > 1)) { // If the token is a number
            values.push(stod(token)); // Convert it to a double and push it onto the stack
        }
        else { // If the token is an operator
            if (values.size() < 2) { // If there are less than 2 values on the stack
                throw runtime_error("Invalid expression"); // Throw an error
            }
            double val2 = values.top(); values.pop(); // Pop the top value
            double val1 = values.top(); values.pop(); // Pop the next top value
            values.push(applyOp(val1, val2, token[0])); // Apply the operator and push the result onto the stack
        }
    }
    if (values.size() != 1) { // If there is not exactly one value on the stack
        throw runtime_error("Invalid expression"); // Throw an error
    }
    return values.top(); // Return the result
}

int main() {
    double memory = 0; // Variable to store memory
    string expr, choice; // Strings to store the expression and user choices

    cout << "+ to add, - to subtract, * to multiply, / to divide, ^ to power, ! to factorial\n"; // Instructions
    cout << "Enter a mathematical expression:\n"; // Prompt for expression

    while (true) { // Infinite loop
        getline(cin, expr); // Get the expression from the user
        if (expr.empty()) { // If the expression is empty
            cout << "Please enter a valid mathematical expression.\n"; // Ask the user to enter a valid expression
            continue; // Restart the loop
        }
        try {
            vector<string> tokens = tokenize(expr); // Tokenize the expression
            if (tokens.size() < 3 && (tokens.back() == "+" || tokens.back() == "-" || tokens.back() == "*" || tokens.back() == "/" || tokens.back() == "^")) { // If the expression is incomplete
                throw runtime_error("Incomplete expression"); // Throw an error
            }
            vector<string> postfix = toPostfix(tokens); // Convert the tokens to postfix notation
            double result = evaluatePostfix(postfix); // Evaluate the postfix expression

            cout << "=" << result << endl; // Print the result

            // Memory operations
            cout << "\nType 'mi' to insert the number into memory, or 'mc' to clear memory\n"; // Instructions for memory
            cin >> choice; // Get the user's choice
            if (choice == "mi") memory = result; // If the choice is 'mi', store the result in memory
            else if (choice == "mc") memory = 0; // If the choice is 'mc', clear the memory
            else cout << "command unknown, program will go on\n"; // If the choice is unknown, print a message

            // Loop options
            cout << "\nType 'r' to start again, 'quit' to quit: "; // Instructions to start again or quit
            cin >> choice; // Get the user's choice
            cin.ignore(); // Ignore the newline character
            if (choice == "r") { // If the choice is 'r'
                continue; // Restart the loop
            }
            else if (choice == "quit") { // If the choice is 'quit'
                break; // Break the loop
            }
        }
        catch (const exception& e) { // Catch any exceptions
            cout << "Error: " << e.what() << endl; // Print the error message
            cout << "Please re-enter a valid mathematical expression:\n"; // Ask the user to enter a valid expression
        }
    }

    return 0; // End the program
}
