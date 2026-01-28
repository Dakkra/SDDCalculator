#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <string>
#include <vector>
#include <stack>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <cctype>

enum class TokenType {
    Number,
    Plus,
    Minus,
    Multiply,
    Divide,
    Power,
    LeftParen,
    RightParen,
    End
};

struct Token {
    TokenType type;
    std::string value;
    int precedence;
    bool rightAssociative;

    Token(TokenType t, std::string v = "", int p = 0, bool ra = false)
        : type(t), value(v), precedence(p), rightAssociative(ra) {}
};

class Lexer {
public:
    explicit Lexer(const std::string& input) : input(input), pos(0) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (pos < input.length()) {
            char current = input[pos];
            if (std::isspace(current)) {
                pos++;
                continue;
            }
            if (std::isdigit(current) || current == '.') {
                tokens.push_back(readNumber());
            } else if (current == '+') {
                tokens.emplace_back(TokenType::Plus, "+", 2);
                pos++;
            } else if (current == '-') {
                bool isUnary = false;
                if (tokens.empty()) {
                    isUnary = true;
                } else {
                    TokenType lastType = tokens.back().type;
                    if (lastType == TokenType::Plus || lastType == TokenType::Minus ||
                        lastType == TokenType::Multiply || lastType == TokenType::Divide ||
                        lastType == TokenType::Power || lastType == TokenType::LeftParen) {
                        isUnary = true;
                    }
                }

                if (isUnary) {
                    if (pos + 1 < input.length() && (std::isdigit(input[pos+1]) || input[pos+1] == '.')) {
                        tokens.push_back(readNumber());
                    } else if (pos + 1 < input.length() && input[pos+1] == '(') {
                        tokens.emplace_back(TokenType::Number, "0");
                        tokens.emplace_back(TokenType::Minus, "-", 2);
                        pos++;
                    } else {
                        tokens.emplace_back(TokenType::Minus, "-", 2);
                        pos++;
                    }
                } else {
                    tokens.emplace_back(TokenType::Minus, "-", 2);
                    pos++;
                }
            } else if (current == '*') {
                tokens.emplace_back(TokenType::Multiply, "*", 3);
                pos++;
            } else if (current == '/') {
                tokens.emplace_back(TokenType::Divide, "/", 3);
                pos++;
            } else if (current == '^') {
                tokens.emplace_back(TokenType::Power, "^", 4, true);
                pos++;
            } else if (current == '(') {
                tokens.emplace_back(TokenType::LeftParen, "(", 0);
                pos++;
            } else if (current == ')') {
                tokens.emplace_back(TokenType::RightParen, ")", 0);
                pos++;
            } else if (current == 'e' || current == 'E') {
                throw std::runtime_error("Unexpected 'e' outside of number");
            } else {
                throw std::runtime_error("Unknown character");
            }
        }
        return tokens;
    }

private:
    std::string input;
    size_t pos;

    Token readNumber() {
        size_t start = pos;
        bool hasDot = false;
        bool hasE = false;

        if (pos < input.length() && input[pos] == '-') {
            pos++;
        }

        while (pos < input.length()) {
            char c = input[pos];
            if (std::isdigit(c)) {
                pos++;
            } else if (c == '.' && !hasDot && !hasE) {
                hasDot = true;
                pos++;
            } else if ((c == 'e' || c == 'E') && !hasE) {
                hasE = true;
                pos++;
                if (pos < input.length() && (input[pos] == '+' || input[pos] == '-')) {
                    pos++;
                }
            } else {
                break;
            }
        }
        return Token(TokenType::Number, input.substr(start, pos - start));
    }
};

class Parser {
public:
    static std::vector<Token> shuntingYard(const std::vector<Token>& tokens) {
        std::vector<Token> outputQueue;
        std::stack<Token> operatorStack;

        for (const auto& token : tokens) {
            if (token.type == TokenType::Number) {
                outputQueue.push_back(token);
            } else if (token.type == TokenType::LeftParen) {
                operatorStack.push(token);
            } else if (token.type == TokenType::RightParen) {
                while (!operatorStack.empty() && operatorStack.top().type != TokenType::LeftParen) {
                    outputQueue.push_back(operatorStack.top());
                    operatorStack.pop();
                }
                if (operatorStack.empty()) {
                    throw std::runtime_error("Mismatched parentheses");
                }
                operatorStack.pop();
            } else {
                while (!operatorStack.empty() && operatorStack.top().type != TokenType::LeftParen) {
                    const auto& top = operatorStack.top();
                    if ((!token.rightAssociative && token.precedence <= top.precedence) ||
                        (token.rightAssociative && token.precedence < top.precedence)) {
                        outputQueue.push_back(top);
                        operatorStack.pop();
                    } else {
                        break;
                    }
                }
                operatorStack.push(token);
            }
        }

        while (!operatorStack.empty()) {
            if (operatorStack.top().type == TokenType::LeftParen) {
                throw std::runtime_error("Mismatched parentheses");
            }
            outputQueue.push_back(operatorStack.top());
            operatorStack.pop();
        }

        return outputQueue;
    }
};

class Evaluator {
public:
    static double evaluateRPN(const std::vector<Token>& rpn) {
        std::stack<double> stack;

        for (const auto& token : rpn) {
            if (token.type == TokenType::Number) {
                stack.push(std::stod(token.value));
            } else {
                if (stack.size() < 2) {
                    throw std::runtime_error("Invalid expression");
                }
                double right = stack.top(); stack.pop();
                double left = stack.top(); stack.pop();

                switch (token.type) {
                    case TokenType::Plus: stack.push(left + right); break;
                    case TokenType::Minus: stack.push(left - right); break;
                    case TokenType::Multiply: stack.push(left * right); break;
                    case TokenType::Divide:
                        if (right == 0) throw std::runtime_error("Division by zero");
                        stack.push(left / right);
                        break;
                    case TokenType::Power: stack.push(std::pow(left, right)); break;
                    default: throw std::runtime_error("Unknown operator");
                }
            }
        }

        if (stack.size() != 1) {
            throw std::runtime_error("Invalid expression");
        }
        return stack.top();
    }
};

#endif
