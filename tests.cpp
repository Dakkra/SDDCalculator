#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "calculator.hpp"
#include "doctest.h"

TEST_CASE("Lexer: Tokenization") {
    SUBCASE("Basic numbers") {
        Lexer lexer("123.45");
        auto tokens = lexer.tokenize();
        CHECK(tokens.size() == 1);
        CHECK(static_cast<int>(tokens[0].type) == static_cast<int>(TokenType::Number));
        CHECK(tokens[0].value == "123.45");
    }

    SUBCASE("Operators") {
        Lexer lexer("+-*/^()");
        auto tokens = lexer.tokenize();
        CHECK(tokens.size() == 7);
        CHECK(static_cast<int>(tokens[0].type) == static_cast<int>(TokenType::Plus));
        CHECK(static_cast<int>(tokens[1].type) == static_cast<int>(TokenType::Minus));
        CHECK(static_cast<int>(tokens[2].type) == static_cast<int>(TokenType::Multiply));
        CHECK(static_cast<int>(tokens[3].type) == static_cast<int>(TokenType::Divide));
        CHECK(static_cast<int>(tokens[4].type) == static_cast<int>(TokenType::Power));
        CHECK(static_cast<int>(tokens[5].type) == static_cast<int>(TokenType::LeftParen));
        CHECK(static_cast<int>(tokens[6].type) == static_cast<int>(TokenType::RightParen));
    }

    SUBCASE("Scientific notation") {
        Lexer lexer("1.2e-3");
        auto tokens = lexer.tokenize();
        CHECK(tokens.size() == 1);
        CHECK(static_cast<int>(tokens[0].type) == static_cast<int>(TokenType::Number));
        CHECK(tokens[0].value == "1.2e-3");
    }

    SUBCASE("Invalid character") {
        Lexer lexer("1 + $");
        CHECK_THROWS_AS(lexer.tokenize(), std::runtime_error);
    }
}

TEST_CASE("Parser: Shunting-Yard") {
    SUBCASE("Simple expression") {
        Lexer lexer("3+4");
        auto tokens = lexer.tokenize();
        auto rpn = Parser::shuntingYard(tokens);
        // RPN: 3 4 +
        CHECK(rpn.size() == 3);
        CHECK(rpn[0].value == "3");
        CHECK(rpn[1].value == "4");
        CHECK(static_cast<int>(rpn[2].type) == static_cast<int>(TokenType::Plus));
    }

    SUBCASE("Operator precedence") {
        Lexer lexer("3+4*2");
        auto tokens = lexer.tokenize();
        auto rpn = Parser::shuntingYard(tokens);
        // RPN: 3 4 2 * +
        CHECK(rpn.size() == 5);
        CHECK(rpn[0].value == "3");
        CHECK(rpn[1].value == "4");
        CHECK(rpn[2].value == "2");
        CHECK(static_cast<int>(rpn[3].type) == static_cast<int>(TokenType::Multiply));
        CHECK(static_cast<int>(rpn[4].type) == static_cast<int>(TokenType::Plus));
    }

    SUBCASE("Parentheses") {
        Lexer lexer("(3+4)*2");
        auto tokens = lexer.tokenize();
        auto rpn = Parser::shuntingYard(tokens);
        // RPN: 3 4 + 2 *
        CHECK(rpn.size() == 5);
        CHECK(rpn[0].value == "3");
        CHECK(rpn[1].value == "4");
        CHECK(static_cast<int>(rpn[2].type) == static_cast<int>(TokenType::Plus));
        CHECK(rpn[3].value == "2");
        CHECK(static_cast<int>(rpn[4].type) == static_cast<int>(TokenType::Multiply));
    }

    SUBCASE("Mismatched parentheses") {
        Lexer lexer("(3+4");
        auto tokens = lexer.tokenize();
        CHECK_THROWS_WITH(Parser::shuntingYard(tokens), "Mismatched parentheses");

        Lexer lexer2("3+4)");
        auto tokens2 = lexer2.tokenize();
        CHECK_THROWS_WITH(Parser::shuntingYard(tokens2), "Mismatched parentheses");
    }
}

TEST_CASE("Evaluator: RPN Evaluation") {
    SUBCASE("Basic arithmetic") {
        CHECK(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("1+1").tokenize())) == doctest::Approx(2.0));
        CHECK(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("10-5").tokenize())) == doctest::Approx(5.0));
        CHECK(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("4*3").tokenize())) == doctest::Approx(12.0));
        CHECK(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("20/4").tokenize())) == doctest::Approx(5.0));
    }

    SUBCASE("Precedence and Parentheses") {
        CHECK(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("2+3*4").tokenize())) == doctest::Approx(14.0));
        CHECK(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("(2+3)*4").tokenize())) == doctest::Approx(20.0));
    }

    SUBCASE("Exponents") {
        CHECK(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("2^3").tokenize())) == doctest::Approx(8.0));
        CHECK(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("2^3^2").tokenize())) == doctest::Approx(512.0)); // Right associativity: 2^(3^2) = 2^9 = 512
    }

    SUBCASE("Division by zero") {
        CHECK_THROWS_WITH(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("5/0").tokenize())), "Division by zero");
    }

    SUBCASE("Negative numbers") {
        CHECK(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("-5+3").tokenize())) == doctest::Approx(-2.0));
        CHECK(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("3+-5").tokenize())) == doctest::Approx(-2.0));
        CHECK(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("-5--3").tokenize())) == doctest::Approx(-2.0));
        CHECK(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("-(5+3)").tokenize())) == doctest::Approx(-8.0));
    }

    SUBCASE("Malformed expressions") {
        CHECK_THROWS_AS(Evaluator::evaluateRPN(Parser::shuntingYard(Lexer("5+").tokenize())), std::runtime_error);
    }
}
