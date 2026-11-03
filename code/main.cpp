
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <cctype>
#include <cmath>
#include <algorithm>
#include "Stack.h"

using namespace std;

// UI FUNCTIONS

const int UI_WIDTH = 64;

void printLine(char symbol = '=', int width = UI_WIDTH) {
    for (int i = 0; i < width; i++) {
        cout << symbol;
    }
    cout << '\n';
}

void printCentered(const string& text, int width = UI_WIDTH) {
    int leftPadding = (width - static_cast<int>(text.length())) / 2;

    if (leftPadding < 0) {
        leftPadding = 0;
    }

    int rightPadding = width - static_cast<int>(text.length()) - leftPadding;

    if (rightPadding < 0) {
        rightPadding = 0;
    }

    cout << string(leftPadding, ' ') << text
         << string(rightPadding, ' ') << '\n';
}

void printHeader() {
    cout << '\n';
    printLine('=');
    printCentered("BALANCED PARENTHESES AND");
    printCentered("ARITHMETIC EVALUATOR");
    printCentered("STACK-BASED EXPRESSION SYSTEM");
    printLine('=');
}

void printSection(const string& title) {
    cout << '\n';
    printLine('-');
    printCentered(title);
    printLine('-');
}

void printSuccess(const string& message) {
    cout << "\n  [SUCCESS] " << message << '\n';
}

void printError(const string& message) {
    cout << "\n  [ERROR] " << message << '\n';
}

void printMenu() {
    printHeader();

    cout << '\n';
    printCentered("MAIN MENU");
    printLine('-');

    cout << "  [1]  Check Brackets\n";
    cout << "  [2]  Convert and Evaluate Expression\n";
    cout << "  [3]  Run Stack Demonstration\n";
    cout << "  [0]  Exit Program\n";

    printLine('-');
    cout << "  Enter your choice: ";
}

// BRACKET CHECKING FUNCTIONS

bool isOpeningBracket(char ch) {
    return ch == '(' || ch == '[' || ch == '{';
}

bool isClosingBracket(char ch) {
    return ch == ')' || ch == ']' || ch == '}';
}

bool bracketsMatch(char opening, char closing) {
    return (opening == '(' && closing == ')') ||
           (opening == '[' && closing == ']') ||
           (opening == '{' && closing == '}');
}

bool checkBalancedBrackets(const string& expression, string& error) {
    Stack<char> brackets;

    for (char ch : expression) {
        if (isOpeningBracket(ch)) {
            brackets.push(ch);
        } else if (isClosingBracket(ch)) {
            char opening;

            if (!brackets.pop(opening)) {
                error = "Unmatched closing bracket: ";
                error += ch;
                return false;
            }

            if (!bracketsMatch(opening, ch)) {
                error = "Mismatched brackets: ";
                error += opening;
                error += " and ";
                error += ch;
                return false;
            }
        }
    }

    if (!brackets.isEmpty()) {
        char opening;
        brackets.peek(opening);

        error = "Unmatched opening bracket: ";
        error += opening;
        return false;
    }

    error.clear();
    return true;
}

// INFIX-TO-POSTFIX FUNCTIONS

int precedence(char op) {
    if (op == '*' || op == '/') {
        return 2;
    }

    if (op == '+' || op == '-') {
        return 1;
    }

    return 0;
}

bool convertToPostfix(
    const string& expression,
    vector<string>& postfix,
    string& error
) {
    postfix.clear();

    if (expression.find_first_not_of(" \t\n\r") == string::npos) {
        error = "Empty expression.";
        return false;
    }

    Stack<char> operators;
    bool expectOperand = true;
    bool foundToken = false;

    for (size_t i = 0; i < expression.length();) {
        char ch = expression[i];

        if (isspace(static_cast<unsigned char>(ch))) {
            i++;
            continue;
        }

        if (isdigit(static_cast<unsigned char>(ch))) {
            if (!expectOperand) {
                error = "Missing operator between operands.";
                return false;
            }

            string number;

            while (i < expression.length() &&
                   isdigit(static_cast<unsigned char>(expression[i]))) {
                number += expression[i];
                i++;
            }

            postfix.push_back(number);
            expectOperand = false;
            foundToken = true;
            continue;
        }

        if (isOpeningBracket(ch)) {
            if (!expectOperand) {
                error = "Missing operator before opening bracket.";
                return false;
            }

            operators.push(ch);
            i++;
            foundToken = true;
            continue;
        }

        if (isClosingBracket(ch)) {
            if (expectOperand) {
                error = "Missing operand or empty bracket group.";
                return false;
            }

            char topOperator;

            while (operators.peek(topOperator) &&
                   !isOpeningBracket(topOperator)) {
                operators.pop(topOperator);
                postfix.push_back(string(1, topOperator));
            }

            if (!operators.pop(topOperator)) {
                error = "Unmatched closing bracket.";
                return false;
            }

            if (!bracketsMatch(topOperator, ch)) {
                error = "Mismatched brackets.";
                return false;
            }

            expectOperand = false;
            i++;
            continue;
        }

        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            if (expectOperand) {
                error = "Invalid operator sequence or missing operand.";
                return false;
            }

            char topOperator;

            // Pop higher or equal precedence operators to maintain left associativity.
            while (operators.peek(topOperator) &&
                   !isOpeningBracket(topOperator) &&
                   precedence(topOperator) >= precedence(ch)) {
                operators.pop(topOperator);
                postfix.push_back(string(1, topOperator));
            }

            operators.push(ch);
            expectOperand = true;
            i++;
            continue;
        }

        error = "Unsupported character: ";
        error += ch;
        return false;
    }

    if (!foundToken) {
        error = "Empty expression.";
        return false;
    }

    if (expectOperand) {
        error = "Expression ends with an operator or missing operand.";
        return false;
    }

    char remainingOperator;

    while (operators.pop(remainingOperator)) {
        if (isOpeningBracket(remainingOperator)) {
            error = "Unmatched opening bracket.";
            return false;
        }

        postfix.push_back(string(1, remainingOperator));
    }

    error.clear();
    return true;
}

void displayPostfix(const vector<string>& postfix) {
    for (size_t i = 0; i < postfix.size(); i++) {
        cout << postfix[i];

        if (i + 1 < postfix.size()) {
            cout << " ";
        }
    }

    cout << '\n';
}

// POSTFIX EVALUATION FUNCTION

bool evaluatePostfix(
    const vector<string>& postfix,
    double& result,
    string& error
) {
    Stack<double> operands;

    for (const string& token : postfix) {
        if (!token.empty() &&
            all_of(token.begin(), token.end(),
                   [](unsigned char ch) { return isdigit(ch); })) {

            try {
                size_t convertedLength = 0;
                double number = stod(token, &convertedLength);

                if (convertedLength != token.length() || !isfinite(number)) {
                    error = "Invalid or non-finite number.";
                    return false;
                }

                operands.push(number);
            } catch (...) {
                error = "Number conversion error.";
                return false;
            }
        } else if (token == "+" || token == "-" ||
                   token == "*" || token == "/") {

            double rightOperand;
            double leftOperand;

            // Pop the right operand first, followed by the left operand.
            if (!operands.pop(rightOperand) ||
                !operands.pop(leftOperand)) {
                error = "Not enough operands for the operator.";
                return false;
            }

            double answer = 0.0;

            if (token == "+") {
                answer = leftOperand + rightOperand;
            } else if (token == "-") {
                answer = leftOperand - rightOperand;
            } else if (token == "*") {
                answer = leftOperand * rightOperand;
            } else if (token == "/") {
                if (rightOperand == 0.0) {
                    error = "Division by zero is not allowed.";
                    return false;
                }

                answer = leftOperand / rightOperand;
            }

            if (!isfinite(answer)) {
                error = "Result is not a finite number.";
                return false;
            }

            operands.push(answer);
        } else {
            error = "Invalid postfix token.";
            return false;
        }
    }

    if (operands.size() != 1) {
        error = "Invalid postfix expression: extra or missing operands.";
        return false;
    }

    operands.pop(result);
    error.clear();
    return true;
}

// STACK DEMONSTRATION

void runStackDemo() {
    Stack<int> numbers;
    int value;

    printHeader();
    printSection("STACK DEMONSTRATION");

    cout << "\n  Step 1: Initial Stack\n";
    numbers.displayBottomToTop();
    cout << "  Current Size: " << numbers.size() << '\n';

    cout << "\n  Step 2: Test POP on an Empty Stack\n";

    if (!numbers.pop(value)) {
        printError("POP failed. Stack is empty.");
    }

    cout << "\n  Step 3: Test PEEK on an Empty Stack\n";

    if (!numbers.peek(value)) {
        printError("PEEK failed. Stack is empty.");
    }

    cout << "\n  Step 4: Push 10, 20, and 30\n";

    numbers.push(10);
    numbers.push(20);
    numbers.push(30);

    numbers.displayBottomToTop();
    cout << "  Current Size: " << numbers.size() << '\n';

    if (numbers.peek(value)) {
        cout << "  Top Element: " << value << '\n';
    }

    cout << "\n  Step 5: Pop the Top Element\n";

    if (numbers.pop(value)) {
        cout << "  Removed Element: " << value << '\n';
    }

    numbers.displayBottomToTop();
    cout << "  Current Size: " << numbers.size() << '\n';

    cout << "\n  Step 6: Clear Remaining Elements\n";

    numbers.clear();

    numbers.displayBottomToTop();
    cout << "  Current Size: " << numbers.size() << '\n';

    printSuccess("Stack demonstration completed.");
    printLine('=');
}

// MENU OPTION 1: BRACKET CHECKING

void checkBracketsOption() {
    string expression;
    string error;

    printHeader();
    printSection("BRACKET CHECKER");

    cout << "\n  Enter an expression: ";
    getline(cin, expression);

    cout << "\n  EXPRESSION\n";
    printLine('-');
    cout << "  " << (expression.empty() ? "(Empty input)" : expression) << '\n';

    cout << "\n  VALIDATION RESULT\n";
    printLine('-');

    if (checkBalancedBrackets(expression, error)) {
        printSuccess("All brackets are balanced.");
    } else {
        printError(error);
    }

    printLine('=');
}

// MENU OPTION 2: CONVERSION AND EVALUATION

void convertAndEvaluateOption() {
    string expression;
    string error;
    vector<string> postfix;
    double result;

    printHeader();
    printSection("EXPRESSION CONVERTER AND EVALUATOR");

    cout << "\n  Enter an arithmetic expression: ";
    getline(cin, expression);

    cout << "\n  [1] INPUT EXPRESSION\n";
    printLine('-');
    cout << "  " << (expression.empty() ? "(Empty input)" : expression) << '\n';

    cout << "\n  [2] BRACKET VALIDATION\n";
    printLine('-');

    if (!checkBalancedBrackets(expression, error)) {
        printError(error);
        cout << "  Expression rejected.\n";
        printLine('=');
        return;
    }

    printSuccess("Brackets are balanced.");

    cout << "\n  [3] POSTFIX CONVERSION\n";
    printLine('-');

    if (!convertToPostfix(expression, postfix, error)) {
        printError(error);
        cout << "  Expression rejected.\n";
        printLine('=');
        return;
    }

    cout << "  Postfix Expression: ";
    displayPostfix(postfix);

    cout << "\n  [4] POSTFIX EVALUATION\n";
    printLine('-');

    if (!evaluatePostfix(postfix, result, error)) {
        printError(error);
        cout << "  Expression rejected.\n";
        printLine('=');
        return;
    }

    printSuccess("Expression evaluated successfully.");

    cout << "\n  FINAL RESULT\n";
    printLine('=');
    cout << fixed << setprecision(6);
    cout << "       " << result << '\n';
    printLine('=');
}

// MENU INPUT VALIDATION

bool readMenuChoice(int& choice) {
    string input;

    if (!getline(cin, input)) {
        return false;
    }

    try {
        size_t position = 0;
        int parsedChoice = stoi(input, &position);

        while (position < input.length() &&
               isspace(static_cast<unsigned char>(input[position]))) {
            position++;
        }

        if (position != input.length()) {
            return false;
        }

        choice = parsedChoice;
        return true;
    } catch (...) {
        return false;
    }
}

// MAIN PROGRAM

int main() {
    int choice;

    while (true) {
        printMenu();

        if (!readMenuChoice(choice)) {
            if (cin.eof()) {
                cout << "\n\n  Input ended. Program terminated.\n";
                break;
            }

            printError("Invalid menu input. Please enter a valid number.");
            cout << "  Available choices: 0, 1, 2, or 3.\n";
            continue;
        }

        switch (choice) {
            case 1:
                checkBracketsOption();
                break;

            case 2:
                convertAndEvaluateOption();
                break;

            case 3:
                runStackDemo();
                break;
            case 0:
                printHeader();
                printSuccess("Thank you for using the evaluator.");
                cout << "  Program terminated.\n";
                printLine('=');
                return 0;

            default:
                printError("Invalid choice.");
                cout << "  Please select 0, 1, 2, or 3.\n";
                break;
        }
    }

    return 0;
}
