#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <csignal>
#include "calculator.hpp"

volatile std::sig_atomic_t gSignalStatus;

void signal_handler(int signal) {
    gSignalStatus = signal;
}

bool isValidInput(const std::string& input) {
    const std::string allowedChars = "0123456789.+-*/^()eE";
    return std::all_of(input.begin(), input.end(), [&](char c) {
        return allowedChars.find(c) != std::string::npos;
    });
}

std::string sanitizeInput(std::string input) {
    input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
    return input;
}

int main() {
    std::signal(SIGINT, signal_handler);

    std::string input;
    std::cout << "theademo Calculator (type 'exit' or 'quit' to quit)" << std::endl;

    while (true) {
        if (gSignalStatus == SIGINT) {
            std::cout << "\nInterrupt received. Exiting..." << std::endl;
            break;
        }

        std::cout << "> ";
        if (!std::getline(std::cin, input)) {
            if (std::cin.eof()) {
                break;
            }
            std::cin.clear();
            continue;
        }

        if (input.empty()) {
            continue;
        }

        std::string lowerInput = input;
        std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);
        if (lowerInput == "exit" || lowerInput == "quit") {
            break;
        }

        std::string sanitized = sanitizeInput(input);
        if (sanitized.empty()) {
            continue;
        }

        if (!isValidInput(sanitized)) {
            std::cerr << "Error: Invalid characters in input. Allowed: 0-9, ., +, -, *, /, ^, (), e/E" << std::endl;
            continue;
        }

        try {
            Lexer lexer(sanitized);
            std::vector<Token> tokens = lexer.tokenize();
            std::vector<Token> rpn = Parser::shuntingYard(tokens);
            double result = Evaluator::evaluateRPN(rpn);
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}