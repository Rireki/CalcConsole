#include <iostream> // Include the library for input and output
#include <string>   // Include the library for using strings
#include <sstream>  // Include the library for string streams
#include <stack>    // Include the library for using stacks
#include <vector>   // Include the library for using vectors
#include <cctype>   // Include the library for character type functions
#include <cmath>    // Include the library for math functions
#include <stdexcept> // Include the library for handling exceptions

using namespace std; // Use the standard namespace for simplicity

// Function to calculate the factorial of a number
unsigned int factorial(int a) {
    int r = 1; // Start with 1
    for (int i = a; i > 0; i--) r *= i; // Multiply r by each number from a down to 1
    return r; // Return the result
}

// Function to determine the precedence of an operator
int precedence(char op) {
    if (op == '+' || op == '-') return 1; // + and - have the lowest precedence
    if (op == '*' || op == '/') return 2; // * and / have higher precedence
    if (op == '^') return 3; // ^ has the highest precedence
    return 0; // Other characters have no precedence
}

// Function to apply an operator to two numbers
double applyOp(double a, double b, char op) {
    switch (op) { // Check which operator we are dealing with
    case '+': return a + b; // Add a and b
    case '-': return a - b; // Subtract b from a
    case '*': return a * b; // Multiply a and b
    case '/': return a / b; // Divide a by b
    case '^': return pow(a, b); // Raise a to the power of b
    }
    return 0; // Return 0 for any other case (shouldn't happen)
}

// Function to split an expression into tokens (numbers and operators)
vector<string> tokenize(string expr) {
    vector<string> tokens; // Create a vector to hold the tokens
    string number; // Create a string to build up numbers
    for (char& c : expr) { // Loop through each character in the expression
        if (isdigit(c) || c == '.') { // If the character is a digit or a decimal point
            number += c; // Add it to the current number
        }
        else { // If the character is not a digit
            if (!number.empty()) { // If we have a number built up
                tokens.push_back(number); // Add the number to the tokens
                number.clear(); // Clear the current number
            }
            if (!isspace(c)) { // If the character is not a space
                tokens.push_back(string(1, c)); // Add the character as a token
            }
        }
    }
    if (!number.empty()) { // If we have a number left at the end
        tokens.push_back(number); // Add it to the tokens
    }
    return tokens; // Return the list of tokens
}

// Function to convert tokens from infix to postfix notation
vector<string> toPostfix(vector<string> tokens) {
    vector<string> output; // Create a vector to hold the output
    stack<string> operators; // Create a stack to hold operators
    for (string& token : tokens) { // Loop through each token
        if (isdigit(token[0]) || (token[0] == '.' && token.size() > 1)) { // If the token is a number
            output.push_back(token); // Add it to the output
        }
        else if (token == "(") { // If the token is an open parenthesis
            operators.push(token); // Push it onto the stack
        }
        else if (token == ")") { // If the token is a close parenthesis
            while (!operators.empty() && operators.top() != "(") { // While there is no matching open parenthesis
                output.push_back(operators.top()); // Pop operators to the output
                operators.pop(); // Remove the operator from the stack
            }
            operators.pop(); // Remove the matching open parenthesis
        }
        else { // If the token is an operator
            while (!operators.empty() && precedence(operators.top()[0]) >= precedence(token[0])) { // While the stack has operators with higher or equal precedence
                output.push_back(operators.top()); // Pop them to the output
                operators.pop(); // Remove the operator from the stack
            }
            operators.push(token); // Push the current operator onto the stack
        }
    }
    while (!operators.empty()) { // If there are any operators left on the stack
        output.push_back(operators.top()); // Pop them to the output
        operators.pop(); // Remove the operator from the stack
    }
    return output; // Return the postfix expression
}

// Function to evaluate a postfix expression
double evaluatePostfix(vector<string> tokens) {
    stack<double> values; // Create a stack to hold values
    for (string& token : tokens) { // Loop through each token
        if (isdigit(token[0]) || (token[0] == '.' && token.size() > 1)) { // If the token is a number
            values.push(stod(token)); // Convert it to a double and push it onto the stack
        }
        else { // If the token is an operator
            double val2 = values.top(); values.pop(); // Pop the top two values from the stack
            double val1 = values.top(); values.pop();
            values.push(applyOp(val1, val2, token[0])); // Apply the operator and push the result onto the stack
        }
    }
    return values.top(); // The result is the top value on the stack
}

int main() {
    double memory = 0; // Initialize memory to 0
    string expr, choice; // Create strings for the expression and user choices

    // Prompt the user for input
    cout << "+ to add, - to subtract, * to multiply, / to divide, ^ to power, ! to factorial\n";
    cout << "Enter a mathematical expression:\n";

    while (getline(cin, expr)) { // Read a line of input
        try {
            vector<string> tokens = tokenize(expr); // Tokenize the expression
            vector<string> postfix = toPostfix(tokens); // Convert to postfix notation
            double result = evaluatePostfix(postfix); // Evaluate the postfix expression

            cout << "=" << result << endl; // Print the result

            // Memory operations
            cout << "\nType 'mi' to insert the number into memory, or 'mc' to clear memory\n";
            cin >> choice;
            if (choice == "mi") memory = result; // Insert result into memory
            else if (choice == "mc") memory = 0; // Clear the memory
            else cout << "command unknown, program will go on\n";

            // Loop options
            cout << "\nType 'r' to start again, 'quit' to quit: ";
            cin >> choice;
            if (choice == "r") {
                cin.ignore(); // Clear the newline left in the buffer
                continue; // Start again
            }
            else if (choice == "quit") {
                break; // Quit the program
            }
        }
        catch (const exception& e) { // If there's an error
            cout << "Error: " << e.what() << endl; // Print the error message
        }
    }

    return 0; // End the program
}