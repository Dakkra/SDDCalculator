### Work Plan: theademo Calculator Implementation

This document outlines the breakdown of tasks required to implement the features defined in `product_definition.md` while adhering to the standards in `guidelines.md`.

---

#### 1. User Input and CLI Interface [DONE]
Goal: Create an interactive command-line interface to capture mathematical expressions from the user.

- **Task 1.1: Implement REPL (Read-Eval-Print Loop)** [DONE]
  - Modify `main.cpp` to include a loop that prompts the user for input (e.g., `> `).
  - Handle termination commands (e.g., "exit", "quit").
  - Implement clean handling of Ctrl+C (signal interrupt) to gracefully exit or clear current input.
- **Task 1.2: Input Sanitization** [DONE]
  - Implement a basic check to strip whitespace and validate that the input contains only allowed characters (`0-9`, `.`, `+`, `-`, `*`, `/`, `^`, `(`, `)`, and `e`/`E` for scientific notation).
- **Task 1.3: Error Reporting** [DONE]
  - Ensure the CLI can display descriptive error messages for invalid inputs or evaluation failures.

#### 2. Expression Parsing and Evaluation [DONE]
Goal: Transform raw string input into a computed result following PEMDAS rules.

- **Task 2.1: Tokenization** [DONE]
  - Implement a Lexer to break the input string into tokens (Numbers, Operators, Parentheses).
- **Task 2.2: Shunting-Yard Algorithm (Parsing)** [DONE]
  - Implement the Shunting-Yard algorithm to convert Infix notation (e.g., `3 + 4 * 2`) to Reverse Polish Notation (RPN) (e.g., `3 4 2 * +`).
  - Account for operator precedence and associativity (PEMDAS).
  - Handle parentheses for overriding standard precedence.
- **Task 2.3: RPN Evaluator** [DONE]
  - Implement a stack-based evaluator for RPN expressions.
  - Support basic arithmetic (`+`, `-`, `*`, `/`).
  - Implement Exponents (`^`) as recommended by the product definition.
- **Task 2.4: Handling Edge Cases** [DONE]
  - Manage division by zero.
  - Detect mismatched parentheses.
  - Handle malformed expressions (e.g., `5 ++ 2`).

#### 3. Unit Testing Strategy [DONE]
Goal: Ensure the reliability of the calculator components through automated tests.

- **Task 3.1: Set up Testing Framework** [DONE]
  - Integrate a lightweight testing framework (doctest) into `CMakeLists.txt`.
- **Task 3.2: Lexer & Parser Tests** [DONE]
  - Validate that strings are correctly converted to tokens.
  - Verify that the Shunting-Yard algorithm produces the correct RPN for complex expressions.
- **Task 3.3: Evaluation Tests** [DONE]
  - **Basic Arithmetic**: `1+1`, `10-5`, `4*3`, `20/4`.
  - **Precedence**: `2+3*4` (should be 14, not 20).
  - **Parentheses**: `(2+3)*4` (should be 20).
  - **Exponents**: `2^3` (should be 8).
  - **Negative Numbers**: Support for unary minus (e.g., `-5 + 3`).
- **Task 3.4: Error Handling Tests** [DONE]
  - Assert that appropriate errors/exceptions are thrown for invalid syntax or division by zero.
