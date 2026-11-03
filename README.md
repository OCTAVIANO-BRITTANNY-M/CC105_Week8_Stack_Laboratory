# CC105_Week8_Stack_Laboratory
Week 8 Laboratory - Balanced Parentheses and Arithmetic Evaluator in C++
# Week 8 Laboratory: Balanced Parentheses and Arithmetic Evaluator in C++

**National Teachers College**  
**CC105 – Data Structures with Lab**

## Project Information

**Project Title:** Balanced Parentheses and Arithmetic Evaluator in C++  
**Project Type:** Individual Submission  
**Student Developer:** Octaviano, Brittanny Ashley M. Octaviano  
**Section:** 2.4 BSIT  
**Date:** November 4, 2026

## Compiler and Development Environment

- **Compiler:** GCC 16.1.0
- **Language Standard:** C++17
- **IDE:** Code::Blocks
- **Data Structure:** Singly Linked List
- **Operating System:** Windows

## Project Files

- `main.cpp` – Contains the main program, bracket checker, expression converter, postfix evaluator, and menu.
- `Stack.h` – Contains the reusable template Stack class implemented using a singly linked list.
- `evidence/` – Contains screenshots of the program tests and outputs.

## Build and Run Instructions

### Using Code::Blocks

1. Open Code::Blocks.
2. Open or create a C++ project.
3. Add `main.cpp` and `Stack.h` to the project.
4. Make sure both files are in the same folder.
5. Build and run the program.

### Using the Command Line

Compile:

```bash
g++ -std=c++17 main.cpp -o evaluator
```

Run:

```bash
evaluator
```

## Supported Syntax

- Integer operands (e.g., `10`, `25`, `100`)
- Addition (`+`)
- Subtraction (`-`)
- Multiplication (`*`)
- Division (`/`)
- Parentheses (`()`)
- Square brackets (`[]`)
- Curly braces (`{}`)
- Whitespace between tokens

## Limitations

- Decimal numbers are not supported as input.
- Unary positive and negative signs are not supported.
- Implicit multiplication, such as `2(3 + 4)`, is not allowed.
- Only the four basic arithmetic operators are supported.
- Unsupported characters and invalid operator sequences are rejected.
- Division by zero is not allowed.
- Expressions must follow the correct order of operands and operators.

## Algorithm Explanation

### 1. Stack Implementation

The program uses a reusable template `Stack<T>` implemented through a singly linked list. It supports `push()`, `pop()`, `peek()`, `isEmpty()`, and `size()`. New nodes are inserted and removed at the head of the linked list.

### 2. Bracket Checking

The program scans the expression from left to right. Opening brackets are pushed onto the stack. When a closing bracket appears, the program pops the most recent opening bracket and checks whether the types match. The expression is accepted only when all brackets are properly matched.

### 3. Infix-to-Postfix Conversion

The program converts an infix expression into postfix notation using an operator stack. Numbers are added directly to the postfix output, while operators are pushed or popped based on their precedence. Higher-precedence operators are processed before lower-precedence operators. Brackets control the order of operations.

### 4. Postfix Evaluation

The program evaluates the postfix expression using a `Stack<double>`. Numbers are pushed onto the stack. When an operator is encountered, the program pops the right operand first and then the left operand. It performs the operation and pushes the result back onto the stack. The final expression must leave exactly one value.

## Complexity Analysis

Let `n` represent the length of the input expression.

| Operation | Time Complexity | Auxiliary Space |
|---|---|---|
| Bracket Checking | O(n) | O(n) |
| Infix-to-Postfix Conversion | O(n) | O(n) |
| Postfix Evaluation | O(n) | O(n) |
| Linked-List Push | O(1) | O(1) |
| Linked-List Pop | O(1) | O(1) |

The expression algorithms take O(n) time because they scan and process the input tokens in order. Their auxiliary space is O(n) because the stacks and postfix output may store elements proportional to the input length.

## Repository Link

https://github.com/OCTAVIANO-BRITTANNY-M/CC105_Week8_Stack_Laboratory
